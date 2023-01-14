#include<stdio.h>
#include<stdlib.h>

#include<http_request_line.h>
#include<http_response_line.h>

#include<file_descriptor_stream.h>

#define TEST_REQUEST
//#define TEST_RESPONSE

int main()
{
	stream rs, ws;
	initialize_stream_for_fd(&rs, 0);
	initialize_stream_for_fd(&ws, 1);
	
	#if defined TEST_REQUEST
	{
		http_request hr;
		init_http_request(&hr);

		if(parse_http_request_head(&rs, &hr))
		{
			printf("parse error\n");
			return 0;
		}

		printf("path : " printf_dstring_format "\n", printf_dstring_params(&(hr.path)));
		printf("params : \n");
		print_dmap(&ws, &(hr.path_params));
		printf("headers : \n");
		print_dmap(&ws, &(hr.headers));

		if(serialize_http_request_head(&ws, &hr))
		{
			printf("serliazation error\n");
			return 0;
		}

		deinit_http_request(&hr);
	}
	#elif defined TEST_RESPONSE
	{
		http_response hr;
		init_http_response(&hr);

		if(parse_http_response_head(&rs, &hr))
		{
			printf("parse error\n");
			return 0;
		}

		if(serialize_http_response_head(&ws, &hr))
		{
			printf("serliazation error\n");
			return 0;
		}

		deinit_http_response(&hr);
	}
	#endif

	deinitialize_stream(&rs);
	deinitialize_stream(&ws);

	return 0;
}