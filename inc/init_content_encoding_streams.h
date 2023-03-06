#ifndef INIT_CONTENT_ENCODING_STREAMS_H
#define INIT_CONTENT_ENCODING_STREAMS_H

#include<dmap.h>
#include<stacked_stream.h>

// return value is the number of encodings stacked in the stacked_stream sstrm
// -1 implies unidentified encoding

int initialize_readable_content_decoding_stream(stacked_stream* sstrm, dmap* headers);

int initialize_writable_content_encoding_stream(stacked_stream* sstrm, dmap* headers);

#endif