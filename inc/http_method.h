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
	GET     = 0,
	POST    = 1,
	PUT     = 2,
	HEAD    = 3,
	DELETE  = 4,
	PATCH   = 5,
	OPTIONS = 6,
	TRACE   = 7,
	CONNECT = 8
};

extern const char* http_method_strings[];

// returns -1 for error and 0 for success
int parse_http_method(stream* rs, http_method* m);

// returns -1 for error and 0 for success
int serialize_http_method(stream* ws, const http_method* m);

#endif