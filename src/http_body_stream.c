#include<http_body_stream.h>

#include<http_constant_dstrings.h>

#include<stdlib.h>
#include<string.h>

#include<cutlery_math.h>

#include<stream_util.h>

static unsigned int read_body_from_stream_body(void* stream_context, void* data, unsigned int data_size, int* error)
{
	http_body_stream_context* stream_context_p = stream_context;

	if(stream_context_p->is_closed)
		return 0;

	int u_error = 0;

	if(!stream_context_p->is_chunked)
	{
		unsigned int bytes_to_read = min(data_size, stream_context_p->body_bytes);
		unsigned int bytes_read = read_from_stream(stream_context_p->underlying_stream, data, bytes_to_read, &u_error);
		if(u_error)
			(*error) = UNDERLYING_STREAM_ERROR;
		stream_context_p->body_bytes -= bytes_read;
		if(stream_context_p->body_bytes == 0)
			stream_context_p->is_closed = 1;
		return bytes_read;
	}
	else
	{
		if(stream_context_p->body_bytes == 0)
		{
			uint64_t body_bytes_val;
			unsigned int body_bytes_bytes_read = read_uint64_from_stream(stream_context_p->underlying_stream, HEXADECIMAL, &body_bytes_val, &u_error);
			if(u_error)
			{
				(*error) = UNDERLYING_STREAM_ERROR;
				return 0;
			}
			if(body_bytes_bytes_read == 0 || body_bytes_val > SIZE_MAX)
			{
				(*error) = -1;
				return 0;
			}
			stream_context_p->body_bytes = body_bytes_val;
			if(stream_context_p->body_bytes == 0)
				stream_context_p->is_closed = 1;

			{
				dstring to_discard = read_until_dstring_from_stream(stream_context_p->underlying_stream, &CRLF, CRLF_spml, 1024, &u_error);
				if(u_error)
				{
					(*error) = UNDERLYING_STREAM_ERROR;
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
		unsigned int bytes_read = read_from_stream(stream_context_p->underlying_stream, data, bytes_to_read, &u_error);
		stream_context_p->body_bytes -= bytes_read;
		if(u_error)
			(*error) = UNDERLYING_STREAM_ERROR;
		if(stream_context_p->body_bytes == 0)
		{
			unsigned int crlf_bytes_read = skip_dstring_from_stream(stream_context_p->underlying_stream, &CRLF, &u_error);
			if(u_error)
				(*error) = UNDERLYING_STREAM_ERROR;
			else if(crlf_bytes_read == 0)
				(*error) = -1;
		}
		return bytes_read;
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

	int u_error = 0;

	if(!stream_context_p->is_chunked)
	{
		unsigned int bytes_to_write = min(data_size, stream_context_p->body_bytes);
		write_to_stream(stream_context_p->underlying_stream, data, bytes_to_write, &u_error);
		if(u_error)
		{
			(*error) = UNDERLYING_STREAM_ERROR;
			return 0;
		}
		stream_context_p->body_bytes -= bytes_to_write;
		if(stream_context_p->body_bytes == 0)
			stream_context_p->is_closed = 1;
		return bytes_to_write;
	}
	else
	{
		unsigned int bytes_to_write = min(WRITE_MAX_CHUNK_SIZE, data_size);
		write_to_stream_formatted(stream_context_p->underlying_stream, &u_error, "%x\r\n%.*s\r\n", bytes_to_write, bytes_to_write, data);
		if(u_error)
		{
			(*error) = UNDERLYING_STREAM_ERROR;
			return 0;
		}
		return bytes_to_write;
	}
}

static void post_http_body_stream_flush_underlying_stream_flush(void* stream_context, int* error)
{
	http_body_stream_context* stream_context_p = stream_context;
	flush_all_from_stream(stream_context_p->underlying_stream, error);
}

static void close_writable_stream_context_body_stream(void* stream_context, int* error)
{
	http_body_stream_context* stream_context_p = stream_context;

	// if the stream_context is chunked we need to send one last empty chunk
	if(stream_context_p->is_chunked && !stream_context_p->is_closed)
	{
		int u_error = 0;
		write_dstring_to_stream(stream_context_p->underlying_stream, &LAST_CHUNK, &u_error);
		if(!u_error)
		{
			flush_all_from_stream(stream_context_p->underlying_stream, &u_error);
			stream_context_p->is_closed = 1;
		}
		if(u_error)
			(*error) = UNDERLYING_STREAM_ERROR;
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

	dmap_entry* content_length_entry = get_from_dmap(headers, &content_length);
	if(content_length_entry != NULL)
	{
		dstring clv = get_dstring_pointing_to_dstring(&(content_length_entry->value));
		trim_dstring(&clv);
		if(is_empty_dstring(&clv) || !get_unsigned_long_long_int_from_dstring(&clv, DECIMAL, &(stream_context_p->body_bytes)))
			return 0;
		return 1;
	}

	for_each_equals_in_dmap(transfer_encoding_entry, headers, &transfer_encoding)
	{
		if(contains_dstring_KMP(&(transfer_encoding_entry->value), &chunked, chunked_spml) != INVALID_INDEX)
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

	initialize_stream(strm, stream_context, read_body_from_stream_body, NULL, close_readable_stream_context_body_stream, destroy_stream_context_body_stream, NULL, 0);

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

	initialize_stream(strm, stream_context, NULL, write_body_to_stream_body, close_writable_stream_context_body_stream, destroy_stream_context_body_stream, post_http_body_stream_flush_underlying_stream_flush, WRITE_MAX_CHUNK_SIZE);

	return 1;
}