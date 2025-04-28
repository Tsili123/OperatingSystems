#include "sharedmem.hpp"
#include "semaphores.hpp"
#include <random>
#include <unistd.h>
#include <sys/wait.h>
#include "queue.hpp"
#include <stdio.h>

using namespace std;

struct  process{
    int pid;
    int priority;
    double stop_time; 
    int sem; 
    int waiting_time; //must be quantums
    int downup=0;

    process(int pid,int priority,double stop_time,int sem,int waiting_time) {
        this->pid = pid;
        this->priority = priority;
        this->stop_time = stop_time;
        this->sem =  sem;
        this->waiting_time =  waiting_time;
    }

    void getvalues() {
        cout << endl;
        cout << "process : "<< this->pid <<endl;
        cout << "priority : "<< this->priority << endl;
        cout << "waiting time : "<< this->waiting_time <<endl;
        cout << "current semaphore : "<< this->sem <<endl;
        std::cout << std::scientific;
        cout << "stopping time : "<< this->stop_time <<endl;
        cout << "down up times : "<< this->downup <<endl;
        
    }
};


int main(int argc, char *argv[]){

    int S;
    float l1,l2,l3;
    unsigned int N;
    unsigned int  karg;
    srand(time(NULL));

    for(int i=1;i<argc;i++){
        if(i==1){
            l1 = stof(argv[i]);
            printf( "%6.4lf \n", l1 );
        }

        if(i==2){
            l2 = stof(argv[i]);
            printf( "%6.4lf \n ", l2 );
        }

        if(i==3){
            l3 = stof(argv[i]);
            printf("%6.4lf \n", l3 );
        }

        if(i==4){
            N = stoi(argv[i]);
            cout << N << endl;
        }

        if(i==5){
             karg = stoi(argv[i]);
            cout <<  karg << endl;
        }

        if(i==6){
            S = stoi(argv[i]);
            cout << S << endl;
        }
    }

    //random priorities of procedures
    const int range_from  = 1;
    const int range_to    = 7;
    std::random_device                  rand_dev;
    std::random_device                  rand_devk;
    std::mt19937                        generator(rand_dev());
    std::mt19937                        generatork(rand_devk());
    std::uniform_int_distribution<int>  distr2(0, S-1);
    std::uniform_int_distribution<int>  distr(1, N);
    std::uniform_int_distribution<int>  distrk(1, 100);

    //std::default_random_engine generator;
    std::default_random_engine generators;
    std::exponential_distribution<double> distribution(l1); // generate numbers commonly between (10-400) from exponential distribution in order 
    std::exponential_distribution<double> distribution2(l3); // generate numbers commonly between (10-400) from exponential distribution in order 
    // to simulate the procedure
    // l1 0.05 l3 0.8

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
        //double cpu_burst = distr2(generator);
        s[i] = new process(i, p[i],quantum,-1,0);
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
    Queue waiting_queue[S];

    ready_queue.outQueue();      //outQueue 
     
     cout<<"Ready queue created:"<<endl; 
     for(int i = 0; i < N; i++){
         ready_queue.inQueue(i);//s[i]->priority
     }
     
     
    ready_queue.Queuedisplay();


    int counter=0;
   while(1){
       if (counter==6)
            break;
       // move processes from ready queue to semaphore waiting rooms
       
        cout << "\n Procedure - fill the waiting rooms of semaphores" << endl;
         int proc2;
        for(int j=0;j<N;j++){
        //get all process out of ready queue
            if( ready_queue.isEmpty()){
                continue;
            }else  {
                    cout << endl;
                    proc2 = ready_queue.outQueue();
                    if(proc2==-1)
                        continue;

                    int m = distr2(generator);
                    s[proc2]->sem = m;
                    // choose random semaphore
                    cout << m << endl;
                    waiting_queue[m].inQueue(proc2);
            }
        }

        //fix waiting times
        for(int i = 0; i < N; i++){
            s[i]->waiting_time++;
        }

        for(int i = 0; i < S; i++){
            proc2 = waiting_queue[i].getFront();
            if(proc2>-1 && proc2 < N)
                s[proc2]->waiting_time--;
        }

        cout << "\n Critical section - empty one place for each waiting room" << endl;

        for(int i = 0; i < S; i++){
        
            //try to access semaphore set
            int kvar = distrk(generatork);
            int proc;

            if(kvar > karg){

                proc = waiting_queue[i].outQueue();
                //empty waiting room
                if(proc==-1)
                    continue;
                
                cout << "\nprocess No." << proc << " choosed semaphore " << s[proc]->sem << endl;
                int var = s[proc]->sem;
                //critical section
                semaphore_p(sembuffer[var]);
                usleep(distribution2(generator));
                semaphore_v(sembuffer[var]);
                s[proc]->downup++;
                ready_queue.inQueue(proc);

            }else{
                cout << "\nsemaphore "<< i <<" down/up missed"<< endl;
                proc = waiting_queue[i].getFront();
                if(proc==-1)
                    continue;
                else
                    s[proc]->waiting_time++;
                continue;
            }
            //ready_queue.inQueue(proc);
        }

        counter++;
   }

    cout << "\n\n";
    for(int i=0;i<N;i++){
        s[i]->getvalues();
        cout << "\n";
    }

    //if( getpid() == main_pid ){
    for(int i=0;i<S; i++){
        semaphore_delete((int)sembuffer[i]);
    }
  
    
}

