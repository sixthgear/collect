#ifndef HASH_H
#define HASH_H

/*
 * hash.h -- generic closed hash table implementation.
 * constant memory, linear/quadratic probing, variable max keylength 
 * by sixthgear. BSD Licenced
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct table {
	unsigned long (*hashfunc)(const char *);
	unsigned long (*probefunc)(struct table *, const char *, bool);
	int size;
	int key_len;
	char *keys;
	void *values;
} table;

table *
ht_init(int num, size_t type_size, int key_len, void *hf, void *pf)
{
	size_t total_size;
	total_size = sizeof(table);			/* table */
	total_size += sizeof(char) * key_len * num; 	/* keys */
	total_size += (type_size * num); 		/* values */
	table *ht = malloc(total_size);
	ht->size = num;	
	ht->key_len = key_len;
	ht->hashfunc = hf;
	ht->probefunc = pf;
	ht->keys = (char *)(ht + 1);
	ht->values = ht->keys + key_len * num;
	return ht;
}

void
ht_free(table *ht)
{
	free(ht);
}

unsigned long 
ht_hash(const char *key)
{
	int c;
	unsigned long hash = 5381;
	while ((c = *key++))
		hash = ((hash << 5) + hash) + c;
	return hash;
}

char *
ht_key_idx(table *ht, unsigned long idx)
{
	return &ht->keys[idx * ht->key_len];
}

unsigned long
ht_lin_probe(table *ht, const char *key, bool error_on_blank)
{
	unsigned long k = ht->hashfunc(key) % ht->size;
	for (int j=0; j<ht->size; j++) {
		if (*ht_key_idx(ht, k) == '\0')
			/* blank */
			return error_on_blank ? -1 : k;
		if (strncmp(ht_key_idx(ht, k), key, ht->key_len-1) == 0)
			/* key exists */
			return k;
		k = (k + 1) % ht->size;
	}
	/* table is full */
	return -1; 
}

unsigned long
ht_quad_probe(table *ht, const char *key, bool error_on_blank)
{
	unsigned long k = ht->hashfunc(key);
	unsigned long i = k % ht->size;
	for (int j=1; j<=ht->size; j++) {
		if (*ht_key_idx(ht, i) == '\0')
			/* blank */
			return error_on_blank ? -1 : i; 
		if (strncmp(ht_key_idx(ht, i), key, ht->key_len-1) == 0)
			/* key exists */
			return i;
		i = ( k + j * j ) % ht->size;
	}
	/* table is full */
	return -1; 
}

unsigned long
ht_add_key(table *ht, const char *key)
{
	unsigned long i = ht->probefunc(ht, key, false);
	if (i == -1) {			
		fprintf(stderr, "HT ERROR: table full.\n");
		exit(1);
	}
	strncpy(ht_key_idx(ht, i), key, ht->key_len-1);
	ht_key_idx(ht, i+1)[-1] = '\0';
	return i;
}

unsigned long 
ht_get_key(table *ht, const char *key)
{
	unsigned long i = ht->probefunc(ht, key, true);
	if (i == -1) {
		fprintf(stderr, "HT KEY ERROR: \"%s\"\n", key);
		exit(1);
	}
	return i;
}

void
ht_del_key(table *ht, const char *key)
{
	unsigned long i = ht_get_key(ht, key);
	*ht_key_idx(ht, i) = '\0';
}

bool
ht_has_key(table *ht, const char *key)
{
	return ht->probefunc(ht, key, 1) != -1;
}

#define HASH_GENERATE_TYPE(TYPE) HASH_GENERATE_NAME(TYPE, TYPE) 
#define HASH_GENERATE_NAME(TYPE, NAME)					       \
table *								       	       \
ht_init_##NAME(int num, int key_len, void *hf, void *pf) 		       \
{ 									       \
	return ht_init(num, sizeof(TYPE), key_len, hf, pf);		       \
}									       \
void								       	       \
ht_add_##NAME(table *ht, const char *key, TYPE value)			       \
{									       \
	((TYPE *) ht->values)[ht_add_key(ht, key)] = value;		       \
}									       \
TYPE								       	       \
ht_get_##NAME(table *ht, const char *key)				       \
{									       \
	return ((TYPE *) ht->values)[ht_get_key(ht, key)];		       \
}                                                                              \
TYPE *									       \
ht_ptr_##NAME(table *ht, const char *key)				       \
{									       \
	return ((TYPE *) ht->values) + ht_get_key(ht, key);		       \
}                                                                              \
void								       	       \
ht_del_##NAME(table *ht, const char *key)				       \
{									       \
	ht_del_key(ht, key);						       \
}

#endif