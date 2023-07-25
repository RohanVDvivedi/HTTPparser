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
	init_empty_dstring(&__boundary, get_char_count_dstring(boundary) + 4);
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

int get_name_n_filename_from_content_disposition_header(const multipart_form_data_segment* seg, dstring* name, dstring* filename)
{
	int name_set = 0;
	int filename_set = 0;
	for_each_equals_in_dmap(content_disposition_entry, &(seg->headers), &content_disposition_HKEY)
	{
		for_each_split_by_delim(content_disposition_value, &(content_disposition_entry->value), &SCL)
		{
			trim_dstring(&content_disposition_value);

			if(0 == compare_dstring(&content_disposition_value, &form_data_cd_HVAL))
				continue;

			dstring cd_key;
			dstring cd_value = split_dstring(&content_disposition_value, &EQ, &cd_key);

			if(get_byte_array_dstring(&cd_value) == NULL)
				continue;

			trim_dstring(&cd_key);
			trim_dstring(&cd_value);

			if(!name_set && 0 == compare_dstring(&cd_key, &get_dstring_pointing_to_literal_cstring("name")))
			{
				name_set = 1;
				concatenate_dstring(name, &cd_value);
			}
			else if(!filename_set && 0 == compare_dstring(&cd_key, &get_dstring_pointing_to_literal_cstring("filename")))
			{
				filename_set = 1;
				concatenate_dstring(filename, &cd_value);
			}

			if(name_set && filename_set)
				break;
		}
		break;
	}

	return (filename_set << 1) | name_set;
}

void destroy_multipart_form_data_segment(multipart_form_data_segment* seg)
{
	int error = 0;
	close_stream(&(seg->body_stream), &error);
	deinitialize_stream(&(seg->body_stream));
	deinit_dmap(&(seg->headers));
	free(seg);
}