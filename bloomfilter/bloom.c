
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "bloom.h"


//list of hash functions
struct bloom_hash {
    hash_function func;
    struct bloom_hash *next;
};
//bloom filter hash
//Contains
//array of bits+pointer to head of hash function list
struct bloom_filter {
    struct bloom_hash *func;
    void *bits;
    size_t size;
};
//Create bloom filter
//Initialize its parts
bloom_t new_bloom(size_t size) {
	bloom_t blm = calloc(1, sizeof(struct bloom_filter));
	blm->size = size;
	blm->bits = malloc(size);
	return blm;
}

//deallocate the space
void free_bloom(bloom_t bfilter) 
{
	struct bloom_hash *hash;
	if (bfilter!=NULL) {//traverse list of fuctions
		while (bfilter->func!=NULL) 
		{
			hash = bfilter->func;
			bfilter->func = hash->next;
			//delete functions one by one
			free(hash);
		}
		//free array
		free(bfilter->bits);
		free(bfilter);
	}
}

//Just add more hash functions to the list of them
void addf_bloom(bloom_t bfilter, hash_function func)
{
	//create node of function for function list
	struct bloom_hash *function = calloc(1, sizeof(struct bloom_hash));
	//get function
	function->func = func;
	//traverse until the last node
	struct bloom_hash *last_node = bfilter->func;
	while (last_node!=NULL && (last_node->next) !=NULL) 
	{
		last_node = last_node->next;
	}
	if (last_node!=NULL) {//if there is not one node in the function list
		last_node->next = function;
	} else {//if there are more nodes in the fuction list
		bfilter->func = function;
	}
}
//add value to the array of hashes
void addk_bloom(bloom_t bfilter, const void *item,size_t size) 
{
	//1.
	//initialize with first node
	struct bloom_hash *hashf = bfilter->func;
		//2.traverse hash function list

	uint8_t *bits = bfilter->bits;
	while (hashf) {
		//get the value from every function
		unsigned int hash = hashf->func(item);
		//get array index
		hash %= size;
		//map a bit type value to a byte type value
		//by dividing with 8 to find the proper byte 
		//and then shifting and bitwise or to put 1 to the correct bit
		bits[hash / 8] |= 1 << hash % 8;
		hashf = hashf->next;
	}
}
//same as before but we return true/false if we find value
bool check_bloom(bloom_t bfilter, const void *item,size_t size) 
{
	struct bloom_hash *hashf = bfilter->func;
	uint8_t *bits = bfilter->bits;
	while (hashf!=NULL) {
		unsigned int hash = hashf->func(item);
		hash %= size;
		//with bitwise end we find if there is 1 in the 
		//position that occured from the modulo
		if (!(bits[hash / 8] & 1 << hash % 8)) 
		{
			return false;
		}
		hashf = hashf->next;
	}
	return true;
}

