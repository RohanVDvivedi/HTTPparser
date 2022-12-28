#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include<http_method.h>
#include<dmap.h>

typedef struct http_request http_request;
struct http_request
{
	http_method method;

	dstring path;

	dmap path_params;

	// version number
	int major;
	int minor;

	dmap headers;

	// body stream
};

#endif