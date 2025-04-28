#include <iostream>
#include <math.h>
#include <string>
#include <ctime>
#include <queue>
#include <random>
#include <list>

#include "process.hpp"

using namespace std;

struct S
{
    int m_n1;//prio
    int m_n2;//time
    int m_n3;//pid

    S(int n1, int n2, int n3) : m_n1(n1), m_n2(n2), m_n3(n3)
    {
    }

    bool operator<(const struct S& other) const
    {
        //Your priority logic goes here
        return m_n1 < other.m_n1;
    }
};

struct Semqueue{
    // Node not in the queue, containing the process
    // that is currently using the critical section

    int using_cs_pid=-1;
    int next_using_cs_pid=-1;
    int blocked_by_lower_process=-1;
    int highest_priority=0;
    int normal_priority=0;
};


int main(int argc, char* argv[]) {
    
    if (argc < 7) {
        cout << "Usage: " << argv[0] << endl; 
        cout << "Average number  process arrivals," << endl; 
        cout << "Average process life," << endl; 
        cout << "Average duration of critical section," << endl; 
        cout << "Maximum  process number," << endl; 
        cout << "Probability of  critical section enter per time slot (k)," << endl; 
        cout << "Semaphore number (S)" << endl;
        return 1;
    }

    int Sem;
    float l1,l2,l3;
    int N;
    float  karg;
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
            printf( "%6.4lf \n", l3 );
        }

        if(i==4){
            N = stoi(argv[i]);
            //cout << N << endl;
        }

        if(i==5){
             karg = stof(argv[i]);
            printf( "%6.4lf \n", karg );
        }

        if(i==6){
            Sem = stoi(argv[i]);
            //cout << Sem << endl;
        }
    }

    unsigned long long int curr_slottime = 0;
    int next_process_arrival = 0;

    std::default_random_engine generator;
    std::exponential_distribution<double> distribution(l1); // gap 0.05
    std::default_random_engine generator2;
    std::exponential_distribution<double> distribution2(l2); // life  0.00005
    std::default_random_engine generator3;
    std::exponential_distribution<double> distribution3(l3); // cs  0.57

    std::random_device                  rand_dev;
	std::mt19937                        generator5(rand_dev());
    std::uniform_int_distribution<int>  distr2(0, 100);

    int process_life;
    typedef process* Process;
    struct Semqueue  sem_queues[Sem];
    
    Process* procedure_array = new Process[N];

    int processnum_curr = -1; // There should be a process with pid 0
    int counter=0;

    priority_queue<S> pq;

    //cout << endl;
    //int m=0;
    int timer=0;
    //int semdown; 
    int chosen_semnum;
    //int test2=0;
    int a;
    //int flag=1;
    //while(i<5)
    int current_prio;

    do {
        //m++;

        // Create new process and insert it into the priority queue
        if ( processnum_curr < (N - 1) &&   next_process_arrival == 0) { 

            processnum_curr++;
            process_life = ceil(distribution2(generator));

            // Process life should not be zero
            while (process_life == 0) {
                process_life = ceil(distribution2(generator));
            }

            next_process_arrival = ceil(distribution(generator2));
            // Minimum next time for a process arrival is the next timeslot
            while (next_process_arrival == 0) {
                next_process_arrival = ceil(distribution(generator2));
            }

            //cout << process_life << endl;
            //cout <<  next_process_arrival << endl;

            int block = 0;
            procedure_array[processnum_curr]  = new process(curr_slottime,processnum_curr, process_life,block);
            pq.push(S(procedure_array[processnum_curr]->getpriority(), timer , procedure_array[processnum_curr]->getpid()));
        }

// /*  xD

        // empty queue , enter semaphore
         if (!pq.empty()){
             
             //out of queue
              S s1 = pq.top();
              pq.pop();

              int pid =  s1.m_n3;
              ////cout << " HERE LIGES FORS " << processnum_curr  << endl;

              ////cout << pid << "lalalal" << endl;
              int using_semnum = procedure_array[pid]->getnum_semuse();

              // If the process is not using a critical section
               if (using_semnum == -1) {
                   // There is a chance that it wants to, so
                    // Generate a random double between 0 and 100

                    a =  distr2(generator5);
        
                    // If it wants to,
                    if (a>=karg) {
                        chosen_semnum = rand() % Sem;

                        procedure_array[pid]->set_num_semuse(chosen_semnum);
                        int crit_usage = ceil(distribution3(generator));

                        // crit_usg should be lower than the remaining life of the process 
                        while (crit_usage == 0 || crit_usage > procedure_array[pid]->get_remaininglife()) {
                            crit_usage = ceil(distribution3(generator));
                        }

                        procedure_array[pid]->set_rem_csusg(crit_usage);
                        //cout << "crit usg "  << crit_usage << endl;

                        // put to semaphore queue
                        //semdown = sem_down(curr_slottime,sem_queues[chosen_semnum],&procedure_array[pid],pid); 
                        ////cout << procedure_array[pid]->get_block() << " block sem " << pid << endl;
                        ////cout << " hello 2"  << endl;
                        current_prio = procedure_array[pid]->getpriority();
    
                        // if no other process is acessing the critical section
                        if (sem_queues[chosen_semnum].using_cs_pid == -1) {
                            ////cout << " hello 3"  << endl;
                            sem_queues[chosen_semnum].using_cs_pid = procedure_array[pid]->getpid();
                            sem_queues[chosen_semnum].highest_priority = current_prio;
                            sem_queues[chosen_semnum].normal_priority = current_prio;
                            procedure_array[pid]->set_block(1);//process is using critical section
                        }
                        else {
                            cout << " Process blocked in semaphore" << endl;
                            // 0-> ready , 1-> cs , 2-> blocked , 3-> dead xD2

                            procedure_array[pid]->set_block(2);//process got blocked
                            procedure_array[pid]->set_waiting_start(curr_slottime);


                            if(sem_queues[chosen_semnum].next_using_cs_pid == -1)
                                sem_queues[chosen_semnum].next_using_cs_pid=pid;
                            else if(sem_queues[chosen_semnum].next_using_cs_pid > -1 && procedure_array[pid]->getpriority() > procedure_array[sem_queues[chosen_semnum].next_using_cs_pid]->getpriority())
                                sem_queues[chosen_semnum].next_using_cs_pid=pid;

                              
                            // If its priority is higher than the previous highest
                            if (current_prio > sem_queues[chosen_semnum].highest_priority) {
                                // Change priority
                                sem_queues[chosen_semnum].highest_priority = current_prio;
                                //procedure_array[pid]->set_priority(sem_queues[chosen_semnum].highest_priority);
                            }
                        }
                    }// execute normally
                    else{
                        //cout << " hello 1"  << endl;
                        procedure_array[pid]->executep(curr_slottime+1);

                        // Check if its remaining life is zero
                        if ( procedure_array[pid]->get_remaininglife() == 0) {
                            procedure_array[pid]->set_block(3); // set process as dead
                        }else
                            pq.push(s1); //put it back in the priority queue to try enter a semaphore again
                    }
                }
         }


        //execute semaphore
        //reduce process life
        
        int pr_max = -1;
        int pidvark=-1;
        int pr_var=0;
        int block =0;
        int pr_sem=-1;


        //find process with greatest priority that is in cs
        for(int i=0;i<processnum_curr+1;i++){
            if(procedure_array[i]->get_block() == 1){
                block=1;
                pr_var = procedure_array[i]->getpriority();  
                if(pr_var > pr_max){
                    pr_max = pr_var;
                    pidvark = procedure_array[i]->getpid(); 
                    pr_sem  = procedure_array[i]->getnum_semuse(); 
                }
            }
        }      


        //int csusgv;
        //int remlife;
        int pr_totmax=-1;
        int pr_totpid=-1;

        // for the process with highest priority in cs
        if(block==1){

            //cout << "edw 4" << endl;
            procedure_array[pidvark]->executep(curr_slottime+1);
            int remlifecs = procedure_array[pidvark]->get_rem_csusg();

            if(remlifecs == 0){
                // perform sem up to the highest priority semaphore 

                //int pidvar = sem_queues[pr_sem].using_cs_pid;
                int nextpid = sem_queues[pr_sem].next_using_cs_pid;

            //eixa edw 1
                
                    // Only if process needs to die after the up 
                    if (procedure_array[pidvark]->get_remaininglife() == 0) {
                        procedure_array[pidvark]->set_priority(sem_queues[pr_sem].normal_priority);
                        procedure_array[pidvark]->set_block(3);//process dead dont join ready queue

                    }else{
                        if (sem_queues[pr_sem].normal_priority != sem_queues[pr_sem].highest_priority) {
                            // Return it to its normal priority
                            procedure_array[pidvark]->set_priority(sem_queues[pr_sem].normal_priority);
                        }

                        procedure_array[pidvark]->set_block(0);//process ready
                        procedure_array[pidvark]->set_num_semuse(-1);// not using semaphore
                        pq.push(S(procedure_array[pidvark]->getpriority(), timer , procedure_array[pidvark]->getpid())); // rejoin ready sem_queues[pr_sem]
                    }
                    
                    //procedure_array[pidvark]->calc_totaltime_waited(curr_slottime);
                    
                    // Take care of the next process accessing the critical section
                    //procedure_array[pid]->set_waiting_start(curr_slottime);
                    if (nextpid>-1) {
                         cout << "Process unblocked in semaphore" << endl;
                        //cout << "nextpid " << nextpid << " " << pr_sem <<endl;
                        sem_queues[pr_sem].using_cs_pid = procedure_array[nextpid]->getpid();
                        sem_queues[pr_sem].normal_priority = procedure_array[nextpid]->getpriority();
                        procedure_array[nextpid]->set_block(1);//using cs
                        procedure_array[nextpid]->calc_totaltime_waited(curr_slottime);
                        
                        procedure_array[nextpid]->set_waiting_start(curr_slottime);


                        //eixa edw

                        //unblock it
                        //if(found==1){
                            //
                            sem_queues[pr_sem].using_cs_pid = nextpid; // nextpid2
                            procedure_array[nextpid]->set_block(1); 
                            if(sem_queues[pr_sem].normal_priority> sem_queues[pr_sem].highest_priority){
                                sem_queues[pr_sem].highest_priority = sem_queues[pr_sem].normal_priority;
                            }
                            procedure_array[nextpid]->set_waiting_start(curr_slottime);
                            //sem_queues[pr_sem].using_cs_pid = nextpid;
                        //}
                        
                        //update next using pid
                            ////cout << "here10" << endl;
                            for(int i=0;i<processnum_curr+1;i++){
                                if(procedure_array[i]->get_block() == 2 && procedure_array[i]->getnum_semuse() == pr_sem){
                                        int pr_tempmax = procedure_array[i]->getpriority();
                                        int pr_tempmaxpid =  procedure_array[i]->getpid();
                                        if( pr_tempmax > pr_totmax ){
                                            pr_totmax = pr_tempmax;
                                            pr_totpid = pr_tempmaxpid;
                                        }
                                    }
                            }

                            sem_queues[pr_sem].next_using_cs_pid = pr_totpid;
                    }
                    else{
                        
                            sem_queues[pr_sem].highest_priority = 0;
                            sem_queues[pr_sem].normal_priority = 0;
                            sem_queues[pr_sem].next_using_cs_pid = -1;
                            sem_queues[pr_sem].using_cs_pid = -1;
                        
                    }
                        
                //}
                //cout << "edw 5" << endl;
            }
                ////cout << "edw 6" <<  pr_totpid <<  endl;
        }    
        //}
 


        //as one timeslot passes by , we increase the waiting time 
        // of all waiting processes in the queues
        for(int i=0;i< processnum_curr;i++){
            if(procedure_array[i]->get_block() == 2){ //blocked
                procedure_array[i]->calc_totaltime_waited(curr_slottime);

                int sem_nmvar =  procedure_array[i]->getnum_semuse();
                int pid_nmvar = sem_queues[sem_nmvar].using_cs_pid;
                int pidtest = sem_queues[pr_sem].using_cs_pid;

                if(procedure_array[i]->getpriority() > procedure_array[pid_nmvar]->getpriority() )
                    procedure_array[i]->set_slots_blocked();
                else if(procedure_array[i]->getnum_semuse()!=pr_sem && pidtest!=-1 && procedure_array[pidtest]->getpriority() < procedure_array[i]->getpriority())
                    procedure_array[i]->set_slots_blocked();
                else{}//nothing}    
                    
                

            }
            
            if(procedure_array[i]->get_block() == 1 && procedure_array[i]->getnum_semuse()!=pr_sem){ // cs but preempted
                int wait = procedure_array[i]->get_total_waited();
                wait++;
                //cout << "edw 7" <<  wait <<  endl;
                procedure_array[i]->set_totalwait(wait);
            }
        }

 // xD */

         // If a process was executed (the timeslot has passed)
        next_process_arrival -= 1;
        curr_slottime++;

        //int all_dead=0;
        int varc=0;
        counter = 0;

        ////cout << "here1" << endl;
        //if(processnum_curr == N-1){
            for(int i=0;i<processnum_curr+1;i++){
                varc= procedure_array[i]->get_block();
                if(varc==3){
                    ////cout << "here8" << endl;
                    counter++;
                }
            }
        //}

    }while ( processnum_curr < (N - 1) || counter < N); // all dead  && m < 5); //

    // for(int i=0;i<pq.size();i++){
    //     S s1 = pq.top();
    //     std::cout << s1.m_n1 << " "  << s1.m_n2 << " " << s1.m_n3 << " " << endl;
    //     pq.pop();
    // }

    cout << "edww mesa" << endl;
    unsigned long long int arr[8]={0,0,0,0,0,0,0,0},b;
    int arr2[8]={0,0,0,0,0,0,0,0};


    for(int i=0;i<N;i++){
        b = procedure_array[i]->getpriority();
        arr[b-1]+= procedure_array[i]->get_total_waited();
    }

    for(int i=0;i<N;i++){
        b = procedure_array[i]->getpriority(); 
        arr2[b-1]++;
    }

    cout << "\n\n\n\n" << endl;
    for(int i=0;i<N;i++){
        cout << "process "<< i << " Blocked by lower process " << procedure_array[i]->get_slots_blocked() << " slots" << endl;
    }

    cout << "\n\n\n\n" << endl;

    for(int i=0;i<8;i++){
        if(arr2[i]>0)
            cout << " priority " << i+1 <<" average waiting " << arr[i]/arr2[i] << endl;
        else   
            cout << " priority " << i+1 <<" average waiting " << arr[i] << endl; 
    }


//showpq(pq);

}

