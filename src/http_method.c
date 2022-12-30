#include<http_method.h>

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

int parse_http_method(stream* rs, http_method* m);

int serialize_http_method(stream* ws, const http_method* m)
{
	int error = 0;

	write_to_stream(ws, http_method_strings[(*m)], strlen(http_method_strings[(*m)]), &error);
	if(error)
		return -1;

	return 0;
}