#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "vect.h"
#include "minunit.h"

int tests_run = 0;
	
VECT_GENERATE_TYPE(int)
VECT_GENERATE_NAME(uint64_t, ui64)

static char *
test_pushpop() 
{
	int result = 0;
	vect_int *vi = vect_init_int(1);
	for (int i=0; i<100; i++) 
		vect_push_int(vi, i);	
	while(vi->size)
		result += vect_pop_int(vi);
		
	mu_assert("error pushpop: result != 4950", result == 4950);
	mu_assert("error pushpop: vi->size != 0", vi->size == 0);
	vect_free(vi);
	return 0;
}

static char * 
test_resize() 
{
	vect_int *vi = vect_init_int(1);
	for (int i=0; i<257; i++) {
		vect_push_int(vi, i);
	}
		
	mu_assert("error resize: vi->capacity != 512", vi->capacity == 512);
	vect_free(vi);
	return 0;
}

static char * 
test_fibs_build() 
{
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
	
	mu_assert("error fibs: result != 12200160415121876738", 
			       result == 12200160415121876738ULL);
	vect_free(vl);
	return 0;
}

static char * 
all_tests() 
{
	mu_run_test(test_pushpop);
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
