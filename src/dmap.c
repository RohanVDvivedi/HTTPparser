#include<dmap.h>

#include<stdlib.h>

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

void init_dmap(dmap* dmap_p, int case_in_sensitive_key);

dmap_entry* get_from_dmap(const dmap* dmap_p, const dstring* key);

dmap_entry* insert_in_dmap(const dmap* dmap_p, const dstring* key);

int delete_from_dmap(const dmap* dmap_p, const dstring* key);

void deinit_dmap(dmap* dmap_p);