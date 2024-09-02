#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include<http_method.h>
#include<http_version.h>
#include<dmap.h>

#include<http_parser_error_codes.h>

typedef struct http_request_head http_request_head;
struct http_request_head
{
	http_method method;

	dstring path;

	dmap path_params;

	http_version version;

	dmap headers;

	// body stream
};

// returns 1 for success and 0 for failure
// may fail only dure to memory allocation failure errors
int init_http_request_head(http_request_head* hr_p);

// returns 1 for success and 0 for error
// this is a util function, hence it does not return any error code that might have revealed why it failed
int init_http_request_head_from_uri(http_request_head* hr_p, const dstring* uri_str);

void deinit_http_request_head(http_request_head* hr_p);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int parse_http_request_head(stream* rs, http_request_head* hr_p);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int serialize_http_request_head(stream* ws, const http_request_head* hr_p);

void print_http_request_head(const http_request_head* hr_p);

#endif