#include <iostream>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h> 
#include <sys/stat.h>
#include <errno.h>

using namespace std;

static int semaphore_id = 34;

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

int semaphore_0_initialize(int semaph_id); 
int semaphore_1_initialize(int semaph_id); 
int semaphore_P(int semaph_id); 
int semaphore_V(int semaph_id);
int semaphore_delete(int semaph_id);
int semaphore_create(void); 
