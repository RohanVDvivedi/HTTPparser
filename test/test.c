#include<stdio.h>
#include<stdlib.h>

#include<http_version.h>

#include<file_descriptor_stream.h>

int main()
{
	stream rs, ws;
	initialize_stream_for_fd(&rs, 0);
	initialize_stream_for_fd(&ws, 1);
	
	http_version v = {5,3};

	if(parse_http_version(&rs, &v))
	{
		printf("parse error\n");
		return 0;
	}

	if(serialize_http_version(&ws, &v))
	{
		printf("serliazation error\n");
		return 0;
	}

	deinitialize_stream(&rs);
	deinitialize_stream(&ws);

	return 0;
}