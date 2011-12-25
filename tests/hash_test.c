#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#include "minunit.h"

int tests_run = 0;

HASH_GENERATE_TYPE(int)
HASH_GENERATE_TYPE(float)

static char *
test_probes()
{	
	table *ht[2];
	int result[2];
	ht[0] = ht_init_int(11, 32, ht_hash, ht_lin_probe);
	ht[1] = ht_init_int(13, 32, ht_hash, ht_quad_probe);
	
	for (int i=0; i<2; i++) {		
		result[i] = 0;
		ht_add_int(ht[i], "one", 1);
		ht_add_int(ht[i], "two", 2);
		ht_add_int(ht[i], "three", 3);
		ht_add_int(ht[i], "four", 4);
		ht_add_int(ht[i], "five", 5);
		ht_add_int(ht[i], "six", 6);
		ht_add_int(ht[i], "seven", 7);
		ht_add_int(ht[i], "eight", 8);	
		ht_add_int(ht[i], "nine", 9);
		ht_add_int(ht[i], "ten", 10);
		ht_add_int(ht[i], "eleven", 11);
		
		result[i] += ht_get_int(ht[i], "one");
		result[i] += ht_get_int(ht[i], "two");
		result[i] += ht_get_int(ht[i], "three");
		result[i] += ht_get_int(ht[i], "four");
		result[i] += ht_get_int(ht[i], "five");
		result[i] += ht_get_int(ht[i], "six");
		result[i] += ht_get_int(ht[i], "seven");
		result[i] += ht_get_int(ht[i], "eight");
		result[i] += ht_get_int(ht[i], "nine");
		result[i] += ht_get_int(ht[i], "ten");
		result[i] += ht_get_int(ht[i], "eleven");
		
		ht_free(ht[i]);
	}
		
	mu_assert("error probes: linear probe res != 66", result[0] == 66);
	mu_assert("error probes: quadratic probe res != 66", result[1] == 66);

	return 0;
}

static char * all_tests() {
	mu_run_test(test_probes);
	return 0;
}

int 
main()
{	
	char *result = all_tests();
	if (result != 0) 
		printf("%s\n", result);
	else
		printf("ALL TESTS PASSED\n");

	printf("Tests run: %d\n", tests_run);
	return result != 0;
}

