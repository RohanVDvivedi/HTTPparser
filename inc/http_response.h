#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include<http_method.h>
#include<dmap.h>

typedef struct http_response http_response;
struct http_response
{
	// version number
	int major;
	int minor;

	int status;

	dmap headers;

	// body stream
};

#endif