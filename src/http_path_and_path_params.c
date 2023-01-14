#include<http_path_and_path_params.h>

#include<stream_util.h>

static char char_to_hex(char c)
{
	if( '0' <= c && c <= '9' )
		return c - '0';
	else if('a' <= c && c <= 'f')
		return c - 'a' + 10;
	else if('A' <= c && c <= 'F')
		return c - 'A' + 10;
	else
		return 'N';
}

static char hex_to_char(char hex)
{
	hex = hex & 0x0f;
	if( 0 <= hex && hex <= 9 )
		return hex + '0';
	return (hex - 10) + 'a';
}

static int path_and_path_params_characters_allowed_on_stream(char c)
{
	if( ('0'<=c && c<='9') || ('a'<=c && c<='z') || ('A'<=c && c<='Z') )
		return 1;
	switch(c)
	{
		case '$' :	case '-' :	case '_' :	case '.' :
		case '+' :	case '!' :	case '*' :	case '\'' :
		case '(' :	case ')' :	case ',' :
			return 1;
		default :
			return 0;
	}
}

dstring to_serializable_format(const dstring* str)
{
	const char* str_data = get_byte_array_dstring(str);
	unsigned int str_size = get_char_count_dstring(str);

	dstring res;
	init_empty_dstring(&res, str_size);

	for(unsigned int i = 0; i < str_size; i++)
	{
		if(path_and_path_params_characters_allowed_on_stream(str_data[i]))
			concatenate_char(&res, str_data[i]);
		else
			snprintf_dstring(&res, "%%%c%c", hex_to_char((str_data[i] >> 4) & 0x0f), hex_to_char(str_data[i] & 0x0f));
	}

	return res;
}

static dstring to_dstring_format(const dstring* str)
{
	const char* str_data = get_byte_array_dstring(str);
	unsigned int str_size = get_char_count_dstring(str);

	dstring res;
	init_empty_dstring(&res, str_size);

	for(unsigned int i = 0; i < str_size;)
	{
		if(str_data[i] == '%' && (str_size - i) >= 3)
		{
			i++;
			char c = (char_to_hex(str_data[i++]) << 4) & 0xf0;
			c = c | (char_to_hex(str_data[i++]) & 0x0f);
			concatenate_char(&res, c);
		}
		else
			concatenate_char(&res, str_data[i++]);
	}

	return res;
}

int parse_http_path_and_path_params(stream* rs, http_request* hr_p)
{
	dstring path_and_params;

	int error = 0;

	// read path and path params from the stream
	{
		dstring until_str = get_literal_cstring(" ");
		unsigned int psm_us[2];
		get_prefix_suffix_match_lengths(&until_str, psm_us);
		path_and_params = read_dstring_until_from_stream(rs, &until_str, psm_us, 2048, &error);
		if(error || is_empty_dstring(&path_and_params))
		{
			deinit_dstring(&path_and_params);
			return -1;
		}

		// put untl_str back into the stream
		// and remove until_str from path_and_params
		if(is_suffix_of_dstring(&path_and_params, &until_str))
		{
			unread_from_stream(rs, get_byte_array_dstring(&until_str), get_char_count_dstring(&until_str));
			discard_chars_dstring(&path_and_params, get_char_count_dstring(&path_and_params) - get_char_count_dstring(&until_str), get_char_count_dstring(&path_and_params) - 1);
		}
	}

	// separate path and params
	dstring path;
	dstring params = split_dstring(&path_and_params, &get_literal_cstring("?"), &path);

	// path must not be empty
	if(is_empty_dstring(&path))
	{
		deinit_dstring(&path_and_params);
		deinit_dstring(&path);
		deinit_dstring(&params);
		return -1;
	}

	// populate path
	{
		make_dstring_empty(&(hr_p->path));
		dstring path_dstring = to_dstring_format(&path);
		concatenate_dstring(&(hr_p->path), &path_dstring);
		deinit_dstring(&path_dstring);
	}

	deinit_dstring(&path);

	// insert params to the hr_p->path_params
	if(!is_empty_dstring(&params))
	{
		dstring delim_1 = get_literal_cstring("&");
		dstring delim_2 = get_literal_cstring("=");

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
				deinit_dstring(&param_key);
				deinit_dstring(&param_value);
				deinit_dstring(&params);
				deinit_dstring(&path_and_params);
				return -1;
			}

			// insert param_key and param_value into path_params
			{
				dstring param_key_dstring = to_dstring_format(&param_key);
				dstring param_value_dstring = to_dstring_format(&param_value);
				insert_in_dmap(&(hr_p->path_params), &param_key_dstring, &param_value_dstring);
				deinit_dstring(&param_key_dstring);
				deinit_dstring(&param_value_dstring);
			}

			deinit_dstring(&param_key);
			deinit_dstring(&param_value);
		}
	}

	deinit_dstring(&params);

	deinit_dstring(&path_and_params);

	return 0;
}

int serialize_http_path_and_path_params(stream* ws, const http_request* hr_p)
{
	int error = 0;

	write_to_stream(ws, get_byte_array_dstring(&(hr_p->path)), get_char_count_dstring(&(hr_p->path)), &error);
	if(error)
		return -1;

	char between_params = '?';
	static const char after_key = '=';

	for(const dmap_entry* e = get_first_of_in_hashmap(&(hr_p->path_params), FIRST_OF_HASHMAP); e != NULL; e = get_next_of_in_hashmap(&(hr_p->path_params), e, ANY_IN_HASHMAP))
	{
		write_to_stream(ws, &between_params, 1, &error);
		if(error)
			return -1;

		write_to_stream(ws, get_byte_array_dstring(&(e->key)), get_char_count_dstring(&(e->key)), &error);
		if(error)
			return -1;

		write_to_stream(ws, &after_key, 1, &error);
		if(error)
			return -1;

		write_to_stream(ws, get_byte_array_dstring(&(e->value)), get_char_count_dstring(&(e->value)), &error);
		if(error)
			return -1;

		between_params = '&';
	}

	return 0;
}