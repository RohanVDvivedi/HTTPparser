#include<stdio.h>
#include<stdlib.h>

#include<http_request.h>
#include<http_response.h>
#include<http_header_util.h>

#include<comm_address.h>
#include<client.h>
#include<ssl_ctx_helper.h>
#include<file_descriptor_stream.h>

int main()
{
	stream rs, ws;
	initialize_stream_for_fd(&rs, 0);
	initialize_stream_for_fd(&ws, 1);

	http_request_head hrq;
	if(!init_http_request_head_from_uri(&hrq, &get_dstring_pointing_to_literal_cstring("https://www.bing.com/?toWww=1&redig=C6A5D09A029446FAA02284B3A374D5F3")))
	{
		printf("failed to initialize http request from given uri\n");
		goto EXIT_0;
	}
	hrq.method = GET;
	if(!insert_literal_cstrings_in_dmap(&(hrq.headers), "accept", "*/*"))
	{
		printf("failed to insert accept header\n");
		goto EXIT_1;
	}
	if(!insert_literal_cstrings_in_dmap(&(hrq.headers), "accept-encoding", "gzip,deflate"))
	{
		printf("failed to insert accept-encoding header\n");
		goto EXIT_1;
	}

	http_response_head hrp;
	init_http_response_head(&hrp);

	comm_address server_address;
	int res = lookup_by_name("bing.com", "443", SOCK_STREAM, AF_INET, &server_address, 1);
	if(res == 0)
	{
		printf("no servers found\n");
		goto EXIT_2;
	}

	stream raw_stream;
	ssl_lib_init();
	SSL_CTX* ssl_ctx = get_ssl_ctx_for_client(NULL, NULL);
	if(-1 == make_connection_stream(&raw_stream, &server_address, "bing.com", NULL, ssl_ctx))
	{
		printf("failed to make connection");
		goto EXIT_3;
	}

	int error = 0;

	if(serialize_http_request_head(&raw_stream, &hrq) == -1)
	{
		printf("error serializing http request head\n");
		goto EXIT_4;
	}
	flush_all_from_stream(&raw_stream, &error);
	if(error)
	{
		printf("%d error flushing request head\n", error);
		goto EXIT_4;
	}
	if(parse_http_response_head(&raw_stream, &hrp) == -1)
	{
		printf("error parsing http response head\n");
		goto EXIT_4;
	}

	// printing response head
	print_http_response_head(&hrp);

	stacked_stream sstrm;
	initialize_stacked_stream(&sstrm);

	if(0 > intialize_http_body_and_decoding_streams_for_reading(&sstrm, &raw_stream, &(hrp.headers)))
	{
		printf("error initializing one of body or decoding streams\n");
		goto EXIT_4;
	}

	#define read_buffer_size 64
	char read_buffer[read_buffer_size];
	while(1)
	{
		unsigned int bytes_read = read_from_stacked_stream(&sstrm, read_buffer, read_buffer_size, &error);
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
			printf("error writing to STDOUT\n");
			break;
		}
		flush_all_from_stream(&ws, &error);
		if(error)
		{
			printf("flushing STDOUT threw error\n");
			break;
		}
	}

	EXIT_5:;
	close_deinitialize_free_all_from_stacked_stream(&sstrm, READ_STREAMS);
	deinitialize_stacked_stream(&sstrm);

	EXIT_4:;
	close_stream(&raw_stream, &error);
	deinitialize_stream(&raw_stream);

	EXIT_3:;
	if(ssl_ctx != NULL)
		destroy_ssl_ctx(ssl_ctx);

	EXIT_2:;
	deinit_http_response_head(&hrp);

	EXIT_1:;
	deinit_http_request_head(&hrq);

	EXIT_0:;
	deinitialize_stream(&rs);
	deinitialize_stream(&ws);

	return 0;
}