#ifndef HTTP_HEADER_UTIL_H
#define HTTP_HEADER_UTIL_H

#include<cutlery/dstring.h>

typedef struct acceptable_value acceptable_value;
struct acceptable_value
{
	dstring value; // this is only a pointing dstring, pointing to your header value
	double q_value;
};

// returns HTTP_PARSER_ERROR for error and HTTP_NO_ERROR for success
// as explained in comment above you do not need to destroy av_p, or any of its attributes
int parse_acceptable_value(const dstring* singlular_header_value, acceptable_value* av_p);

#include<httpparser/http_request.h>
#include<httpparser/http_response.h>

const dstring* find_acceptable_content_encoding_for_http_response_body(const http_request_head* hrq_p);

// check if a given content type is acceptable
int check_content_type_acceptable(const dstring* content_type, const http_request_head* hrq_p);

#include<httpparser/http_parser_error_codes.h>

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int parse_cookies_from_cookie_header(dmap* cookies, const dmap* headers);

int has_url_encoded_params_in_body(const dmap* headers);

// the returned boundary is a pointing to dstring, pointing to the boundary present in the headers, hence no need to de allocate it
int has_multipart_form_data_in_body(const dmap* headers, int* is_boundary_present, dstring* boundary);

#include<connman/stacked_stream.h>

// return < 0 (with no streams pushed) for an error
// else a value > 0 for the number of streams initialized and pushed in the stacked_stream
int intialize_http_body_and_encoding_streams_for_writing(stacked_stream* sstrm, stream* raw_strm, const dmap* headers);
int intialize_http_body_and_decoding_streams_for_reading(stacked_stream* sstrm, stream* raw_strm, const dmap* headers);

#endif