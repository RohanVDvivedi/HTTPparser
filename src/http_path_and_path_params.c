#include<http_path_and_path_params.h>

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
static dstring to_serializable_format(const dstring* str, int is_path)
{
	const char* str_data = get_byte_array_dstring(str);
	unsigned int str_size = get_char_count_dstring(str);

	dstring res;
	init_empty_dstring(&res, str_size);

	for(unsigned int i = 0; i < str_size; i++)
	{
		if(path_and_path_params_characters_allowed_on_stream_unencoded(str_data[i]) || (is_path && str_data[i] == '/'))
			concatenate_char(&res, str_data[i]);
		else
			snprintf_dstring(&res, "%%%c%c", hex_to_char((str_data[i] >> 4) & 0x0f), hex_to_char(str_data[i] & 0x0f));
	}

	return res;
}

// returns 1 for success and 0 for an error
static int to_dstring_format(const dstring* str, dstring* res)
{
	const char* str_data = get_byte_array_dstring(str);
	unsigned int str_size = get_char_count_dstring(str);

	make_dstring_empty(res);

	for(unsigned int i = 0; i < str_size;)
	{
		if(str_data[i] == '%' && (str_size - i) >= 3)
		{
			i++;
			unsigned int digits[2];
			digits[0] = get_digit_from_char(str_data[i++], 16);
			digits[1] = get_digit_from_char(str_data[i++], 16);
			if(digits[0] == INVALID_INDEX || digits[1] == INVALID_INDEX)
			{
				make_dstring_empty(res);
				return 0;
			}
			concatenate_char(res, (((digits[0] << 4) & 0xf0) | (digits[1] & 0x0f)));
		}
		else
			concatenate_char(res, str_data[i++]);
	}

	return 1;
}

int parse_http_path_and_path_params(stream* rs, http_request* hr_p)
{
	dstring path_and_params;

	int error = 0;

	// read path and path params from the stream
	{
		dstring until_str = get_dstring_pointing_to_literal_cstring(" ");
		unsigned int psm_us[2];
		get_prefix_suffix_match_lengths(&until_str, psm_us);
		path_and_params = read_dstring_until_from_stream(rs, &until_str, psm_us, 2048, &error);
		if(error || is_empty_dstring(&path_and_params))
		{
			deinit_dstring(&path_and_params);
			return -1;
		}

		// put until_str back into the stream
		// and remove until_str from path_and_params
		unread_from_stream(rs, get_byte_array_dstring(&until_str), get_char_count_dstring(&until_str));
		discard_chars_dstring(&path_and_params, get_char_count_dstring(&path_and_params) - get_char_count_dstring(&until_str), get_char_count_dstring(&path_and_params) - 1);
	}

	// separate path and params
	dstring path;
	dstring params = split_dstring(&path_and_params, &get_dstring_pointing_to_literal_cstring("?"), &path);

	// path must not be empty
	if(is_empty_dstring(&path))
	{
		deinit_dstring(&path_and_params);
		return -1;
	}

	// populate path
	make_dstring_empty(&(hr_p->path));
	if(!to_dstring_format(&path, &(hr_p->path)))
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

	dstring remaining = params;
	while(!is_empty_dstring(&remaining))
	{
		dstring param;
		remaining = split_dstring(&remaining, &delim_1, &param);

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
			init_empty_dstring(&param_key_dstring, get_char_count_dstring(&param_key));
			if(!to_dstring_format(&param_key, &param_key_dstring))
			{
				deinit_dstring(&param_key_dstring);
				deinit_dstring(&path_and_params);
				return -1;
			}

			dstring param_value_dstring;
			init_empty_dstring(&param_value_dstring, get_char_count_dstring(&param_value));
			if(!to_dstring_format(&param_value, &param_value_dstring))
			{
				deinit_dstring(&param_key_dstring);
				deinit_dstring(&param_value_dstring);
				deinit_dstring(&path_and_params);
				return -1;
			}

			insert_in_dmap(&(hr_p->path_params), &param_key_dstring, &param_value_dstring);

			deinit_dstring(&param_key_dstring);
			deinit_dstring(&param_value_dstring);
		}
	}

	deinit_dstring(&path_and_params);

	return 0;
}

int serialize_http_path_and_path_params(stream* ws, const http_request* hr_p)
{
	int error = 0;

	{
		dstring path_serializable = to_serializable_format(&(hr_p->path), 1);
		write_to_stream(ws, get_byte_array_dstring(&path_serializable), get_char_count_dstring(&path_serializable), &error);
		deinit_dstring(&path_serializable);
		if(error)
			return -1;
	}

	char between_params = '?';
	static const char after_key = '=';

	for(const dmap_entry* e = get_first_of_in_hashmap(&(hr_p->path_params), FIRST_OF_HASHMAP); e != NULL; e = get_next_of_in_hashmap(&(hr_p->path_params), e, ANY_IN_HASHMAP))
	{
		write_to_stream(ws, &between_params, 1, &error);
		if(error)
			return -1;

		{
			dstring key_serializable = to_serializable_format(&(e->key), 0);
			write_to_stream(ws, get_byte_array_dstring(&key_serializable), get_char_count_dstring(&key_serializable), &error);
			deinit_dstring(&key_serializable);
			if(error)
				return -1;
		}

		write_to_stream(ws, &after_key, 1, &error);
		if(error)
			return -1;

		{
			dstring value_serializable = to_serializable_format(&(e->value), 0);
			write_to_stream(ws, get_byte_array_dstring(&value_serializable), get_char_count_dstring(&value_serializable), &error);
			deinit_dstring(&value_serializable);
			if(error)
				return -1;
		}

		between_params = '&';
	}

	return 0;
}