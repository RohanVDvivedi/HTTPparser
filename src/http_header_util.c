#include<http_header_util.h>

#include<http_constant_dstrings.h>

int parse_acceptable_value(const dstring* singlular_header_value, acceptable_value* av_p)
{
	// initalize the point_dstr input parameter
	dstring shv = get_dstring_pointing_to_dstring(singlular_header_value);
	trim_dstring(&shv);

	// initialize return value
	init_empty_dstring(&(av_p->value), 0);
	av_p->q_value = 1.0;

	// split at ";"" to get to get value
	dstring split;
	dstring remaining = split_dstring(&shv, &SCL, &split);

	// concatenate split to value
	concatenate_dstring(&(av_p->value), &split);

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
	return 0;

	RETURN_ERROR:;
	deinit_dstring(&(av_p->value));
	return -1;
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
			if(-1 == parse_acceptable_value(&value, &av))
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

#include<file_handling_util.h>

int check_content_type_acceptable(const dstring* content_type, const http_request_head* hrq_p)
{
	for_each_equals_in_dmap(accept_entry, &(hrq_p->headers), &accept_HKEY)
	{
		for_each_split_by_delim(value, &(accept_entry->value), &CM)
		{
			trim_dstring(&value);

			acceptable_value av;
			if(-1 == parse_acceptable_value(&value, &av))
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