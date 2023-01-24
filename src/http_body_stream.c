#include<http_body_stream.h>

#include<stdlib.h>
#include<string.h>

#include<cutlery_math.h>

#include<stream_util.h>

static unsigned int read_body_from_stream_body(void* stream_context, void* data, unsigned int data_size, int* error)
{
	http_body_stream_context* stream_context_p = stream_context;

	if(stream_context_p->is_closed)
		return 0;

	switch(stream_context_p->is_chunked)
	{
		case 0 :
		{
			unsigned int bytes_to_read = min(data_size, stream_context_p->body_bytes);
			unsigned int bytes_read = read_from_stream(stream_context_p->underlying_stream, data, bytes_to_read, error);
			stream_context_p->body_bytes -= bytes_read;
			if(stream_context_p->body_bytes == 0)
				stream_context_p->is_closed = 1;
			return bytes_read;
			break;
		}
		case 1 :
		{
			dstring CRLF = get_literal_cstring("\r\n");

			if(stream_context_p->body_bytes == 0)
			{
				uint64_t body_bytes_val;
				unsigned int body_bytes_bytes_read = read_uint64_from_stream(stream_context_p->underlying_stream, HEXADECIMAL, &body_bytes_val, error);
				if(*error)
					return 0;
				if(body_bytes_bytes_read == 0 || body_bytes_val > UINT_MAX)
				{
					(*error) = -1;
					return 0;
				}
				stream_context_p->body_bytes = body_bytes_val;
				if(stream_context_p->body_bytes == 0)
					stream_context_p->is_closed = 1;

				{
					unsigned int CRLF_spml[3];
					get_prefix_suffix_match_lengths(&CRLF, CRLF_spml);
					dstring to_discard = read_dstring_until_from_stream(stream_context_p->underlying_stream, &CRLF, CRLF_spml, 1024, error);
					if((*error))
					{
						deinit_dstring(&to_discard);
						return 0;
					}
					if(get_char_count_dstring(&to_discard) == 0)
					{
						deinit_dstring(&to_discard);
						(*error) = -1;
						return 0;
					}
					deinit_dstring(&to_discard);
				}
			}

			unsigned int bytes_to_read = min(stream_context_p->body_bytes, data_size);
			unsigned int bytes_read = read_from_stream(stream_context_p->underlying_stream, data, bytes_to_read, error);
			stream_context_p->body_bytes -= bytes_read;

			if(stream_context_p->body_bytes == 0)
			{
				unsigned int crlf_bytes_read = skip_dstring_from_stream(stream_context_p->underlying_stream, &CRLF, error);
				if((*error))
					return bytes_read;
				if(crlf_bytes_read == 0)
				{
					(*error) = -1;
					return bytes_read;
				}
			}
			break;
		}
	}

	return 0;
}

#define WRITE_MAX_CHUNK_SIZE 4096

static unsigned int write_body_to_stream_body(void* stream_context, const void* data, unsigned int data_size, int* error)
{
	http_body_stream_context* stream_context_p = stream_context;

	if(stream_context_p->is_closed)
	{
		(*error) = -1;
		return 0;
	}

	if(data_size == 0)
		return 0;

	switch(stream_context_p->is_chunked)
	{
		case 0 :
		{
			unsigned int bytes_to_write = min(data_size, stream_context_p->body_bytes);
			unsigned int bytes_written = write_to_stream(stream_context_p->underlying_stream, data, bytes_to_write, error);
			stream_context_p->body_bytes -= bytes_written;
			if(stream_context_p->body_bytes == 0)
				stream_context_p->is_closed = 1;
			return bytes_written;
			break;
		}
		case 1 :
		{
			unsigned int bytes_to_write = min(WRITE_MAX_CHUNK_SIZE, data_size);
			write_to_stream_formatted(stream_context_p->underlying_stream, "%x\r\n%.*s\r\n", error, bytes_to_write, bytes_to_write, data, error);
			return bytes_to_write;
			break;
		}
	}

	return 0;
}

static void close_writable_stream_context_body_stream(void* stream_context, int* error)
{
	http_body_stream_context* stream_context_p = stream_context;

	// if the stream_context is chunked we need to send one last empty chunk
	if(stream_context_p->is_chunked && !stream_context_p->is_closed)
	{
		static const char* last_chunk = "0\r\n\r\n";
		write_to_stream(stream_context_p->underlying_stream, last_chunk, strlen(last_chunk), error);
		stream_context_p->is_closed = 1;
	}

	// a chunked stream is closed after a 0 sized chunk is written to the stream
	// while a non chunked stream is closed after body_bytes reach 0
}

static void close_readable_stream_context_body_stream(void* stream_context, int* error){}
// a readable http body stream is closed only upon reading a chunk of size 0, or when its context length number of bytes are read
// hence this is a NOP

static void destroy_stream_context_body_stream(void* stream_context)
{
	free(stream_context);
}

// returns 1 for success and 0 for error
static int init_body_stream_context(http_body_stream_context* stream_context_p, const dmap* headers)
{
	stream_context_p->body_bytes = 0;
	stream_context_p->is_chunked = 0;

	dmap_entry* content_length = get_from_dmap(headers, &get_literal_cstring("content-length"));
	if(content_length != NULL)
	{
		dstring clv = get_trimmed_dstring_pointing_to(&(content_length->value));
		if(is_empty_dstring(&clv) || !get_unsigned_int_from_dstring(&clv, DECIMAL, &(stream_context_p->body_bytes)))
			return 0;
		return 1;
	}

	dstring chunked = get_literal_cstring("chunked");
	unsigned int spml_chunked[8];
	get_prefix_suffix_match_lengths(&chunked, spml_chunked);

	for(const dmap_entry* transfer_encoding = get_from_dmap(headers, &get_literal_cstring("transfer-encoding")); transfer_encoding != NULL; transfer_encoding = get_next_of_in_hashmap(headers, transfer_encoding, ANY_THAT_EQUALS))
	{
		if(contains_dstring_KMP(&(transfer_encoding->value), &chunked, spml_chunked) != INVALID_INDEX)
		{
			stream_context_p->is_chunked = 1;
			return 1;
		}
	}

	return 0;
}

int initialize_readable_body_stream(stream* strm, stream* underlying_stream, const dmap* headers)
{
	http_body_stream_context* stream_context = malloc(sizeof(http_body_stream_context));

	// intialize stream context
	stream_context->underlying_stream = underlying_stream;
	stream_context->is_closed = 0;

	if(!init_body_stream_context(stream_context, headers))
	{
		free(stream_context);
		return 0;
	}

	if(!stream_context->is_chunked && stream_context->body_bytes == 0)
	{
		free(stream_context);
		return 0;
	}

	initialize_stream(strm, stream_context, read_body_from_stream_body, NULL, close_readable_stream_context_body_stream, destroy_stream_context_body_stream);

	return 1;
}

int initialize_writable_body_stream(stream* strm, stream* underlying_stream, const dmap* headers)
{
	http_body_stream_context* stream_context = malloc(sizeof(http_body_stream_context));

	// intialize stream context
	stream_context->underlying_stream = underlying_stream;
	stream_context->is_closed = 0;

	if(!init_body_stream_context(stream_context, headers))
	{
		free(stream_context);
		return 0;
	}

	if(!stream_context->is_chunked && stream_context->body_bytes == 0)
	{
		free(stream_context);
		return 0;
	}

	initialize_stream(strm, stream_context, NULL, write_body_to_stream_body, close_writable_stream_context_body_stream, destroy_stream_context_body_stream);

	return 1;
}