#include "source/semaphore_operators/semaphore_operators.hpp"
#include "source/shared_memory_operators/shared_memory_operators.hpp"
#include "source/input_handling/input_handling.hpp"
#include <string>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

unsigned int N=0; // Number of messages to be sent   
unsigned int K=0; // Number of P processes
unsigned int input_file_lines=0; //  Total lines of  input file 
ifstream input_file; // Input file 
unsigned int line_max_length=0; // Line with max length of input file 

int main(int argc, char *argv[]){
    
    // Cmd handling 
    int input_check = cmd_handler(argc,argv);
    
    if(input_check == -1 ){
        cout << "The main 1 program will now exit." << endl;
        return -1;
    }

    // Get the number of lines the input file has, will be used as upper bound to random line number generator 
    input_file_lines = get_lines_num();

    // That integer , increased by sizeof(int) will be the size of ds-in 
    line_max_length = get_line_maximum_length();

    cout << "max line length , line num: " << line_max_length  << " " << input_file_lines << endl;

    // Initialization of shared memory 
    int ds_out_id = shared_memory_initialize(2 * sizeof(int) + N * (line_max_length + sizeof(int))); // ds_out shared segment

    int ds_in_id = shared_memory_initialize( 2 * sizeof(int) + N * (sizeof(int) + sizeof(int))); // ds_in shared segment 

    // Secure pointers to shared memory segments 
    char *ds_in,*ds_out; 

    ds_out = get_shared_memory_ptr(ds_out_id);
    ds_in = get_shared_memory_ptr(ds_in_id);

    // Intiialize ds_in shared memory 
    const int zero_const = 0;

    // Intiialize ds_out shared memory 
    memcpy(ds_out,&zero_const,sizeof(int));  
    memcpy(ds_out+sizeof(int),&zero_const,sizeof(int));

    memcpy(ds_in,&zero_const,sizeof(int));
    memcpy(ds_in+sizeof(int),&zero_const,sizeof(int));

    // Semaphore Initialization 

    // ds_out 
    int ds_out_mutex = semaphore_create();
    if( semaphore_1_initialize(ds_out_mutex) == -1 ) 
        return -1;

    // ds_in 
    int ds_in_mutex = semaphore_create();
    if( semaphore_1_initialize(ds_in_mutex) == -1 ) 
        return -1;

    // Now lets fork the C process 
    bool C_process = 0;
    pid_t * child_pids = (pid_t *)malloc((K+1)*sizeof(pid_t));
    int pid_main = getpid();
    child_pids[K] = fork();
    int same_pid = 0;

    if( getpid() != pid_main ){

        // Only C processes enter this section 
        std::cout << "[MESSAGE] Forked a C process with pid: " << getpid() << endl;
        same_pid = -1;

        while(1){

            C_process = 1;

            // Can i receive another message? 
            semaphore_P(ds_out_mutex);

            int messg_sent;
            memcpy(&messg_sent,ds_out,sizeof(int));

            semaphore_V(ds_out_mutex);

            if( messg_sent == N ) 
                break;

            // Compare values of messg_read and messg_sent to find out if all sent messages have been received 
            int messg_read_ds_in,messg_sent_ds_in;

            // Preparations to receive message from P processes 
            int pid_sender;

            char * str_var = (char *) malloc(line_max_length);
            memset(str_var,'\0',line_max_length);

            while(1){
                //no busy waiting
                semaphore_P(ds_in_mutex);

                memcpy(&messg_sent_ds_in,ds_in,sizeof(int));
                memcpy(&messg_read_ds_in,ds_in+sizeof(int),sizeof(int));

                if( messg_sent_ds_in > messg_read_ds_in ) 
                    break;

                semaphore_V(ds_in_mutex);
            }

            int line_num;
            memcpy(&pid_sender,ds_in + 2*sizeof(int) + messg_read_ds_in * (sizeof(int) + sizeof(int)  ) ,sizeof(int));
            memcpy(&line_num,ds_in + 3*sizeof(int) + messg_read_ds_in * (sizeof(int) + sizeof(int)  )  ,sizeof(int) );
            //std::cout << "[MESSAGE]C process (" << getpid() << ") received message from " << pid_sender << " line num "<< line_num << endl;
            messg_read_ds_in++;
            memcpy(ds_in+sizeof(int),&messg_read_ds_in,sizeof(int));

            semaphore_V(ds_in_mutex);

            // Send desired line from file as a message 
            string text_line;
            input_file.clear();
            input_file.seekg(0,ios::beg);
            
            //reach the desired line of the file
            for (unsigned int i = 0; i < line_num; ++i){
                if(std::getline(input_file,text_line)){
                    //ignore line
                }else{
                    break;
                }
            }
            strcpy(str_var,text_line.c_str());
            // Send Response to all P processes using ds_out 
            semaphore_P(ds_out_mutex);

            //reset shared memory segment from previous message
            memcpy(ds_out + 2 * sizeof(int) + messg_sent * (sizeof(int) + line_max_length ),&pid_sender,sizeof(int));
            memcpy(ds_out + 3 * sizeof(int) + messg_sent * (sizeof(int) + line_max_length ),str_var,strlen(str_var));
            messg_sent++;

            memcpy(ds_out,&messg_sent,sizeof(int));
            
            semaphore_V(ds_out_mutex);

            // Free dynamically allocated memory 
            free(str_var);
            text_line.clear();
        }
    }else{

        // Lets fork the P processes (K of them) 
        int i;
        for(i=0;i<K;i++){
            child_pids[i] = fork();
            if( getpid() != pid_main ) 
                break;
        }
    }



    if( C_process == 0  && getpid() != pid_main ){

        // Only P processes enter this section 
        std::cout << "[MESSAGE] Forked a P process with pid: " << getpid() << endl;
       
        while(1){

            // Can i send a message? Lets check the messg_count shared memory and find out 
            semaphore_P(ds_in_mutex);
    
            int messg_sent;
            memcpy(&messg_sent,ds_in,sizeof(int));

            if( messg_sent >= N ){
                semaphore_V(ds_in_mutex);
                break;
            } 

            // Message Preparations - get pid and get a random line from input text 
            string str_var;
            input_file.clear();
            input_file.seekg(0,ios::beg);
            std::getline(input_file,str_var);
            int p_pid = getpid();

            time_t t;
            srand((unsigned)time(&t) ^ getpid());
            int seed = rand() % input_file_lines;
            //cout << seed << endl;

            // Send the message 
            memcpy(ds_in + 2 * sizeof(int) + messg_sent * ( sizeof(int) + sizeof(int)),&p_pid,sizeof(int));
            memcpy(ds_in + 3 * sizeof(int) + messg_sent * ( sizeof(int) +  sizeof(int)),&seed,sizeof(int));
            messg_sent++;
            memcpy(ds_in,&messg_sent,sizeof(int));

            semaphore_V(ds_in_mutex);

            // Wait for C Process Response from ds_out 
            char * received_str = (char *) malloc(line_max_length);
            memset(received_str,'\0',line_max_length);
            int messg_pid;
            int prev_same_pid = same_pid;
            int messgs_sent_ds_out;
               
            while( same_pid == prev_same_pid ){
                sleep(1);
                semaphore_P(ds_out_mutex);

                memcpy(&messgs_sent_ds_out,ds_out,sizeof(int));

                for(int j=0;j<messgs_sent_ds_out;j++){
                    
                    memcpy(&messg_pid, ds_out + 2*sizeof(int) + j * (sizeof(int) + line_max_length),sizeof(int) );
                    memcpy(received_str,ds_out + 3*sizeof(int) + j * (sizeof(int) + line_max_length),line_max_length);

                    // Is the message in ds_out sent by this process? 
                    if(  messg_pid == getpid() && messg_pid != 0){ 
                        same_pid++;
                        //reset 
                        memcpy(ds_out + 2*sizeof(int) + j * (sizeof(int) + line_max_length),&zero_const,sizeof(int));
                        //memcpy(ds_out + 3*sizeof(int) + j * (sizeof(int) + line_max_length),&zero_const,line_max_length);
                        semaphore_V(ds_out_mutex);
                        break;
                    }else{ 
                        // Turns out its not so restore semaphore state 
                        if( messg_pid != 0 ) 
                            std::cout << "[MESSAGE] Process with pid " << getpid() << " found a message from " << messg_pid << " in out-ds: " << received_str << endl;
                    }
                    
                }
            
                //semaphore_V(ds_out_mutex);
            }
            std::cout << "[~SUCCESS~]same_pid happened: " << received_str << " (" << messg_pid << ")" << endl;

            // Free dynamically allocated Memory 
            str_var.clear();
            free(received_str);
        }
    }


    if( getpid() == pid_main ){

        // Wait for all children processes to finish executing 
        int status=0;
        unsigned int total_same_pids = 0;
        // Wait for C process to exit 
        waitpid(child_pids[K],&status,0);
 
        // Wait for all the P processes to exit 
        for(int j=0;j<K;j++){
            waitpid(child_pids[j],&status,0);

            if( WIFEXITED(status) ){
                int returned = WEXITSTATUS(status);
                total_same_pids = total_same_pids + returned;
            }
        }


        // Print statistics 
        cout << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~STATISTICS~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "Number of P Processes: " << K << " PIDs:{";
        for(int j=0;j<K;j++) 
            cout << " " << child_pids[j] << " ";
        cout << "}" << endl;
        cout << "Total Number of messages sent: " << N << endl;
        cout << "Total number of same_pides: " << total_same_pids << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

        cout << "\n\n";

        // Clear shared Memory Segments 
        shared_memory_delete(ds_in_id);
        shared_memory_delete(ds_out_id);

        // Clear Semaphores 
        // ds_out 
        semaphore_delete(ds_out_mutex);
        // ds_in 
        semaphore_delete(ds_in_mutex);
        // Close Input File 
        close_file();
    }

    // Deallocate Memory 
    free(child_pids);   
    cout << "[MESSAGE]Process with pid " << getpid() << " and same_pid " << same_pid << " just exited." << endl;

    return same_pid;
}