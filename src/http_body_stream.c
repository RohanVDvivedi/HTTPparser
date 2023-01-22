#include<http_body_stream.h>

static unsigned int read_body_from_stream_body(void* stream_context, void* data, unsigned int data_size, int* error)
{
	// TODO
}

static unsigned int write_body_to_stream_body(void* stream_context, const void* data, unsigned int data_size, int* error)
{
	// TODO
}

static void close_stream_context_body_stream(void* stream_context, int* error)
{
	// TODO
}

static void destroy_stream_context_body_stream(void* stream_context)
{
	// TODO
}

int initialize_readable_body_stream(stream* strm, stream* underlying_stream, const dmap* headers)
{
	body_stream_context* stream_context = malloc(sizeo(body_stream_context));

	// intialize stream context

	initialize_stream(strm, stream_context, read_body_from_stream_body, NULL, close_stream_context_body_stream, destroy_stream_context_body_stream);

	return 1;
}

int initialize_writable_body_stream(stream* strm, stream* underlying_stream, const dmap* headers)
{
	body_stream_context* stream_context = malloc(sizeo(body_stream_context));

	// intialize stream context

	initialize_stream(strm, stream_context, NULL, write_body_to_stream_body, close_stream_context_body_stream, destroy_stream_context_body_stream);

	return 1;
}