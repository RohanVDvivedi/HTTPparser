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

void init_http_response(http_response* hr_p);

// returns -1 for error and 0 for success
int parse_http_response_head(stream* rs, http_response* hr_p);

// returns -1 for error and 0 for success
int serialize_http_response_head(stream* ws, const http_response* hr_p);

#endif