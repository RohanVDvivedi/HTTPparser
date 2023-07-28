#ifndef HTTP_REQUEST_LINE_H
#define HTTP_REQUEST_LINE_H

#include<stream.h>

#include<http_request.h>

#include<http_parser_error_codes.h>

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int parse_http_request_line(stream* rs, http_request_head* hr_p);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int serialize_http_request_line(stream* ws, const http_request_head* hr_p);

#endif