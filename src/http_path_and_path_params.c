#include<http_path_and_path_params.h>

#include<http_constant_dstrings.h>

#include<stream_util.h>

static char hex_to_char(char hex)
{
	hex = hex & 0x0f;
	if( 0 <= hex && hex <= 9 )
		return hex + '0';
	return (hex - 10) + 'a';
}

static int path_and_path_params_characters_allowed_on_stream_unencoded(char c)
{
	if( ('0'<=c && c<='9') || ('a'<=c && c<='z') || ('A'<=c && c<='Z') )
		return 1;
	switch(c)
	{
		case '-' :	case '_' :	case '.' :	case '~' :
		case '+' :	case '!' :	case '*' :
		case ',' :	case ':' :
			return 1;
		default :
			return 0;
	}
}

// if is_path is set then '/' is left unencoded
// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
static int to_serializable_format(const dstring* str, int is_path, dstring* res)
{
	const char* str_data = get_byte_array_dstring(str);
	cy_uint str_size = get_char_count_dstring(str);

	make_dstring_empty(res);

	for(cy_uint i = 0; i < str_size; i++)
	{
		if(path_and_path_params_characters_allowed_on_stream_unencoded(str_data[i]) || (is_path && str_data[i] == '/'))
		{
			if(!concatenate_char(res, str_data[i]))
				return HTTP_ALLOCATION_ERROR;
		}
		else
		{
			if(!snprintf_dstring(res, "%%%c%c", hex_to_char((str_data[i] >> 4) & 0x0f), hex_to_char(str_data[i] & 0x0f)))
				return HTTP_ALLOCATION_ERROR;
		}
	}

	return HTTP_NO_ERROR;
}

// returns int value, suggesting error, from error codes given in http_parser_error_codes.h
int uri_to_dstring_format(const dstring* str, dstring* res)
{
	const char* str_data = get_byte_array_dstring(str);
	cy_uint str_size = get_char_count_dstring(str);

	make_dstring_empty(res);

	for(cy_uint i = 0; i < str_size;)
	{
		if(str_data[i] == '%' && (str_size - i) >= 3)
		{
			i++;
			unsigned int digits[2];
			digits[0] = get_digit_from_char(str_data[i++], 16);
			digits[1] = get_digit_from_char(str_data[i++], 16);
			if(digits[0] == INVALID_INDEX || digits[1] == INVALID_INDEX)
				return HTTP_OBJECT_INVALID_ERROR;
			if(!concatenate_char(res, (((digits[0] << 4) & 0xf0) | (digits[1] & 0x0f))))
				return HTTP_ALLOCATION_ERROR;
		}
		else
		{
			if(!concatenate_char(res, str_data[i++]))
				return HTTP_ALLOCATION_ERROR;
		}
	}

	return HTTP_NO_ERROR;
}

static int is_end_char_for_param_key(int is_end_of_stream, char c, const void* cntxt)
{
	if(c == '%')
		return 0;
	return c == '=' || !path_and_path_params_characters_allowed_on_stream_unencoded(c);
}

static int is_end_char_for_param_value(int is_end_of_stream, char c, const void* cntxt)
{
	if(c == '%')
		return 0;
	return is_end_of_stream || c == '&' || !path_and_path_params_characters_allowed_on_stream_unencoded(c);
}

