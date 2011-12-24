#include <stdlib.h>
#include <stdio.h>
#include "hash.h"

HASH_GENERATE_TYPE(int);
HASH_GENERATE_TYPE(float);	

int 
main()
{
	table *hti = ht_init_int(3700, 32, ht_hash, ht_lin_probe);
	table *htf = ht_init_float(23, 32, ht_hash, ht_quad_probe);
	
	ht_add_int(hti, "hello", 100);
	ht_add_int(hti, "hellote", 150);
	ht_add_int(hti, "world!", 200);
	ht_add_int(hti, "foo", 300);
	ht_add_int(hti, "a", 1);
	ht_add_int(hti, "b", 2);
	ht_add_int(hti, "c", 3);
	ht_add_int(hti, "d", 4);	
	ht_add_int(hti, "e", 5);
	ht_add_int(hti, "f", 6);
	ht_add_int(hti, "g", 7);
	ht_del_int(hti, "g");		
	// ht_get_int(hti, "hello");
	
	ht_add_float(htf, "hello", 10.0/3);
	ht_add_float(htf, "world!", 67.2);
	ht_add_float(htf, "foo", 11.111);
	
	// ht_get_float(htf, "hello");
	
	printf("\nINTS:\n");
	for (int i=0; i<hti->size; i++) {
		char *key = (char *) hti->keys + i * hti->key_len;
		if (!key[0]) continue; 
		int val = *((int *)hti->values + i);
		printf("%d: '%s': %d\n", i, key, val);
	}		
	
	printf("\nFLOATS:\n");
	for (int i=0; i<htf->size; i++) {
		char *key = (char *) htf->keys + i * htf->key_len;
		if (!key[0]) continue; 
		float val = *((float *)htf->values + i);
		printf("%d: '%s': %f\n", i, key, val);
	}		
		
	ht_free(hti);
	ht_free(htf);
		
	return 0;
}
