#ifndef HTTP_HEADERS_H
#define HTTP_HEADERS_H

#include<dmap.h>

#include<http_parser_error_codes.h>

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int parse_http_headers(stream* rs, dmap* headers);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int serialize_http_headers(stream* ws, const dmap* headers);

#endif