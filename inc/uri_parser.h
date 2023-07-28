#ifndef URI_PARSER_H
#define URI_PARSER_H

#include<dstring.h>

#include<http_parser_error_codes.h>

// scheme ":" [ "//" [ userinfo "@" ] host [ ":" port ] ] path [ "?" query ] [ "#" fragment ]

typedef struct uri uri;
struct uri
{
	dstring scheme;
	dstring userinfo;
	dstring host;
	dstring port;
	dstring path;
	dstring query;
	dstring fragment;
};

void init_uri(uri* uri_p);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int parse_uri(uri* uri_p, const dstring* uri_val);

void deinit_uri(uri* uri_p);

#endif