int parse_url_encoded_param(stream* rs, dstring* key, dstring* value, int is_first_param)
{
	int stream_error = 0;

	make_dstring_empty(key);
	make_dstring_empty(value);

	if(!is_first_param)
	{
		size_t bytes_skipped = skip_dstring_from_stream(rs, &AMP, &stream_error);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;
		if(bytes_skipped == 0) // a not first param must start with an ampersand, an absence of that implies end of params
			return HTTP_OBJECT_INVALID_ERROR;
	}

	int last_byte;

	dstring key_encoded = read_until_any_end_chars_from_stream(rs, is_end_char_for_param_key, NULL, &last_byte, 2048, &stream_error);
	if(stream_error)
	{
		deinit_dstring(&key_encoded);
		return HTTP_ERROR_IN_STREAM;
	}
	if(is_empty_dstring(&key_encoded))
	{
		deinit_dstring(&key_encoded);
		return HTTP_PARSER_ERROR;
	}

	if(get_char_count_dstring(&key_encoded) == 1)
	{
		deinit_dstring(&key_encoded);
		if(is_first_param)
			return HTTP_OBJECT_INVALID_ERROR;
		return HTTP_PARSER_ERROR;
	}
	else if(((char)last_byte) != '=')
	{
		deinit_dstring(&key_encoded);
		return HTTP_PARSER_ERROR;
	}

	discard_chars_from_back_dstring(&key_encoded, 1);

	dstring value_encoded = read_until_any_end_chars_from_stream(rs, is_end_char_for_param_value, NULL, &last_byte, 2048, &stream_error);
	if(stream_error)
	{
		deinit_dstring(&key_encoded);
		deinit_dstring(&value_encoded);
		return HTTP_ERROR_IN_STREAM;
	}
	if(is_empty_dstring(&value_encoded) && last_byte != 256) // a last_byte of 256 implies that the last byte read was end of stream
	{
		deinit_dstring(&key_encoded);
		deinit_dstring(&value_encoded);
		return HTTP_PARSER_ERROR;
	}

	// a last_byte of 256 implies that the last byte read was end of stream
	// the below condition is true if a value_encoded is successfully read and the last byte read was not an EOF, so in this case we need to discard this last byte
	if(!is_empty_dstring(&value_encoded) && last_byte != 256)
	{
		char lb = last_byte;
		unread_from_stream(rs, &lb, 1, &stream_error);
		if(stream_error)
		{
			deinit_dstring(&key_encoded);
			deinit_dstring(&value_encoded);
			return HTTP_ERROR_IN_STREAM;
		}
		discard_chars_from_back_dstring(&value_encoded, 1);
	}

	// clubbed conditionals with function calls
	// FAIL if (current capacity of dstring is less AND dstring could not be expanded)
	if(get_unused_capacity_dstring(key) < get_char_count_dstring(&key_encoded) &&
		!expand_dstring(key, get_char_count_dstring(&key_encoded) - get_unused_capacity_dstring(key)))
	{
		deinit_dstring(&key_encoded);
		deinit_dstring(&value_encoded);
		return HTTP_ALLOCATION_ERROR;
	}

	// FAIL if (uri format to dstring format conversion fails)
	int error = uri_to_dstring_format(&key_encoded, key);
	if(error)
	{
		deinit_dstring(&key_encoded);
		deinit_dstring(&value_encoded);
		return error;
	}

	// clubbed conditionals with function calls just like above
	if(get_unused_capacity_dstring(value) < get_char_count_dstring(&value_encoded) &&
		!expand_dstring(value, get_char_count_dstring(&value_encoded) - get_unused_capacity_dstring(value)))
	{
		deinit_dstring(&key_encoded);
		deinit_dstring(&value_encoded);
		return HTTP_ALLOCATION_ERROR;
	}

	error = uri_to_dstring_format(&value_encoded, value);
	if(error)
	{
		deinit_dstring(&key_encoded);
		deinit_dstring(&value_encoded);
		return error;
	}

	return HTTP_NO_ERROR;
}

int parse_url_encoded_params(stream* rs, dmap* params)
{
	int error = 0;

	int is_first_param = 1;

	while(1)
	{
		dstring key;
		dstring value;

		init_empty_dstring(&key, 0);
		init_empty_dstring(&value, 0);

		error = parse_url_encoded_param(rs, &key, &value, is_first_param);
		if(error)
		{
			deinit_dstring(&key);
			deinit_dstring(&value);
			break;
		}

		if(!insert_in_dmap(params, &key, &value))
		{
			deinit_dstring(&key);
			deinit_dstring(&value);
			return -1;
		}

		deinit_dstring(&key);
		deinit_dstring(&value);

		is_first_param = 0;
	}

	if(error == -1) // an error of -2 implies end of params
		return -1;

	return 0;
}

