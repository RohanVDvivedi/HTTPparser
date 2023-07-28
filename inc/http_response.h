#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include<http_version.h>
#include<http_status.h>
#include<dmap.h>

#include<http_parser_error_codes.h>

typedef struct http_response_head http_response_head;
struct http_response_head
{
	http_version version;

	int status;

	dmap headers;

	// body stream
};

void init_http_response_head(http_response_head* hr_p);

void deinit_http_response_head(http_response_head* hr_p);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int parse_http_response_head(stream* rs, http_response_head* hr_p);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int serialize_http_response_head(stream* ws, const http_response_head* hr_p);

void print_http_response_head(const http_response_head* hr_p);

// additional utility functions

#include<http_request.h>

#define TRANSFER_CHUNKED SIZE_MAX

// returns 1 for success and 0 for failure
// it is a util function, hence it does not return any error code to specify why it failed
int init_http_response_head_from_http_request_head(http_response_head* hrp_p, const http_request_head* hrq_p, int status, size_t content_length_val);

const dstring* find_acceptable_content_encoding_for_response(const http_request_head* hrq_p);

#endif