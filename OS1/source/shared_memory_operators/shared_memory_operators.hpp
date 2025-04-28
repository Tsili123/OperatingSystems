#include <iostream>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

// ds_out 
static key_t ds_out_key;
static int ds_out_id;
static char * ds_out_ptr;
// ds_in 
static key_t ds_in_key;
static int ds_in_id;
static char * ds_in_ptr;
// msg_count 
static key_t msg_count_key;
static int msg_count_id;
static char * msg_count_ptr;

static int shared_memory_id = 34;
extern unsigned int line_max_length;

char * get_shared_memory_ptr(int id); 
int shared_memory_delete(int shmid); 
int shared_memory_initialize(int size);
