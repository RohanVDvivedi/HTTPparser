#include<http_response_line.h>

#include<http_constant_dstrings.h>
#include<http_version.h>
#include<http_status.h>

#include<stream_util.h>

#include<ctype.h>

int parse_http_response_line(stream* rs, http_response_head* hr_p)
{
	int error = parse_http_version(rs, &(hr_p->version));
	if(error)
		return error;

	int stream_error = 0;

	// skip at most 5 spaces
	#define MAX_SPACES 5
	size_t space_bytes = skip_whitespaces_from_stream(rs, MAX_SPACES, &stream_error);
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;
	if(space_bytes == 0)
		return HTTP_PARSER_ERROR;

	error = parse_http_status_line(rs, &(hr_p->status));
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

int serialize_http_response_line(stream* ws, const http_response_head* hr_p)
{
	int error = serialize_http_version(ws, &(hr_p->version));
	if(error)
		return error;

	int stream_error = 0;
	write_dstring_to_stream(ws, &SP, &stream_error); // " "
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;

	error = serialize_http_status_line(ws, &(hr_p->status));
	if(error)
		return error;

	write_dstring_to_stream(ws, &CRLF, &stream_error);	// "\r\n"
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;

	return HTTP_NO_ERROR;
}