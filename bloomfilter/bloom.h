#ifndef _BLOOM_H
#define _BLOOM_H
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

typedef unsigned int (*hash_function)(const void *data);
typedef struct bloom_filter * bloom_t;

/* Creates a new bloom filter with no hash functions and size * 8 bits. */
bloom_t new_bloom(size_t size);
/* Frees a bloom filter. */
void free_bloom(bloom_t bfilter);
/* Adds a hashing function to the bloom filter. You should add all of the
 * functions you intend to use before you add any items. */
void addf_bloom(bloom_t bfilter, hash_function funcs);
/* Adds an item to the bloom filter. */
void addk_bloom(bloom_t bfilter, const void *item,size_t size);
/* Tests if an item is in the bloom filter.
 *
 * Returns false if the item has definitely not been added before. Returns true
 * if the item was probably added before. */
bool check_bloom(bloom_t bfilter, const void *item,size_t size);

#endif
