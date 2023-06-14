#include<http_multipart_form_data.h>

// return -1 if boundary is not read
int read_prefix_multipart_form_data(stream* strm, const dstring* boundary)
{

}

multipart_form_data_segment* parse_next_multipart_form_data(stream* strm, const dstring* boundary, int* error)
{

}

void destroy_multipart_form_data_segment(multipart_form_data_segment* seg)
{
	
}