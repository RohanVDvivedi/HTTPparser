#ifndef HTTP_REQUEST_METHOD_H
#define HTTP_REQUEST_METHOD_H

#include<cutlery/stream.h>

#include<httpparser/http_parser_error_codes.h>

/*
	to add additional http method
	update enum http_method and http_method_strings array
*/

typedef enum http_method http_method;
enum http_method
{
	GET = 0, // values assigned 0 and so on
	POST,
	PUT,
	HEAD,
	DELETE,
	PATCH,
	OPTIONS,
	TRACE,
	CONNECT
};

extern char const * const http_method_strings[];

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int parse_http_method(stream* rs, http_method* m);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int serialize_http_method(stream* ws, const http_method* m);

#endif