#ifndef HTTP_MULTIPART_FORM_DATA_H
#define HTTP_MULTIPART_FORM_DATA_H

#include<dmap.h>
#include<dstring.h>

typedef struct multipart_form_data_segment multipart_form_data_segment;
struct multipart_form_data_segment
{
	dmap headers;

	stream body_stream;
};

// return -1 if boundary is not read
int read_prefix_multipart_form_data(stream* strm, const dstring* boundary);

// returns NULL if not new segment present, error will be set on an error
multipart_form_data_segment* parse_next_multipart_form_data(stream* strm, const dstring* boundary, int* error);

void destroy_multipart_form_data_segment(multipart_form_data_segment* seg);

#endif