#ifndef HTTP_PATH_AND_PATH_PARAMS_H
#define HTTP_PATH_AND_PATH_PARAMS_H

#include<http_request.h>

#include<http_parser_error_codes.h>

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int parse_http_path_and_path_params(stream* rs, http_request_head* hr_p);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int serialize_http_path_and_path_params(stream* ws, const http_request_head* hr_p);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
// an error of HTTP_OBJECT_INVALID_ERROR implies an end of params
int parse_url_encoded_param(stream* rs, dstring* key, dstring* value, int is_first_param);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int parse_url_encoded_params(stream* rs, dmap* params);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
// in the below functions set the is_first_param flag if this is the first param being written to the stream
int serialize_url_encoded_param(stream* ws, const dstring* key, const dstring* value, int is_first_param);

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int serialize_url_encoded_params(stream* ws, const dmap* params);

// internal function exposed api

// use this only if you know what you are doing
// returns 1 for success and 0 for an error
int uri_to_dstring_format(const dstring* str, dstring* res);

#endif