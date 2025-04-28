#include "sharedmem.hpp"
#include "semaphores.hpp"
#include <random>
#include <unistd.h>
#include <sys/wait.h>
#include "queue.hpp"

using namespace std;

unsigned int N=7; 

struct  process{
    int pid;
    int priority;
    int state; // 0 ready 1 wait 2 busy 3 ended
    double stop_time; 
    int cpu_burst; //must be quantums
    int waiting_time; //must be quantums

    process(int pid,int priority,int ready,double stop_time,double cpu_burst,int waiting_time) {
        this->pid = pid;
        this->priority = priority;
        this->state = ready;
        this->stop_time = stop_time;
        this->cpu_burst =  cpu_burst;
        this->waiting_time =  waiting_time;
    }

    void getvalues() {
        cout << endl;
        cout << "process : "<< this->pid <<endl;
        cout << "priority : "<< this->priority << endl;
        cout << "ready(0 or 1 ) : "<< this->state << endl;
        cout << "waiting time : "<< this->waiting_time <<endl;
        cout << "cpu burst time : "<< this->cpu_burst <<endl;
        std::cout << std::scientific;
        cout << "stopping time : "<< this->stop_time <<endl;
        
    }
};



int main(int argc, char *argv[]){

    int S = 5;
    srand(time(NULL));

    //random priorities of procedures
    const int range_from  = 1;
    const int range_to    = 7;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);
    std::uniform_int_distribution<int>  distr2(0, 4);

    //std::default_random_engine generator;
    std::default_random_engine generators;
    std::exponential_distribution<double> distribution(0.8); // generate numbers commonly between (10-400) from exponential distribution in order 
    // to simulate the procedure

    int p[N];
    for(int i=0;i<N;i++){
        p[i] = distr(generator);
    }

    // for(int i=0;i<N;i++){
    //    cout <<  p[i] << endl;
    // }

    struct process *s[N];
    for (int i = 0; i < N; i++) {
        double quantum = distribution(generator);
        double cpu_burst = distr2(generator);
        s[i] = new process(i, p[i],0,quantum,cpu_burst,0);
        s[i]->getvalues();
        usleep(quantum);
    }

    // Semaphore Initialization 
    //create an array of semaphores
    int sembuffer[S];
    int sem;
    for(int i=0;i<S; i++){
        sembuffer[i] = semaphore_create();
        //initialize them with 0
        if( semaphore_init_1(sembuffer[i]) == -1 ) return -1;
    }

    for(int i=0;i<S; i++){
        cout << sembuffer[i] << endl;
    }

    int val;
    val =  semaphore_getvalue(sembuffer[0]);
    cout << " value of first semaphore " << val << endl;
    
    //Untitled132.cpp

    //insert processes to ready queue



    // ready queue
    // waiting queue
    // critical section 

    Queue ready_queue;
    Queue working_queue;
    Queue waiting_queue[N];

    ready_queue.outQueue();      //outQueue 
     
     cout<<"Ready queue created:"<<endl; 
     for(int i = 0; i < N; i++){
         ready_queue.inQueue(i);//s[i]->priority
     }
     
     
    ready_queue.displayQueue();

    // for(int j=0;j<2;j++){
    //     for(int i = 0; i < 3; i++){
    //      waiting_queue[j].inQueue(s[i]->priority);
    //  }
    // }

    // waiting_queue[0].displayQueue();
    // waiting_queue[1].displayQueue();
    
    // waiting_queue[1].outQueue();
    // waiting_queue[1].displayQueue();

    int m;
   while(1){

       //get all process out of waiting queue
        for(int j = 0; j < S; j++){
            if( waiting_queue[j].isEmpty())
                continue;
            else  {
                //try to access semaphore set
                // empty waiting queue 
                //for(int i = 0; i < N; i++){

                    int proc;
                    cout << endl;
                    proc = waiting_queue[j].outQueue();
                    if(proc==-1)
                    continue;

                    cout << "\nProcess No." << proc << " choosed semaphore " << j << endl;
                            
                    if(semaphore_getvalue(sembuffer[j]) > 0){
                        working_queue.inQueue(proc);
                        //critical section
                        semaphore_p(sembuffer[j]);

                    }else{
                        waiting_queue[j].inQueue(proc);
                    }
                }
        } 

       //get all processes out of ready queue
       for(int i = 0; i < N; i++){
           int proc;
          
           //try to access semaphore set
           float prob = 0.6;
           if(prob > 0.5){
                //cout << "EDW" << endl;
                proc = ready_queue.outQueue();
                if(proc==-1)
                    continue;
                m = distr2(generator);
                // choose random semaphore
                cout << "\nprocess No." << proc << " choosed semaphore " << m << endl;

                if(semaphore_getvalue(sembuffer[m]) > 0){
                    working_queue.inQueue(proc);
                    //critical section
                    semaphore_p(sembuffer[m]);
                }else{
                    waiting_queue[m].inQueue(proc);
                }
           }
       }

        for(int i = 0; i < S; i++){
            if(semaphore_getvalue(sembuffer[i]) == 0){
                //sleep random exponential time
                sleep(distribution(generator));
                semaphore_v(sembuffer[i]);
                int var2;
                var2 = working_queue.outQueue();
                ready_queue.inQueue(var2);
            }
        }
   }

    //if( getpid() == main_pid ){
    for(int i=0;i<S; i++){
        semaphore_delete((int)sembuffer[i]);
    }
  
    
}