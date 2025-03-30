#include<httpparser/http_header_util.h>

#include<httpparser/http_constant_dstrings.h>

int parse_acceptable_value(const dstring* singlular_header_value, acceptable_value* av_p)
{
	// initalize the point_dstr input parameter
	dstring shv = get_dstring_pointing_to_dstring(singlular_header_value);
	trim_dstring(&shv);

	// initialize return value
	av_p->q_value = 1.0;

	// split at ";"" to get to get value
	dstring split;
	dstring remaining = split_dstring(&shv, &SCL, &split);

	// split is the value
	av_p->value = split;

	// trim and check if the remaining is empty, if so this implies that there is only value and no q_value
	ltrim_dstring(&remaining);
	if(is_empty_dstring(&remaining))
		goto RETURN_SUCCESS;

	const char* rem_data = get_byte_array_dstring(&remaining);
	cy_uint rem_size = get_char_count_dstring(&remaining);

	// make sure that there is "q=" before tha actual q value
	if(rem_size < 2 || !is_prefix_of_dstring(&remaining, &get_dstring_pointing_to_literal_cstring("q=")))
		goto RETURN_ERROR;
	discard_chars_from_front_dstring(&remaining, 2);

	rem_data = get_byte_array_dstring(&remaining);
	rem_size = get_char_count_dstring(&remaining);

	// initialize q_value
	av_p->q_value = 0;

	// read everything before decimal point
	cy_uint i = 0;
	for(; i < rem_size && '0' <= rem_data[i] && rem_data[i] <= '9'; i++)
		av_p->q_value = 10 * av_p->q_value + get_digit_from_char(rem_data[i], 10);

	// if we reached end, then return with success
	if(i == rem_size || rem_data[i] == ',' || rem_data[i] == ';')
		goto RETURN_SUCCESS;

	// if we could not find a "." we return error
	if(rem_data[i] != '.' || ((++i) == rem_size))
		goto RETURN_ERROR;

	// read everything after decimal point
	for(double pos = 0.1; i < rem_size && '0' <= rem_data[i] && rem_data[i] <= '9'; i++, pos /= 10)
		av_p->q_value = get_digit_from_char(rem_data[i], 10) * pos;

	// if we reached end, then return succes else return error
	if(i == rem_size || rem_data[i] == ',' || rem_data[i] == ';')
		goto RETURN_SUCCESS;
	else
		goto RETURN_ERROR;

	RETURN_SUCCESS:;
	return HTTP_NO_ERROR;

	RETURN_ERROR:;
	return HTTP_PARSER_ERROR;
}

const dstring* find_acceptable_content_encoding_for_http_response_body(const http_request_head* hrq_p)
{
	const dstring* result_encoding = NULL;

	int none_seen = 1;

	for_each_equals_in_dmap(accept_encoding_entry, &(hrq_p->headers), &accept_encoding_HKEY)
	{
		none_seen = 0;
		for_each_split_by_delim(value, &(accept_encoding_entry->value), &CM)
		{
			trim_dstring(&value);

			acceptable_value av;
			if(HTTP_NO_ERROR != parse_acceptable_value(&value, &av))
				break;

			// a qvalue of 0 implies do not use it
			if(av.q_value == 0)
				continue;

			// q values if present must also be compare but we are not doing it, yet
			if(0 == compare_dstring(&(av.value), &gzip_ce_HVAL))
				result_encoding = &gzip_ce_HVAL;
			else if(0 == compare_dstring(&(av.value), &deflate_ce_HVAL))
				result_encoding = &deflate_ce_HVAL;
			else if(0 == compare_dstring(&(av.value), &identity_ce_HVAL))
				result_encoding = &identity_ce_HVAL;
			else
				continue;

			if(result_encoding != NULL)
				break;
		}
	}

	if(none_seen)
		return &identity_ce_HVAL;

	return result_encoding;
}

#include<httpparser/file_handling_util.h>

