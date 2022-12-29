#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include<http_version.h>
#include<http_status.h>
#include<dmap.h>

typedef struct http_response http_response;
struct http_response
{
	http_version version;

	int status;

	dmap headers;

	// body stream
};

#endif