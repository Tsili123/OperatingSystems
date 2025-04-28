#include "semaphore_operators.hpp"

using namespace std;

int semaphore_P(int semaph_id){
    // Perfosms the P() operation on given semaphore, returns -1 on ERROR, otherwise 0
	struct sembuf sem_buffer = { 0 , -1 , 0};

	if( semop(semaph_id, &sem_buffer, 1) < 0 ){
		cout <<  "[ERROR] Semaphore P() with " << semaph_id << " failed." << endl;
		return -1;
	}
	return 0;
};

int semaphore_V(int semaph_id){
    // Perfosms the V() operation on semaphore, returns -1 on ERROR, otherwise 0.
	struct sembuf sem_buffer = {0 , 1 , 0};

	if( semop(semaph_id, &sem_buffer, 1) < 0 ){
		cout <<  "[ERROR] Semaphore V() with " << semaph_id << " failed." << endl;
		return -1;
	}
	return 0;
}

int semaphore_0_initialize(int semaph_id){
    // Initializes specified sempahore, with value 0, returns -1 on ERROR, else 0
	union semun arg;
	arg.val=0;

	if( semctl(semaph_id,0,SETVAL,arg) < 0 ){
        cout << "[ERROR] Cannot initialize the semaphore to 0 with id number: " << semaph_id << endl;
        return -1;
    }
    return 0;
}

int semaphore_1_initialize(int semaph_id){
    // Initializes specified sempahore, with value 1, returns -1 on ERROR, else 0. 
	union semun arg;
	arg.val=1;

	if( semctl(semaph_id,0,SETVAL,arg) < 0 ){
        cout << "[ERROR] Cannot initialize the semaphore to 1 with id number: " << semaph_id << endl;
        return -1;
    }
    return 0;
}

int semaphore_delete(int sem_id){
    //Deletes specified semaphore,returns -1 on ERROR, else 0.
	union semun sem_union;
	if ( semctl(sem_id, 0, IPC_RMID) < 0 ){
        cout << "[ERROR]Cannot delete semaphore with id: " << sem_id << endl;
        return -1;
    }
    return 0;
};

int semaphore_create(void){
    // Creates a system V semaphore and returns it's ID or -1 on ERROR. 
    int semaphore;
    semaphore_id++; // Increment this, so that next semaphore we create has a completely new id number
    key_t sem_key = ftok("test.txt",semaphore_id);

    if( sem_key < 0 ){
        cout << "[ERROR]Cannot create semaphore key.(ftok) " << endl;
        return -1;
    }
    semaphore = semget(sem_key,1,IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    int errsv = errno;

    if( semaphore < 0 ){
        cout << "[ERROR]Cannot create semaphore. " << endl;
        return -1;
    }
    return semaphore;
}