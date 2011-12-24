#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "vect.h"
#include "minunit.h"

int tests_run = 0;
	
VECT_GENERATE_TYPE(int)	
VECT_GENERATE_NAME(uint64_t, ui64)

static char * test_resize() {
	int result;
	vect_int *vi = vect_init_int(1);
	vect_push_int(vi, 1);
	vect_push_int(vi, 2);
	vect_push_int(vi, 3);
	vect_push_int(vi, 4);
	vect_push_int(vi, 5);
	result = vi->capacity;
	vect_free(vi);
	mu_assert("error: vi->capacity != 8", result == 8);
	return 0;
}

static char * test_fibs_build() {	
	/* build fibs sequence */
	uint64_t result;
	vect_ui64 *vl = vect_init_ui64(8);
	uint64_t a = 0;
	uint64_t b = 1;	
	vect_push_ui64(vl, 0);
	for(int i=1; i<94; i++) {
		uint64_t tmp = a;
		a = a + b;
		b = tmp;
		vect_push_ui64(vl, a);
	}		
	result = vect_at_ui64(vl, vl->size-1);
	vect_free(vl);		
	mu_assert("error: result != 12200160415121876738", result == 12200160415121876738ULL);
	return 0;
}

static char * all_tests() {
	mu_run_test(test_resize);
	mu_run_test(test_fibs_build);
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
