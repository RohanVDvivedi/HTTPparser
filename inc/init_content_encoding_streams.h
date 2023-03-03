#ifndef INIT_CONTENT_ENCODING_STREAMS_H
#define INIT_CONTENT_ENCODING_STREAMS_H

#include<dmap.h>
#include<stream.h>

// return value of 1 is success
// return value of 0 implies a failure, i.e. unsupported encoded stream
// return value of 2 implies the stream is identity encoded hence not required

int initialize_readable_content_decoding_stream(stream* strm, stream* underlying_stream, dmap* headers);

int initialize_writable_content_encoding_stream(stream* strm, stream* underlying_stream, dmap* headers);

#endif