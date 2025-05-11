#ifndef HTTP_STATUS_H
#define HTTP_STATUS_H

#include<cutlery/stream.h>

#include<httpparser/http_parser_error_codes.h>

extern const int http_status_codes[];
extern char const * const http_status_code_reason_strings[];

const char* get_http_status_line(int status);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int parse_http_status_line(stream* rs, int* s);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int serialize_http_status_line(stream* ws, const int* s);

#endif