int parse_http_path_and_path_params(stream* rs, http_request_head* hr_p)
{
	dstring path_and_params;

	int error = 0;

	// read path and path params from the stream, until " " is encountered
	{
		path_and_params = read_until_dstring_from_stream(rs, &SP, SP_spml, 2048, &error);
		if(error || is_empty_dstring(&path_and_params))
		{
			deinit_dstring(&path_and_params);
			return -1;
		}

		// put SP (" ") back into the stream
		// and remove SP (" ") from path_and_params
		unread_dstring_from_stream(rs, &SP, &error);
		if(error)
		{
			deinit_dstring(&path_and_params);
			return -1;
		}
		discard_chars_from_back_dstring(&path_and_params, get_char_count_dstring(&SP));
	}

	// separate path and params
	dstring path;
	dstring params = split_dstring(&path_and_params, &QM, &path);

	// path must not be empty
	if(is_empty_dstring(&path))
	{
		deinit_dstring(&path_and_params);
		return -1;
	}

	// populate path
	make_dstring_empty(&(hr_p->path));
	if(!uri_to_dstring_format(&path, &(hr_p->path)))
	{
		deinit_dstring(&path_and_params);
		return -1;
	}

	// if there are no params then exit with success
	if(is_empty_dstring(&params))
	{
		deinit_dstring(&path_and_params);
		return 0;
	}

	// insert params to the hr_p->path_params
	dstring delim_1 = get_dstring_pointing_to_literal_cstring("&");
	dstring delim_2 = get_dstring_pointing_to_literal_cstring("=");

	for_each_split_by_delim(param, &params, &delim_1)
	{
		dstring param_key;
		dstring param_value = split_dstring(&param, &delim_2, &param_key);

		// param key can not be empty
		if(is_empty_dstring(&param_key))
		{
			deinit_dstring(&path_and_params);
			return -1;
		}

		// insert param_key and param_value into path_params
		{
			dstring param_key_dstring;
			if(!init_empty_dstring(&param_key_dstring, get_char_count_dstring(&param_key)))
			{
				deinit_dstring(&path_and_params);
				return -1;
			}
			if(!uri_to_dstring_format(&param_key, &param_key_dstring))
			{
				deinit_dstring(&param_key_dstring);
				deinit_dstring(&path_and_params);
				return -1;
			}

			dstring param_value_dstring;
			if(!init_empty_dstring(&param_value_dstring, get_char_count_dstring(&param_value)))
			{
				deinit_dstring(&param_key_dstring);
				deinit_dstring(&path_and_params);
				return -1;
			}
			if(!uri_to_dstring_format(&param_value, &param_value_dstring))
			{
				deinit_dstring(&param_key_dstring);
				deinit_dstring(&param_value_dstring);
				deinit_dstring(&path_and_params);
				return -1;
			}

			if(!insert_in_dmap(&(hr_p->path_params), &param_key_dstring, &param_value_dstring))
			{
				deinit_dstring(&param_key_dstring);
				deinit_dstring(&param_value_dstring);
				deinit_dstring(&path_and_params);
				return -1;
			}

			deinit_dstring(&param_key_dstring);
			deinit_dstring(&param_value_dstring);
		}
	}

	deinit_dstring(&path_and_params);

	return 0;
}

int serialize_http_path_and_path_params(stream* ws, const http_request_head* hr_p)
{
	int stream_error = 0;

	{
		dstring path_serializable;
		if(!init_empty_dstring(&path_serializable, get_char_count_dstring(&(hr_p->path))))
			return HTTP_ALLOCATION_ERROR;
		int error = to_serializable_format(&(hr_p->path), 1, &path_serializable);
		if(error)
		{
			deinit_dstring(&path_serializable);
			return error;
		}
		write_dstring_to_stream(ws, &path_serializable, &stream_error);
		deinit_dstring(&path_serializable);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;
	}

	// if the hashmap is not empty, then write "?" to the stream and then serialize the path_params
	if(!is_empty_hashmap(&(hr_p->path_params)))
	{
		write_dstring_to_stream(ws, &QM, &stream_error);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;

		int error = serialize_url_encoded_params(ws, &(hr_p->path_params));
		if(error)
			return error;
	}

	return HTTP_NO_ERROR;
}

int serialize_url_encoded_params(stream* ws, const dmap* params)
{
	int is_first_param = 1;
	for_each_in_dmap(e, params)
	{
		int error = serialize_url_encoded_param(ws, &(e->key), &(e->value), is_first_param);
		if(error)
			return error;

		is_first_param = 0;
	}
	return HTTP_NO_ERROR;
}

int serialize_url_encoded_param(stream* ws, const dstring* key, const dstring* value, int is_first_param)
{
	int stream_error = 0;

	if(!is_first_param) // write '&' if this is not the first param
	{
		write_dstring_to_stream(ws, &AMP, &stream_error);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;
	}

	{
		dstring key_serializable;
		if(!init_empty_dstring(&key_serializable, get_char_count_dstring(key)))
			return HTTP_ALLOCATION_ERROR;
		int error = to_serializable_format(key, 0, &key_serializable);
		if(error)
		{
			deinit_dstring(&key_serializable);
			return error;
		}
		write_dstring_to_stream(ws, &key_serializable, &stream_error);
		deinit_dstring(&key_serializable);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;
	}

	write_dstring_to_stream(ws, &EQ, &stream_error); // '='
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;

	{
		dstring value_serializable;
		if(!init_empty_dstring(&value_serializable, get_char_count_dstring(value)))
			return HTTP_ALLOCATION_ERROR;
		int error = to_serializable_format(value, 0, &value_serializable);
		if(error)
		{
			deinit_dstring(&value_serializable);
			return error;
		}
		write_dstring_to_stream(ws, &value_serializable, &stream_error);
		deinit_dstring(&value_serializable);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;
	}

	return 0;
}