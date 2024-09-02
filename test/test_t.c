#include<stdio.h>
#include<stdlib.h>

#include<http_path_and_path_params.h>

#include<stream_for_file_descriptor.h>

int main()
{
	stream rs, ws;
	initialize_stream_for_fd(&rs, 0);
	initialize_stream_for_fd(&ws, 1);

	dmap params;
	init_dmap(&params, 0);

	int error = parse_url_encoded_params(&rs, &params);

	if(error)
	{
		printf("error parsing params\n");
		goto EXIT;
	}

	printf("\n");

	print_dmap(&ws, &params);

	flush_all_from_stream(&ws, &error);

	printf("\n");

	error = serialize_url_encoded_params(&ws, &params);

	if(error)
	{
		printf("error serializing params\n");
		goto EXIT;
	}

	flush_all_from_stream(&ws, &error);

	if(error)
	{
		printf("error flushing serilized params\n");
		goto EXIT;
	}


	EXIT:;
	deinitialize_stream(&rs);
	deinitialize_stream(&ws);
	return 0;
}