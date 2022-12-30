#include<stdio.h>
#include<stdlib.h>

#include<http_status.h>

#include<file_descriptor_stream.h>

int main()
{
	stream rs, ws;
	initialize_stream_for_fd(&rs, 0);
	initialize_stream_for_fd(&ws, 1);
	
	int s = 53;

	if(parse_http_status(&rs, &s))
	{
		printf("parse error\n");
		return 0;
	}

	if(serialize_http_status(&ws, &s))
	{
		printf("serliazation error\n");
		return 0;
	}

	deinitialize_stream(&rs);
	deinitialize_stream(&ws);

	return 0;
}