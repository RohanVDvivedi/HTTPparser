#include<http_response.h>

void init_http_response_head(http_response_head* hr_p)
{
	init_dmap(&(hr_p->headers), 1);
}

void deinit_http_response_head(http_response_head* hr_p)
{
	deinit_dmap(&(hr_p->headers));
}

#include<http_response_line.h>
#include<http_headers.h>

int parse_http_response_head(stream* rs, http_response_head* hr_p)
{
	int error = 0;

	error = parse_http_response_line(rs, hr_p);
	if(error)
		return -1;

	error = parse_http_headers(rs, &(hr_p->headers));
	if(error)
		return -1;

	return 0;
}

int serialize_http_response_head(stream* ws, const http_response_head* hr_p)
{
	int error = 0;

	error = serialize_http_response_line(ws, hr_p);
	if(error)
		return -1;

	error = serialize_http_headers(ws, &(hr_p->headers));
	if(error)
		return -1;

	return 0;
}

#include<http_constant_dstrings.h>

const dstring* find_acceptable_content_encoding_for_response(const http_request_head* hrq_p)
{
	const dstring* result_encoding = NULL;

	int none_seen = 1;

	for_each_equals_in_dmap(accept_encoding_entry, &(hrq_p->headers), &accept_encoding)
	{
		for_each_split_by_delim(value, &(accept_encoding_entry->value), &CM)
		{
			trim_dstring(&value);
			none_seen = 0;

			// q values if present must also be compare but we are not doing it, yet
			if(is_prefix_of_dstring(&value, &gzip_ce))
				result_encoding = &gzip_ce;
			else if(is_prefix_of_dstring(&value, &deflate_ce))
				result_encoding = &deflate_ce;
			else if(is_prefix_of_dstring(&value, &identity_ce))
				result_encoding = &identity_ce;
			else
				continue;

			if(result_encoding != NULL)
				break;
		}
	}

	if(none_seen)
		return &identity_ce;

	return result_encoding;
}

#include<stdio.h>
#include<stdint.h>

void init_http_response_head_from_http_request_head(http_response_head* hrp_p, const http_request_head* hrq_p, int status, size_t content_length_val)
{
	init_http_response_head(hrp_p);
	hrp_p->version = hrq_p->version;
	hrp_p->status = status;
	if(content_length_val == TRANSFER_CHUNKED)
	{
		insert_in_dmap(&(hrp_p->headers), &transfer_encoding, &chunked);
		const dstring* content_encoding_val = find_acceptable_content_encoding_for_response(hrq_p);
		if(content_encoding_val)
			insert_in_dmap(&(hrp_p->headers), &content_encoding, content_encoding_val);
	}
	else
	{
		char content_length_in_decimal[128];
		sprintf(content_length_in_decimal, "%zu", content_length_val);
		insert_in_dmap(&(hrp_p->headers), &content_length, &get_dstring_pointing_to_cstring(content_length_in_decimal));
		insert_in_dmap(&(hrp_p->headers), &content_encoding, &identity_ce);
	}
}

void print_http_response_head(const http_response_head* hr_p)
{
	printf("version = %d.%d\n", hr_p->version.major, hr_p->version.minor);
	printf("status = %d\n", hr_p->status);
	printf("headers = \n");
	for_each_in_dmap(e, &(hr_p->headers))
		printf("\t<" printf_dstring_format "> -> <" printf_dstring_format ">\n", printf_dstring_params(&(e->key)), printf_dstring_params(&(e->value)));
}