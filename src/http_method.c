#include<http_method.h>

#include<string.h>

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
	unsigned int byte_read = 0;
	int error = 0;

	int method_count = sizeof(http_method_strings)/sizeof(char*);

	int can_be_count = sizeof(http_method_strings)/sizeof(char*);
	int can_not_be[sizeof(http_method_strings)/sizeof(char*)] = {};

	unsigned int bytes_matched = 0;

	int res = -1;

	while(can_be_count > 0 && res == -1)
	{
		byte_read = read_from_stream(rs, &byte, 1, &error);
		if(byte_read == 0 || error != 0)
			return -1;

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
		return -1;

	(*m) = res;

	return 0;
}

int serialize_http_method(stream* ws, const http_method* m)
{
	int error = 0;

	write_to_stream(ws, http_method_strings[(*m)], strlen(http_method_strings[(*m)]), &error);
	if(error)
		return -1;

	return 0;
}