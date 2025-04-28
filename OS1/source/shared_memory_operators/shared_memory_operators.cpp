#include "shared_memory_operators.hpp"

int shared_memory_delete(int shm_id){ 
    // Deletes all of the shared memory segments. It returns -1 on ERROR, otherwise 0 
    if( shmctl(shm_id,IPC_RMID,0) == -1 ){
        cout << "[ERROR] Cannot delete the " << shm_id << " memory segment. (shmctl)" << endl;
        return -1;
    }    
    return 0;
}

char* get_shared_memory_ptr(int shm_id){
    // Returns pointer to pointer of given ID, or NULL in case of error 
    return (char *)shmat(shm_id,NULL,0); 
}

int shared_memory_initialize(int size){

    // Initializes a shared memory segment with given size in bytes and returns its ID, or -1 on error 
    int shm_id;
    key_t shm_key;

    shm_key = ftok("test.txt",shared_memory_id);
    if( shm_key < 0 ){
        cout << "[ERROR]Error when creating shared memory segment key. (ftok)" << endl;
        return -1;
    }
    shared_memory_id++; // Increment this, so that the next shared memory segment we create will have a tottaly new id 
    shm_id = shmget(shm_key,size, 0666 | IPC_CREAT);
    
    if( shm_id < 0 ){
        cout << "[ERROR]Error when creating shared memory segment. (shmget)" << endl;
        return -1;
    }
    return shm_id;
}