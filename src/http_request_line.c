#include<http_request_line.h>

#include<http_version.h>
#include<http_status.h>

#include<stream_util.h>

#include<ctype.h>

int parse_http_request_line(stream* rs, http_request* hr_p)
{
	if(parse_http_method(rs, &(hr_p->method)) == -1)
		return -1;

	int error = 0;

	// skip spaces
	{
		#define MAX_SPACES 5
		unsigned int space_bytes = skip_whitespaces_from_stream(rs, MAX_SPACES, &error);
		if(space_bytes == 0 || error)
			return -1;

		// make sure that the next byte is not a whitespace
		{
			char byte;
			unsigned int byte_read = read_from_stream(rs, &byte, 1, &error);
			if(byte_read == 0 || error || isspace(byte))
				return -1;
			unread_from_stream(rs, &byte, 1);
		}
	}

	// TODO parse path and path params

	// skip spaces
	{
		#define MAX_SPACES 5
		unsigned int space_bytes = skip_whitespaces_from_stream(rs, MAX_SPACES, &error);
		if(space_bytes == 0 || error)
			return -1;

		// make sure that the next byte is not a whitespace
		{
			char byte;
			unsigned int byte_read = read_from_stream(rs, &byte, 1, &error);
			if(byte_read == 0 || error || isspace(byte))
				return -1;
			unread_from_stream(rs, &byte, 1);
		}
	}

	if(parse_http_version(rs, &(hr_p->version)) == -1)
		return -1;

	// skip reading the "\r\n"
	{
		unsigned int line_end_read = skip_dstring_from_stream(rs, &get_literal_cstring("\r\n"), &error);
		if(line_end_read == 0 || error)
			return -1;
	}

	return 0;
}

int serialize_http_request_line(stream* ws, const http_request* hr_p)
{
	if(serialize_http_method(ws, &(hr_p->method)) == -1)
		return -1;

	int error = 0;
	static const char space = ' ';

	write_to_stream(ws, &space, 1, &error);
	if(error)
		return -1;

	// TODO serialize path and path params

	write_to_stream(ws, &space, 1, &error);
	if(error)
		return -1;

	if(serialize_http_version(ws, &(hr_p->version)) == -1)
		return -1;

	// serilialize "\r\n"
	static const char* CRLF = "\r\n";
	write_to_stream(ws, CRLF, 2, &error);
	if(error)
		return -1;

	return 0;
}