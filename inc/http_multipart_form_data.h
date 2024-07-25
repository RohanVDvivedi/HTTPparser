#ifndef HTTP_MULTIPART_FORM_DATA_H
#define HTTP_MULTIPART_FORM_DATA_H

#include<dmap.h>
#include<dstring.h>

typedef struct multipart_form_data_segment multipart_form_data_segment;
struct multipart_form_data_segment
{
	// headers that go along with it
	dmap headers;

	// actual contents of the body of multipart form daa
	stream body_stream;
};

// return -1 if "--boundary" is not read
// this function must be called first on the stream (on top of body stream and decoding streams)
int read_prefix_multipart_form_data(stream* strm, const dstring* boundary);

// returns NULL if no new segment present, error will be set on an error
// the contents of the file/field will be inside the body_stream of the returned multipart_form_segment
// you may call this function again and again (as many times as the number of fields) until it returns NULL
// you must call this function only after reading all the contents of the previous segment
multipart_form_data_segment* parse_next_multipart_form_data(stream* strm, const dstring* boundary, int* error);

#define NAME_PRESENT 			0b01
#define FILENAME_PRESENT 		0b10
#define NAME_n_FILENAME_PRESENT 0b11

// get name and filename values from content_disposition header value
int get_name_n_filename_from_content_disposition_header(const multipart_form_data_segment* seg, dstring* name, dstring* filename);

// you must call destroy function on any of multipart_form_data_segment returned by parse_next_multipart_form_data
void destroy_multipart_form_data_segment(multipart_form_data_segment* seg);

#endif