#ifndef HTTP_RESPONSE_LINE_H
#define HTTP_RESPONSE_LINE_H

#include<stream.h>

#include<http_response.h>

#include<http_parser_error_codes.h>

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int parse_http_response_line(stream* rs, http_response_head* hr_p);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int serialize_http_response_line(stream* ws, const http_response_head* hr_p);

#endif