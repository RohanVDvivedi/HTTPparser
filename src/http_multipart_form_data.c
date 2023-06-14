#include<http_multipart_form_data.h>

#include<stream_util.h>
#include<read_until_dstring_stream.h>

#include<http_headers.h>
#include<http_constant_dstrings.h>

#include<stdlib.h>

const dstring __MULTIPART_FORM_DATA = get_dstring_pointing_to_literal_cstring("--");
const dstring MULTIPART_FORM_DATA_end = get_dstring_pointing_to_literal_cstring("--\r\n");

// return -1 if boundary is not read
int read_prefix_multipart_form_data(stream* strm, const dstring* boundary)
{
	dstring __boundary;
	init_empty_dstring(&__boundary, get_char_count_dstring(boundary) + 2);
	concatenate_dstring(&__boundary, &__MULTIPART_FORM_DATA);
	concatenate_dstring(&__boundary, boundary);

	int error = 0;
	size_t bytes_read = skip_dstring_from_stream(strm, &__boundary, &error);
	if(error || bytes_read == 0)
	{
		deinit_dstring(&__boundary);
		return -1;
	}

	deinit_dstring(&__boundary);
	return 0;
}

static multipart_form_data_segment* new_multipart_form_data_segment()
{
	multipart_form_data_segment* seg = malloc(sizeof(multipart_form_data_segment));
	init_dmap(&(seg->headers), 1);
	return seg;
}
#include<stdio.h>
multipart_form_data_segment* parse_next_multipart_form_data(stream* strm, const dstring* boundary, int* error)
{
	(*error) = 0;

	size_t bytes_read = skip_dstring_from_stream(strm, &MULTIPART_FORM_DATA_end, error);
	if((*error))
		return NULL;
	if(bytes_read > 0)
	{
		char byte;
		bytes_read = read_from_stream(strm, &byte, 1, error);
		if(!(*error) && bytes_read > 0)
			(*error) = -1;
		return NULL;
	}

	bytes_read = skip_dstring_from_stream(strm, &CRLF, error);
	if((*error))
		return NULL;
	else if(bytes_read == 0)
	{
		(*error) = -1;
		return NULL;
	}

	multipart_form_data_segment* seg = new_multipart_form_data_segment();
	if(-1 == ((*error) = parse_http_headers(strm, &(seg->headers))))
	{
		deinit_dmap(&(seg->headers));
		free(seg);
		return NULL;
	}

	dstring __boundary;
	init_empty_dstring(&__boundary, get_char_count_dstring(boundary) + 4);
	concatenate_dstring(&__boundary, &CRLF);
	concatenate_dstring(&__boundary, &__MULTIPART_FORM_DATA);
	concatenate_dstring(&__boundary, boundary);

	if(!initialize_stream_for_reading_until_dstring(&(seg->body_stream), strm, &__boundary))
	{
		deinit_dstring(&__boundary);
		deinit_dmap(&(seg->headers));
		free(seg);
		return NULL;
	}

	deinit_dstring(&__boundary);
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