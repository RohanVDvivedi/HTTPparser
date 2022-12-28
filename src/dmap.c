#include<dmap.h>

#include<stdlib.h>
#include<ctype.h>

void init_dmap_entry(dmap_entry* dmap_entry_p, const dstring* key)
{
	init_copy_dstring(&(dmap_entry_p->key), key);
	dmap_entry_p->values_capacity = 0;
	dmap_entry_p->values_count = 0;
	dmap_entry_p->values = NULL;
}

void insert_in_dmap_entry(dmap_entry* dmap_entry_p, const dstring* value)
{
	if(dmap_entry_p->values_count == dmap_entry_p->values_capacity)
	{
		dmap_entry_p->values_capacity = 2 * dmap_entry_p->values_capacity + 1;
		dmap_entry_p->values = realloc(dmap_entry_p->values, dmap_entry_p->values_capacity * sizeof(dstring));
	}
	init_copy_dstring(dmap_entry_p->values + dmap_entry_p->values_count++, value);
}

void deinit_dmap_entry(dmap_entry* dmap_entry_p)
{
	deinit_dstring(&(dmap_entry_p->key));
	for(unsigned int i = 0; i < dmap_entry_p->values_count; i++)
		deinit_dstring(dmap_entry_p->values + i);
	if(dmap_entry_p->values)
		free(dmap_entry_p->values);
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
		initialize_hashmap(dmap_p, ROBINHOOD_HASHING, 8, dstring_key_hash_case, (int (*)(const void*, const void*))case_compare_dstring, 0);
	else
		initialize_hashmap(dmap_p, ROBINHOOD_HASHING, 8, dstring_key_hash, (int (*)(const void*, const void*))compare_dstring, 0);
}

dmap_entry* get_from_dmap(const dmap* dmap_p, const dstring* key)
{
	return (dmap_entry*) find_equals_in_hashmap(dmap_p, &((dmap_entry){.key = (*key)}));
}

dmap_entry* insert_in_dmap(dmap* dmap_p, const dstring* key)
{
	dmap_entry* e = get_from_dmap(dmap_p, key);
	if(e == NULL)
	{
		e = malloc(sizeof(dmap_entry));
		init_dmap_entry(e, key);

		// insert the new dmap_entry in hashmap
		// if it fails then expand the hashmap and retry insertion
		if(!insert_in_hashmap(dmap_p, e))
		{
			expand_hashmap(dmap_p, 2.0);
			insert_in_hashmap(dmap_p, e);
		}
	}
	return e;
}

int delete_from_dmap(dmap* dmap_p, const dstring* key)
{
	dmap_entry* e = get_from_dmap(dmap_p, key);
	if(e == NULL)
		return 0;

	remove_from_hashmap(dmap_p, e);
	deinit_dmap_entry(e);
	free(e);
	return 1;
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