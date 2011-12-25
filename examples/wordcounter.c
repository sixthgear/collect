#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "vect.h"

const int max_words = 17609;
const int max_buffer = 100;

typedef struct wc_tuple {
	char word[max_buffer];
	unsigned int count;
} wc_tuple;

HASH_GENERATE_NAME(unsigned int, int)
VECT_GENERATE_NAME(wc_tuple, wc)

int cmp_wc(const void *a, const void *b);

int 
main (int argc, char const *argv[])
{	
	/* allocate hash table of ints to store counts 			      */
	/* i've implemented a *closed* hash table with quadratic probing      */
	/* so i tried to pick some large prime value as an upper limit.       */
	table *wt = ht_init_int(max_words, max_buffer, ht_hash, ht_quad_probe);
	vect_wc *wv = vect_init_wc(0);
			
	char buf[max_buffer]; /* buffer to hold each word as we discover it   */
	char c;	              /* char we are currently examining in the text  */
	int count;            /* count of the current word in the text 	      */
	int i = 0; 	      /* offset to the current position in the buffer */
	
	/* read a character from stdin */
	while((c = getchar()) != EOF) {
						
		/* look for valid characters. */
		/* special characters - and ' are only allowed mid-word */
		if (isalnum(c) || i > 0 && (c == '\'' || c == '-')) {
			buf[i++] = tolower(c);
			if (i >= max_buffer-1)
				break;
			continue;
		}
		
		/* found a delimiting character */
		/* skip if the buffer is empty */
		if (i == 0)
			continue;
		
		/* append null byte */
		buf[i] = '\0';
		
		/* remove any trailing special characters */		
		while (!isalnum(buf[i]) && i > 0)
			buf[i--] = '\0';
		
		/* find out if we've already stored this word */
		if (ht_has_key(wt, buf))			
			count = ht_get_int(wt, buf);
		else
			count = 0;
		
		/* add or update the word, incrementing count */
		ht_add_int(wt, buf, count+1);
		
		/* clear the buffer for the next word */
		buf[0] = '\0';
		i = 0;
	}	
		
	/* convert hash table into vector */
	for (int i=0; i < wt->size; i++) {
		struct wc_tuple wc;
		strcpy(wc.word, wt->keys + wt->key_len * i);
		wc.count = *((unsigned int *) wt->values + i);
		if (wc.word[0])
			vect_push_wc(wv, wc);
	}
	
	/* sort the vector */
	qsort(wv->data, wv->size, wv->data_size, cmp_wc);
	
	/* output sorted vector */	
	for (int i=0; i < wv->size; i++)
		printf("%s: %d\n", wv->data[i].word, wv->data[i].count);

	/* not necessary, but momma always told me to free what i malloc */
	ht_free(wt);
	vect_free(wv);
	return 0;
}

int 
cmp_wc(const void *a, const void *b) 
{	
	wc_tuple *aa = (wc_tuple *) a;
	wc_tuple *bb = (wc_tuple *) b;
	int count = bb->count - aa->count;
	if (!count)
		return strcmp(aa->word, bb->word);
	return count;
}
