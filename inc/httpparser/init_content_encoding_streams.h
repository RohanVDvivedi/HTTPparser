#ifndef INIT_CONTENT_ENCODING_STREAMS_H
#define INIT_CONTENT_ENCODING_STREAMS_H

#include<httpparser/dmap.h>
#include<connman/stacked_stream.h>

// return value is the number of encodings stacked in the stacked_stream sstrm
// return >= 0 success
// -1 implies unidentified encoding
// -2 implies memory allocation error in stacks OR stacked_stream
// -3 implies failed to initialize a stream

int initialize_readable_content_decoding_stream(stacked_stream* sstrm, const dmap* headers);

int initialize_writable_content_encoding_stream(stacked_stream* sstrm, const dmap* headers);

#endif