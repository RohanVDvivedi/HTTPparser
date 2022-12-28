#ifndef DMAP_H
#define DMAP_H

#include<dstring.h>
#include<hashmap.h>

typedef struct dmap_entry dmap_entry;
struct dmap_entry
{
	dstring key;

	unsigned int values_capacity;
	unsigned int values_count;
	dstring* values;
};

void init_dmap_entry(dmap_entry* dmap_entry_p, const dstring* key);

void insert_in_dmap_entry(dmap_entry* dmap_entry_p, const dstring* value);

void deinit_dmap_entry(dmap_entry* dmap_entry_p);



typedef hashmap dmap;

void init_dmap(dmap* dmap_p, int case_in_sensitive_key);

dmap_entry* get_from_dmap(const dmap* dmap_p, const dstring* key);

// inserts a new dmap_entry in dmap with the given key and returns the inserted dmap
dmap_entry* insert_in_dmap(const dmap* dmap_p, const dstring* key);

int delete_from_dmap(const dmap* dmap_p, const dstring* key);

void deinit_dmap(dmap* dmap_p);

#endif