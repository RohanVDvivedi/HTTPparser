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
	// TODO
}

int initialize_writable_body_stream(stream* strm, stream* underlying_stream, const dmap* headers)
{
	// TODO
}