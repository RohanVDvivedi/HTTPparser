#include<dmap.h>

#include<stdlib.h>
#include<ctype.h>

void init_dmap_entry(dmap_entry* dmap_entry_p, const dstring* key, const dstring* value)
{
	init_copy_dstring(&(dmap_entry_p->key), key);
	init_copy_dstring(&(dmap_entry_p->value), value);
	initialize_rbhnode(&(dmap_entry_p->embed_node));
}

void deinit_dmap_entry(dmap_entry* dmap_entry_p)
{
	deinit_dstring(&(dmap_entry_p->key));
	deinit_dstring(&(dmap_entry_p->value));
}

static unsigned int dstring_key_hash_case(const void* element)
{
	const dmap_entry* ent = element;
	const char* key_data = get_byte_array_dstring(&(ent->key));
	unsigned int key_size = get_char_count_dstring(&(ent->key));
	unsigned int hash = 0;
	for(unsigned int i = 0; i < key_size; i++)
		hash += (((((unsigned int)(tolower(key_data[i]))) * i) ^ 0xf0) & 0xffffff);
	return hash;
}

static unsigned int dstring_key_hash(const void* element)
{
	const dmap_entry* ent = element;
	const char* key_data = get_byte_array_dstring(&(ent->key));
	unsigned int key_size = get_char_count_dstring(&(ent->key));
	unsigned int hash = 0;
	for(unsigned int i = 0; i < key_size; i++)
		hash += (((((unsigned int)(key_data[i])) * i) ^ 0xf0) & 0xffffff);
	return hash;
}

void init_dmap(dmap* dmap_p, int ignore_case_for_key)
{
	if(ignore_case_for_key)
		initialize_hashmap(dmap_p, ROBINHOOD_HASHING, 8, dstring_key_hash_case, (int (*)(const void*, const void*))case_compare_dstring, offsetof(dmap_entry, embed_node));
	else
		initialize_hashmap(dmap_p, ROBINHOOD_HASHING, 8, dstring_key_hash, (int (*)(const void*, const void*))compare_dstring, offsetof(dmap_entry, embed_node));
}

dmap_entry* get_from_dmap(const dmap* dmap_p, const dstring* key)
{
	return (dmap_entry*) find_equals_in_hashmap(dmap_p, &((dmap_entry){.key = (*key)}));
}

dmap_entry* insert_in_dmap(dmap* dmap_p, const dstring* key, const dstring* value)
{
	dmap_entry* dmap_entry_p = malloc(sizeof(dmap_entry));
	init_dmap_entry(dmap_entry_p, key, value);

	// insert the new dmap_entry in hashmap
	// if it fails then expand the hashmap and retry insertion
	if(!insert_in_hashmap(dmap_p, dmap_entry_p))
	{
		expand_hashmap(dmap_p, 2.0);
		insert_in_hashmap(dmap_p, dmap_entry_p);
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
		write_to_stream_formatted(ws, "<" printf_dstring_format "> -> <" printf_dstring_format ">\n", &error, printf_dstring_params(&(e->key)), printf_dstring_params(&(e->value)));
		if(error)
			break;
	}
}

static void destroy_dmap_entries_in_dmap(const void* data, const void* additional_params)
{
	dmap_entry* e = (dmap_entry*) data;
	deinit_dmap_entry(e);
	free(e);
}

void deinit_dmap(dmap* dmap_p)
{
	for_each_in_hashmap(dmap_p, destroy_dmap_entries_in_dmap, NULL);
	deinitialize_hashmap(dmap_p);
}