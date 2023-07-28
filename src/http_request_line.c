#include<http_request_line.h>

#include<http_constant_dstrings.h>
#include<http_path_and_path_params.h>
#include<http_version.h>

#include<stream_util.h>

#include<ctype.h>

int parse_http_request_line(stream* rs, http_request_head* hr_p)
{
	int error = parse_http_method(rs, &(hr_p->method));
	if(error)
		return error;

	int stream_error = 0;

	// skip spaces
	#define MAX_SPACES 5
	size_t space_bytes = skip_whitespaces_from_stream(rs, MAX_SPACES, &stream_error);
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;
	if(space_bytes == 0)
		return HTTP_PARSER_ERROR;

	error = parse_http_path_and_path_params(rs, hr_p);
	if(error)
		return error;

	// skip spaces
	space_bytes = skip_whitespaces_from_stream(rs, MAX_SPACES, &stream_error);
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;
	if(space_bytes == 0)
		return HTTP_PARSER_ERROR;

	error = parse_http_version(rs, &(hr_p->version));
	if(error)
		return error;

	// skip reading the "\r\n"
	size_t line_end_read = skip_dstring_from_stream(rs, &CRLF, &stream_error);
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;
	if(line_end_read == 0)
		return HTTP_PARSER_ERROR;

	return HTTP_NO_ERROR;
}

int serialize_http_request_line(stream* ws, const http_request_head* hr_p)
{
	int error = serialize_http_method(ws, &(hr_p->method));
	if(error)
		return error;

	int stream_error = 0;
	write_dstring_to_stream(ws, &SP, &stream_error); // " "
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;

	error = serialize_http_path_and_path_params(ws, hr_p);
	if(error)
		return error;

	write_dstring_to_stream(ws, &SP, &stream_error);  // " "
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;

	error = serialize_http_version(ws, &(hr_p->version));
	if(error)
		return error;

	write_dstring_to_stream(ws, &CRLF, &stream_error); // "\r\n"
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;

	return HTTP_NO_ERROR;
}