int check_content_type_acceptable(const dstring* content_type, const http_request_head* hrq_p)
{
	for_each_equals_in_dmap(accept_entry, &(hrq_p->headers), &accept_HKEY)
	{
		for_each_split_by_delim(value, &(accept_entry->value), &CM)
		{
			trim_dstring(&value);

			acceptable_value av;
			if(HTTP_NO_ERROR != parse_acceptable_value(&value, &av))
				break;

			// a qvalue of 0 implies do not use it
			if(av.q_value == 0)
				continue;

			if(match_accept_to_content_type(content_type, &(av.value)))
				return 1;
		}
	}

	return 0;
}
#include<stdio.h>
int parse_cookies_from_cookie_header(dmap* cookies, const dmap* headers)
{
	for_each_equals_in_dmap(cookie_entry, headers, &cookie_HKEY)
	{
		for_each_split_by_delim(cookie_whole, &(cookie_entry->value), &SCL)
		{
			if(INVALID_INDEX == contains_dstring_RK(&cookie_whole, &EQ))
				return HTTP_PARSER_ERROR;

			dstring cookie_key;
			dstring cookie_value = split_dstring(&cookie_whole, &EQ, &cookie_key);
			trim_dstring(&cookie_key);
			trim_dstring(&cookie_value);

			if(!insert_in_dmap(cookies, &cookie_key, &cookie_value))
				return HTTP_ALLOCATION_ERROR;
		}
	}

	return HTTP_NO_ERROR;
}

int has_url_encoded_params_in_body(const dmap* headers)
{
	for_each_equals_in_dmap(content_type_entry, headers, &content_type_HKEY)
		if(0 == compare_dstring(&(content_type_entry->value), &form_url_encoded_ct_HVAL))
			return 1;
		else
			return 0;
	return 0;
}

int has_multipart_form_data_in_body(const dmap* headers, int* is_boundary_present, dstring* boundary)
{
	int is_multi_part_form_data = 0;
	(*is_boundary_present) = 0;
	for_each_equals_in_dmap(content_type_entry, headers, &content_type_HKEY)
	{
		int is_first = 1;
		for_each_split_by_delim(content_type_value, &(content_type_entry->value), &SCL)
		{
			if(is_first)
			{
				is_first = 0;
				if(0 == compare_dstring(&content_type_value, &multipart_form_data_ct_HVAL))
					is_multi_part_form_data = 1;
				else
					break;
			}
			else
			{
				dstring key;
				dstring val = split_dstring(&content_type_value, &EQ, &key);

				if(get_byte_array_dstring(&val) == NULL)
					continue;

				trim_dstring(&key);
				trim_dstring(&val);

				if(0 == compare_dstring(&key, &get_dstring_pointing_to_literal_cstring("boundary")))
				{
					(*boundary) = get_dstring_pointing_to_dstring(&val);
					(*is_boundary_present) = 1;
					break;
				}
			}
		}
		break;
	}
	return is_multi_part_form_data;
}

#include<httpparser/http_body_stream.h>
#include<httpparser/init_content_encoding_streams.h>

#include<stdlib.h>

int intialize_http_body_and_encoding_streams_for_writing(stacked_stream* sstrm, stream* raw_strm, const dmap* headers)
{
	int result = 0;
	int streams_pushed = 0;

	stream* body_stream = malloc(sizeof(stream));
	if(body_stream == NULL)
	{
		result = -10;
		goto EXIT_0;
	}
	if(!initialize_writable_body_stream(body_stream, raw_strm, headers))
	{
		free(body_stream);
		result = -10;
		goto EXIT_0;
	}
	push_to_stacked_stream(sstrm, body_stream, WRITE_STREAMS);
	streams_pushed++;

	if((result = initialize_writable_content_encoding_stream(sstrm, headers)) < 0)
		goto EXIT_1;
	streams_pushed += result;

	return streams_pushed;

	EXIT_1:;
	close_deinitialize_free_all_from_stacked_stream(sstrm, WRITE_STREAMS);

	EXIT_0:;
	return result;
}

int intialize_http_body_and_decoding_streams_for_reading(stacked_stream* sstrm, stream* raw_strm, const dmap* headers)
{
	int result = 0;
	int streams_pushed = 0;

	stream* body_stream = malloc(sizeof(stream));
	if(body_stream == NULL)
	{
		result = -10;
		goto EXIT_0;
	}
	if(!initialize_readable_body_stream(body_stream, raw_strm, headers))
	{
		free(body_stream);
		result = -10;
		goto EXIT_0;
	}
	push_to_stacked_stream(sstrm, body_stream, READ_STREAMS);
	streams_pushed++;

	if((result = initialize_readable_content_decoding_stream(sstrm, headers)) < 0)
		goto EXIT_1;
	streams_pushed += result;

	return streams_pushed;

	EXIT_1:;
	close_deinitialize_free_all_from_stacked_stream(sstrm, READ_STREAMS);

	EXIT_0:;
	return result;
}