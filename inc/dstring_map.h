#ifndef DMAP_H
#define DMAP_H

typedef struct dmap_entry dmap_entry;
struct dmap_entry
{
	dstring key;

	unsigned int values_count;
	dstring* values;
};

typedef hashmap dmap;

void init_dmap(dmap* dmap_p, int case_in_sensitive_key);

dmap_entry* get_from_dmap(const dmap* dmap_p, const dstring* key);

dmap_entry* insert_in_dmap(const dmap* dmap_p, const dstring* key);

int delete_from_dmap(const dmap* dmap_p, const dstring* key);

void deinit_dmap(dmap* dmap_p, int case_in_sensitive_key);

#endif