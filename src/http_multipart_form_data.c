#include<http_multipart_form_data.h>

#include<stream_util.h>
#include<read_until_dstring_stream.h>

#include<stdlib.h>

#include<http_headers.h>

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

const dstring MULTIPART_FORM_DATA_end = get_dstring_pointing_to_literal_cstring("--");

multipart_form_data_segment* parse_next_multipart_form_data(stream* strm, const dstring* boundary, int* error)
{
	(*error) = 0;

	size_t bytes_read = skip_dstring_from_stream(strm, &MULTIPART_FORM_DATA_end, error);
	if((*error))
		return NULL;

	if(bytes_read == 0)
		goto PARSE_HEADER;

	char byte;
	size_t byte_read = read_from_stream(strm, &byte, 1, error);
	if((*error) || byte_read == 0)
		return NULL;

	// unread all of the contents that we read until now
	unread_from_stream(strm, &byte, 1);
	unread_dstring_from_stream(strm, &MULTIPART_FORM_DATA_end);

	PARSE_HEADER:;

	multipart_form_data_segment* seg = new_multipart_form_data_segment();
	if(-1 == ((*error) = parse_http_headers(strm, &(seg->headers))))
	{
		deinit_dmap(&(seg->headers));
		free(seg);
		return NULL;
	}

	if(!initialize_stream_for_reading_until_dstring(&(seg->body_stream), strm, boundary))
	{
		deinit_dmap(&(seg->headers));
		free(seg);
		return NULL;
	}

	return seg;
}

void destroy_multipart_form_data_segment(multipart_form_data_segment* seg)
{
	int error = 0;
	close_stream(&(seg->body_stream), &error);
	deinitialize_stream(&(seg->body_stream));
	deinit_dmap(&(seg->headers));
	free(seg);
}