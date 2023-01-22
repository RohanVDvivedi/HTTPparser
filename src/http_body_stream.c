#include<http_body_stream.h>

#include<stdlib.h>
#include<string.h>

static unsigned int read_body_from_stream_body(void* stream_context, void* data, unsigned int data_size, int* error)
{
	http_body_stream_context* stream_context_p = stream_context;

	// TODO
}

static unsigned int write_body_to_stream_body(void* stream_context, const void* data, unsigned int data_size, int* error)
{
	http_body_stream_context* stream_context_p = stream_context;

	// TODO
}

static void close_writable_stream_context_body_stream(void* stream_context, int* error)
{
	http_body_stream_context* stream_context_p = stream_context;

	// if the stream_context is chunked we need to send one last empty chunk
	if(stream_context_p->is_chunked)
	{
		static const char* last_chunk = "0\r\n\r\n";
		write_to_stream(stream_context_p->underlying_stream, last_chunk, strlen(last_chunk), error);
	}
}

static void close_readable_stream_context_body_stream(void* stream_context, int* error){}

static void destroy_stream_context_body_stream(void* stream_context)
{
	free(stream_context);
}

int initialize_readable_body_stream(stream* strm, stream* underlying_stream, const dmap* headers)
{
	http_body_stream_context* stream_context = malloc(sizeof(http_body_stream_context));

	// intialize stream context
	// TODO

	initialize_stream(strm, stream_context, read_body_from_stream_body, NULL, close_readable_stream_context_body_stream, destroy_stream_context_body_stream);

	return 1;
}

int initialize_writable_body_stream(stream* strm, stream* underlying_stream, const dmap* headers)
{
	http_body_stream_context* stream_context = malloc(sizeof(http_body_stream_context));

	// intialize stream context
	// TODO

	initialize_stream(strm, stream_context, NULL, write_body_to_stream_body, close_writable_stream_context_body_stream, destroy_stream_context_body_stream);

	return 1;
}