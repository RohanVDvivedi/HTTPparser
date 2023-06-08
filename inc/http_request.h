#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include<http_method.h>
#include<http_version.h>
#include<dmap.h>

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

void init_http_request(http_request_head* hr_p);

void deinit_http_request(http_request_head* hr_p);

// returns -1 for error and 0 for success
int parse_http_request_head(stream* rs, http_request_head* hr_p);

// returns -1 for error and 0 for success
int serialize_http_request_head(stream* ws, const http_request_head* hr_p);

void print_http_request(const http_request_head* hr_p);

#endif