#include<stdio.h>
#include<stdlib.h>

#include<httpparser/http_request.h>
#include<httpparser/http_response.h>
#include<httpparser/http_header_util.h>

#include<connman/comm_address.h>
#include<connman/client.h>
#include<connman/ssl_ctx_helper.h>
#include<cutlery/stream_for_file_descriptor.h>

#include<cutlery/deferred_callbacks.h>

int main()
{
	NEW_DEFERRED_CALLS(16);

	stream rs, ws;
	initialize_stream_for_fd(&rs, 0);DEFER(deinitialize_stream, &rs);
	initialize_stream_for_fd(&ws, 1);DEFER(deinitialize_stream, &ws);

	http_request_head hrq;
	if(!init_http_request_head_from_uri(&hrq, &get_dstring_pointing_to_literal_cstring("https://www.bing.com/?toWww=1&redig=C6A5D09A029446FAA02284B3A374D5F3")))
	{
		printf("failed to initialize http request from given uri\n");
		goto EXIT;
	}
	DEFER(deinit_http_request_head, &hrq);
	hrq.method = GET;
	if(!insert_literal_cstrings_in_dmap(&(hrq.headers), "accept", "*/*"))
	{
		printf("failed to insert accept header\n");
		goto EXIT;
	}
	if(!insert_literal_cstrings_in_dmap(&(hrq.headers), "accept-encoding", "gzip,deflate"))
	{
		printf("failed to insert accept-encoding header\n");
		goto EXIT;
	}

	http_response_head hrp;
	init_http_response_head(&hrp);
	DEFER(deinit_http_response_head, &hrp);

	comm_address server_address;
	int res = lookup_by_name("bing.com", "443", SOCK_STREAM, AF_INET, &server_address, 1);
	if(res == 0)
	{
		printf("no servers found\n");
		goto EXIT;
	}

	stream raw_stream;
	ssl_lib_init();
	SSL_CTX* ssl_ctx = get_ssl_ctx_for_client(NULL, NULL);
	if(ssl_ctx)
		DEFER(destroy_ssl_ctx, ssl_ctx);
	if(-1 == make_connection_stream(&raw_stream, &server_address, "bing.com", NULL, ssl_ctx))
	{
		printf("failed to make connection");
		goto EXIT;
	}
	DEFER(deinitialize_stream, &raw_stream);
	DEFER(close_stream_error_disregard, &raw_stream);

	int error = 0;

	if(serialize_http_request_head(&raw_stream, &hrq) == -1)
	{
		printf("error serializing http request head\n");
		goto EXIT;
	}
	flush_all_from_stream(&raw_stream, &error);
	if(error)
	{
		printf("%d error flushing request head\n", error);
		goto EXIT;
	}
	if(parse_http_response_head(&raw_stream, &hrp) == -1)
	{
		printf("error parsing http response head\n");
		goto EXIT;
	}

	// printing response head
	print_http_response_head(&hrp);

	stacked_stream sstrm;
	initialize_stacked_stream(&sstrm);
	DEFER(deinitialize_stacked_stream, &sstrm);

	if(0 > intialize_http_body_and_decoding_streams_for_reading(&sstrm, &raw_stream, &(hrp.headers)))
	{
		printf("error initializing one of body or decoding streams\n");
		goto EXIT;
	}
	DEFER(close_deinitialize_free_all_from_READER_stacked_stream, &sstrm);

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

	EXIT:;
	CALL_ALL_DEFERRED();
	return 0;
}