#include<dmap.h>

#include<stdlib.h>

static int init_dmap_entry(dmap_entry* dmap_entry_p, const dstring* key, const dstring* value)
{
	if(!init_copy_dstring(&(dmap_entry_p->key), key))
		return 0;
	if(!init_copy_dstring(&(dmap_entry_p->value), value))
	{
		deinit_dstring(&(dmap_entry_p->key));
		return 0;
	}
	initialize_rbhnode(&(dmap_entry_p->embed_node));
	return 1;
}

static void deinit_dmap_entry(dmap_entry* dmap_entry_p)
{
	deinit_dstring(&(dmap_entry_p->key));
	deinit_dstring(&(dmap_entry_p->value));
}

static cy_uint dstring_key_hash_case(const void* element)
{
	const dmap_entry* ent = element;
	const char* key_data = get_byte_array_dstring(&(ent->key));
	cy_uint key_size = get_char_count_dstring(&(ent->key));
	cy_uint hash = 0;
	for(cy_uint i = 0; i < key_size; i++)
		hash += ((((cy_uint)(to_lowercase_char(key_data[i]))) * i) ^ 0xf0f0f0f0);
	return hash;
}

static cy_uint dstring_key_hash(const void* element)
{
	const dmap_entry* ent = element;
	const char* key_data = get_byte_array_dstring(&(ent->key));
	cy_uint key_size = get_char_count_dstring(&(ent->key));
	cy_uint hash = 0;
	for(cy_uint i = 0; i < key_size; i++)
		hash += ((((cy_uint)(key_data[i])) * i) ^ 0xf0f0f0f0);
	return hash;
}

int init_dmap(dmap* dmap_p, int ignore_case_for_key)
{
	return initialize_hashmap(
								dmap_p,
								ROBINHOOD_HASHING,
								8,
								&simple_hasher(ignore_case_for_key ? dstring_key_hash_case : dstring_key_hash),
								&simple_comparator(((int (*)(const void*, const void*))(ignore_case_for_key ? case_compare_dstring : compare_dstring))),
								offsetof(dmap_entry, embed_node)
							);
}

dmap_entry* get_from_dmap(const dmap* dmap_p, const dstring* key)
{
	return (dmap_entry*) find_equals_in_hashmap(dmap_p, &((dmap_entry){.key = (*key)}));
}

dmap_entry* insert_in_dmap(dmap* dmap_p, const dstring* key, const dstring* value)
{
	dmap_entry* dmap_entry_p = malloc(sizeof(dmap_entry));
	if(dmap_entry_p == NULL) // fail if malloc fails
		return NULL;

	if(!init_dmap_entry(dmap_entry_p, key, value)) // fail if we fail to initialize the dmap_entry
	{
		free(dmap_entry_p);
		return NULL;
	}

	// insert the new dmap_entry in hashmap
	// if it fails then expand the hashmap and retry insertion
	if(!insert_in_hashmap(dmap_p, dmap_entry_p))
	{
		expand_hashmap(dmap_p, 2.0);
		if(!insert_in_hashmap(dmap_p, dmap_entry_p)) // if the retry also fails, then fail the insert_in_dmap
		{
			deinit_dmap_entry(dmap_entry_p);
			free(dmap_entry_p);
			return NULL;
		}
	}

	return dmap_entry_p;
}

dmap_entry* insert_formatted_in_dmap(dmap* dmap_p, const dstring* key, const char* value_format, ...)
{
	// create a dmap_entry indentically to the above function, but with an empty value
	dmap_entry* dmap_entry_p = malloc(sizeof(dmap_entry));
	if(dmap_entry_p == NULL) // fail if malloc fails
		return NULL;

	if(!init_dmap_entry(dmap_entry_p, key, &get_dstring_pointing_to_literal_cstring("")))  // fail if we fail to initialize the dmap_entry
	{
		free(dmap_entry_p);
		return NULL;
	}

	va_list var_args;
	va_start(var_args, value_format);
	if(!vsnprintf_dstring(&(dmap_entry_p->value), value_format, var_args))  // fail if we fail to write the formatted value to dmap's value
	{
		va_end(var_args);
		deinit_dmap_entry(dmap_entry_p);
		free(dmap_entry_p);
		return NULL;
	}
	va_end(var_args);

	// insert the new dmap_entry in hashmap
	// if it fails then expand the hashmap and retry insertion
	if(!insert_in_hashmap(dmap_p, dmap_entry_p))
	{
		expand_hashmap(dmap_p, 2.0);
		if(!insert_in_hashmap(dmap_p, dmap_entry_p)) // if the retry also fails, then fail the insert_in_dmap
		{
			deinit_dmap_entry(dmap_entry_p);
			free(dmap_entry_p);
			return NULL;
		}
	}

	return dmap_entry_p;
}

int delete_from_dmap_key(dmap* dmap_p, dmap_entry* dmap_entry_p)
{
	if(remove_from_hashmap(dmap_p, dmap_entry_p))
	{
		deinit_dmap_entry(dmap_entry_p);
		free(dmap_entry_p);
		return 1;
	}
	return 0;
}

#include<stream_util.h>

void print_dmap(stream* ws, const dmap* dmap_p)
{
	int error = 0;
	for_each_in_dmap(e, dmap_p)
	{
		write_to_stream_formatted(ws, &error, "<" printf_dstring_format "> -> <" printf_dstring_format ">\n", printf_dstring_params(&(e->key)), printf_dstring_params(&(e->value)));
		if(error)
			break;
	}
	flush_all_from_stream(ws, &error);
}

static void on_remove_all_from_dmap_delete_entry(void* resource, const void* _de)
{
	dmap_entry* e = (dmap_entry*) _de;
	deinit_dmap_entry(e);
	free(e);
}

void deinit_dmap(dmap* dmap_p)
{
	remove_all_from_hashmap(dmap_p, &((notifier_interface){NULL, on_remove_all_from_dmap_delete_entry}));
	deinitialize_hashmap(dmap_p);
}