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
			// TODO
			// if body_bytes == 0
			// 		then read hexadecimal body_bytes
			// 		skip "\r\n", if "\r\n" not read then throw error
			// read min(body_bytes, data_size)
			// body_bytes -= bytes_read
			// if(body_bytes == 0)
			//		skip "\r\n", if "\r\n" not read then throw error
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

int initialize_readable_body_stream(stream* strm, stream* underlying_stream, const dmap* headers)
{
	http_body_stream_context* stream_context = malloc(sizeof(http_body_stream_context));

	// intialize stream context
	stream_context->is_closed = 0;

	// TODO

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
	stream_context->is_closed = 0;

	// TODO

	if(!stream_context->is_chunked && stream_context->body_bytes == 0)
	{
		free(stream_context);
		return 0;
	}

	initialize_stream(strm, stream_context, NULL, write_body_to_stream_body, close_writable_stream_context_body_stream, destroy_stream_context_body_stream);

	return 1;
}