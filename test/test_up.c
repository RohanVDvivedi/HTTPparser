#include<stdio.h>
#include<stdlib.h>

#include<uri_parser.h>

int main(int argc, char** argv)
{
	if(argc != 2)
	{
		printf("invalid argument count, it must be 2\n");
		return -1;
	}

	uri uriv;
	init_uri(&uriv);
	int res = parse_uri(&uriv, &get_dstring_pointing_to_cstring(argv[1]));
	if(res)
		printf("error parsing the uri\n");
	printf("scheme : < " printf_dstring_format " >\n", printf_dstring_params(&(uriv.scheme)));
	printf("userinfo : < " printf_dstring_format " >\n", printf_dstring_params(&(uriv.userinfo)));
	printf("host : < " printf_dstring_format " >\n", printf_dstring_params(&(uriv.host)));
	printf("port : < " printf_dstring_format " >\n", printf_dstring_params(&(uriv.port)));
	printf("path : < " printf_dstring_format " >\n", printf_dstring_params(&(uriv.path)));
	printf("query : < " printf_dstring_format " >\n", printf_dstring_params(&(uriv.query)));
	printf("fragment : < " printf_dstring_format " >\n", printf_dstring_params(&(uriv.fragment)));
	deinit_uri(&uriv);
	return res;
}