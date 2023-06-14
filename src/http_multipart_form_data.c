#include<http_multipart_form_data.h>

#include<stdlib.h>

// return -1 if boundary is not read
int read_prefix_multipart_form_data(stream* strm, const dstring* boundary)
{
	if(is_empty_dstring(boundary))
		return -1;

	int error = 0;
	size_t bytes_read = skip_dstring_from_stream(strm, boundary, &error);
	if(error || bytes_read == 0)
		return -1;

	return 0;
}

static multipart_form_data_segment* new_multipart_form_data_segment()
{
	multipart_form_data_segment* seg = malloc(sizeof(multipart_form_data_segment));
	init_dmap(&(seg->headers), 1);
	return seg;
}

multipart_form_data_segment* parse_next_multipart_form_data(stream* strm, const dstring* boundary, int* error)
{

}

void destroy_multipart_form_data_segment(multipart_form_data_segment* seg)
{
	int error = 0;
	close_stream(&(seg->body_stream), &error);
	deinitialize_stream(&(seg->body_stream));
	deinit_dmap(&(seg->headers));
	free(seg);
}