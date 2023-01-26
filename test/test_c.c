#include<stdio.h>
#include<stdlib.h>

#include<http_request.h>
#include<http_response.h>
#include<http_body_stream.h>

#include<comm_address.h>
#include<client.h>
#include<ssl_ctx_helper.h>
#include<file_descriptor_stream.h>

int main()
{
	stream rs, ws;
	initialize_stream_for_fd(&rs, 0);
	initialize_stream_for_fd(&ws, 1);

	http_request hrq;
	init_http_request(&hrq);
	hrq.method = GET;
	concatenate_dstring(&(hrq.path), &get_dstring_pointing_to_literal_cstring("/?toWww=1&redig=C6A5D09A029446FAA02284B3A374D5F3"));
	hrq.version.major = 1;
	hrq.version.minor = 1;
	insert_in_dmap(&(hrq.headers), &(get_dstring_pointing_to_literal_cstring("host")), &(get_dstring_pointing_to_literal_cstring("www.bing.com")));
	insert_in_dmap(&(hrq.headers), &(get_dstring_pointing_to_literal_cstring("accept")), &(get_dstring_pointing_to_literal_cstring("*/*")));

	http_response hrp;
	init_http_response(&hrp);

	comm_address server_address;
	int res = lookup_by_name("bing.com", "443", SOCK_STREAM, AF_INET, &server_address, 1);
	if(res == 0)
	{
		printf("no servers found\n");
		goto EXIT_1;
	}

	stream raw_stream;
	ssl_lib_init();
	SSL_CTX* ssl_ctx = get_ssl_ctx_for_client(NULL, NULL);
	if(!make_connection_stream(&raw_stream, &server_address, NULL, ssl_ctx))
	{
		printf("failed to make connection");
		goto EXIT_2;
	}

	int error = 0;

	if(serialize_http_request_head(&raw_stream, &hrq) == -1)
	{
		printf("error serializing http request head\n");
		goto EXIT_3;
	}
	if(parse_http_response_head(&raw_stream, &hrp) == -1)
	{
		printf("error parsing http response head\n");
		goto EXIT_3;
	}

	// printing response head
	{
		printf("version = %d.%d\n", hrp.version.major, hrp.version.minor);
		printf("status = %d\n", hrp.status);
		print_dmap(&ws, &(hrp.headers));
		printf("\n");
	}

	stream body_stream;
	if(!initialize_readable_body_stream(&body_stream, &raw_stream, &(hrp.headers)))
	{
		printf("body stream could not be initialized\n");
		goto EXIT_3;
	}

	#define read_buffer_size 64
	char read_buffer[read_buffer_size];
	while(1)
	{
		unsigned int bytes_read = read_from_stream(&body_stream, read_buffer, read_buffer_size, &error);
		if(error)
		{
			printf("body stream read error\n");
			break;
		}
		if(bytes_read == 0)
		{
			printf("\n\nbody complete\n");
			break;
		}

		write_to_stream(&ws, read_buffer, bytes_read, &error);
		if(error)
		{
			printf("STDOUT threw error\n");
			break;
		}
	}

	close_stream(&body_stream, &error);
	deinitialize_stream(&body_stream);

	EXIT_3:;
	close_stream(&raw_stream, &error);
	deinitialize_stream(&raw_stream);

	EXIT_2:;
	if(ssl_ctx != NULL)
		destroy_ssl_ctx(ssl_ctx);

	EXIT_1:;
	deinit_http_request(&hrq);
	deinit_http_response(&hrp);

	deinitialize_stream(&rs);
	deinitialize_stream(&ws);

	return 0;
}