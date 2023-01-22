#ifndef HTTP_BODY_STREAM_H
#define HTTP_BODY_STREAM_H

#include<stream.h>
#include<dmap.h>

typedef struct http_body_stream_context http_body_stream_context;
struct http_body_stream_context
{
	stream* underlying_stream;

	// there can be a closed stream
	int is_closed:1;

	// whether the body is chunked or not
	int is_chunked:1;

	// body_bytes to be read next
	unsigned int body_bytes;
};

int initialize_readable_body_stream(stream* strm, stream* underlying_stream, const dmap* headers);

int initialize_writable_body_stream(stream* strm, stream* underlying_stream, const dmap* headers);

#endif