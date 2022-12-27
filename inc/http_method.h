#ifndef HTTP_REQUEST_METHOD_H
#define HTTP_REQUEST_METHOD_H

#include<stream.h>

/*
	to add additional http method
	update enum http_method and http_method_strings array
*/

typedef enum http_method http_method;
enum http_method
{
	GET,
	POST,
	PUT,
	HEAD,
	DELETE,
	PATCH,
	OPTIONS,
	TRACE,
	CONNECT
};

extern const char* http_method_strings[];

http_method parse_http_method(stream* rs, int* error);

int serialize_http_method(stream* ws, http_method m);

#endif