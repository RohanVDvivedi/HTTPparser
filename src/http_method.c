#include<http_method.h>

#include<dstring.h>

#include<stream_util.h>

const char* http_method_strings[] = {
	"GET",
	"POST",
	"PUT",
	"HEAD",
	"DELETE",
	"PATCH",
	"OPTIONS",
	"TRACE",
	"CONNECT"
};

int parse_http_method(stream* rs, http_method* m)
{
	char byte;
	cy_uint byte_read = 0;
	int stream_error = 0;

	int method_count = sizeof(http_method_strings)/sizeof(char*);

	int can_be_count = sizeof(http_method_strings)/sizeof(char*);
	int can_not_be[sizeof(http_method_strings)/sizeof(char*)] = {};

	cy_uint bytes_matched = 0;

	int res = -1;

	while(can_be_count > 0 && res == -1)
	{
		byte_read = read_from_stream(rs, &byte, 1, &stream_error);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;
		if(byte_read == 0)
			return HTTP_PARSER_ERROR;

		for(int i = 0; i < method_count; i++)
		{
			if(can_not_be[i])
				continue;

			if(byte != http_method_strings[i][bytes_matched])
			{
				can_not_be[i] = 1;
				can_be_count--;
			}
			else if(http_method_strings[i][bytes_matched+1] == '\0')
			{
				res = i;
				break;
			}
		}

		bytes_matched++;
	}

	if(res == -1)
		return HTTP_PARSER_ERROR;

	(*m) = res;

	return HTTP_NO_ERROR;
}

int serialize_http_method(stream* ws, const http_method* m)
{
	int stream_error = 0;
	write_dstring_to_stream(ws, &get_dstring_pointing_to_cstring(http_method_strings[(*m)]), &stream_error);
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;

	return HTTP_NO_ERROR;
}