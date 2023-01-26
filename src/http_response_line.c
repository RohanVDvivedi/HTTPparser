#include<http_response_line.h>

#include<http_version.h>
#include<http_status.h>

#include<stream_util.h>

#include<ctype.h>

#include<http_constant_dstrings.h>

int parse_http_response_line(stream* rs, http_response* hr_p)
{
	if(parse_http_version(rs, &(hr_p->version)) == -1)
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

	if(parse_http_status_line(rs, &(hr_p->status)) == -1)
		return -1;

	// skip reading the "\r\n"
	{
		unsigned int line_end_read = skip_dstring_from_stream(rs, &CRLF, &error);
		if(line_end_read == 0 || error)
			return -1;
	}

	return 0;
}

int serialize_http_response_line(stream* ws, const http_response* hr_p)
{
	if(serialize_http_version(ws, &(hr_p->version)) == -1)
		return -1;

	int error = 0;
	write_dstring_to_stream(ws, &SP, &error); // " "
	if(error)
		return -1;

	if(serialize_http_status_line(ws, &(hr_p->status)) == -1)
		return -1;

	write_dstring_to_stream(ws, &CRLF, &error);	// "\r\n"
	if(error)
		return -1;

	return 0;
}