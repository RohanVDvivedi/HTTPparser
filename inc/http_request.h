#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include<http_method.h>
#include<http_version.h>
#include<dmap.h>

typedef struct http_request http_request;
struct http_request
{
	http_method method;

	dstring path;

	dmap path_params;

	http_version version;

	dmap headers;

	// body stream
};

#endif