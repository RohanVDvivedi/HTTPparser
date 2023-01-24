#include<stdio.h>
#include<stdlib.h>

#include<http_request.h>
#include<http_response.h>
#include<http_body_stream.h>

#include<comm_address.h>
#include<client.h>
#include<ssl_ctx_helper.h>

int main()
{
	http_request hrq;
	init_http_request(&hrq);
	hrq.method = GET;
	concatenate_dstring(&(hrq.path), &get_literal_cstring("/api/v2/entries/en/hello"));
	hrq.version.major = 1;
	hrq.version.minor = 1;
	insert_in_dmap(&(hrq.headers), &(get_literal_cstring("host")), &(get_literal_cstring("api.dictionaryapi.dev")));
	insert_in_dmap(&(hrq.headers), &(get_literal_cstring("accept")), &(get_literal_cstring("*/*")));

	http_response hrp;
	init_http_response(&hrp);

	comm_address server_address;
	int res = lookup_by_name("api.dictionaryapi.dev", /*"443"*/ "80", SOCK_STREAM, AF_INET, &server_address, 1);
	if(res == 0)
	{
		printf("no servers found\n");
		goto EXIT_1;
	}

	stream raw_stream;
	//ssl_lib_init();
	SSL_CTX* ssl_ctx = NULL;//get_ssl_ctx_for_client(NULL, NULL);
	if(!make_connection_stream(&raw_stream, &server_address, NULL, ssl_ctx))
	{
		printf("failed to make connection");
		goto EXIT_2;
	}

	int error = 0;

	close_stream(&raw_stream, &error);
	deinitialize_stream(&raw_stream);

	EXIT_2:;
	if(ssl_ctx != NULL)
		destroy_ssl_ctx(ssl_ctx);

	EXIT_1:;
	deinit_http_request(&hrq);
	deinit_http_response(&hrp);

	return 0;
}