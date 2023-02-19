#ifndef HTTP_PATH_AND_PATH_PARAMS_H
#define HTTP_PATH_AND_PATH_PARAMS_H

#include<http_request.h>

// returns -1 for error and 0 for success
int parse_http_path_and_path_params(stream* rs, http_request* hr_p);

// returns -1 for error and 0 for success
int serialize_http_path_and_path_params(stream* ws, const http_request* hr_p);

// key and value will be made empty and after this call they will contain the next key value pair, unless there is an error (return value = -1)
int parse_url_encoded_param(stream* ws, dstring* key, dstring* value, int* is_last_param);

int parse_url_encoded_params(stream* ws, dmap_p* params);

// in the below functions set the is_first_param flag if this is the first param being written to the stream
int serialize_url_encoded_param(stream* ws, const dstring* key, const dstring* value, int is_first_param);

int serialize_url_encoded_params(stream* ws, const dmap* params);

#endif