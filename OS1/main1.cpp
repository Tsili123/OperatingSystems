#include "source/semaphore_operators/semaphore_operators.hpp"
#include "source/shared_memory_operators/shared_memory_operators.hpp"
#include "source/input_handling/input_handling.hpp"
#include <string.h>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>

using namespace std;

unsigned int N=0; // Number of messages to be sent   
unsigned int K=0; // Number of P processes
unsigned int input_file_lines=0; //  Total lines of  input file 
ifstream input_file; // Input file 
unsigned int line_max_length=0; // Line with max length of input file 

int main(int argc, char *argv[]){
    
    // Cmd handling 
    int input_check = cmd_handler(argc,argv);
    
    if( input_check == -1 ){
        cout << "The main 1 program will now exit." << endl;
        return -1;
    }

    // Get the number of lines the input file has, will be used as upper bound to random line number generator 
    input_file_lines = get_lines_num();

    // That integer , increased by sizeof(int) will be the size of ds-in 
    line_max_length = get_line_maximum_length();

    cout << "max line length , line num: " << line_max_length  << " " << input_file_lines << endl;

    // Initialization of shared memory 
    int count_messg_id = shared_memory_initialize(sizeof(int)); // count_messg shared segment
    int ds_out_id = shared_memory_initialize(sizeof(int) + line_max_length  ); // ds_out shared segment
    int ds_in_id = shared_memory_initialize(sizeof(int) + sizeof(int) ); // ds_in shared segment 

    // Secure pointers to shared memory segments 
    char *count_messg,*ds_in,*ds_out; 

    count_messg = get_shared_memory_ptr(count_messg_id);
    ds_in = get_shared_memory_ptr(ds_in_id);
    ds_out = get_shared_memory_ptr(ds_out_id);

    // Initialize count_messg shared memory 
    memcpy(count_messg,&N,sizeof(int));

    // Semaphore Initialization 

    // count_messg 
    int count_messg_mutex = semaphore_create();
    if( semaphore_1_initialize(count_messg_mutex) == -1 ) 
        return -1;

    // ds_in 
    int ds_in_full = semaphore_create();
    if( semaphore_0_initialize(ds_in_full) == -1 ) 
        return -1;
    int ds_in_empty = semaphore_create();
    if( semaphore_1_initialize(ds_in_empty) == -1 ) 
        return -1;
    int ds_in_mutex = semaphore_create();
    if( semaphore_1_initialize(ds_in_mutex) == -1 ) 
        return -1;

    // ds_out 
    int ds_out_full = semaphore_create();
    if( semaphore_0_initialize(ds_out_full) == -1 ) 
        return -1;
    int ds_out_empty = semaphore_create();
    if( semaphore_1_initialize(ds_out_empty) == -1 ) 
        return -1;
    int ds_out_mutex = semaphore_create();
    if( semaphore_1_initialize(ds_out_mutex) == -1 ) 
        return -1;

    // Seed random number generator 
    srand(time(NULL));

    // Now lets fork the C process 
    bool C_process = 0;
    pid_t * child_pids = (pid_t *)malloc((K+1)*sizeof(pid_t));
    int pid_main = getpid();
    child_pids[K] = fork();
    int same_pid = 0;

    if( getpid() != pid_main ){

        // Only C processes enter this section 

        int msg_to_receive = N;
        same_pid = -1;

        cout << "[MESSAGE] Forked one C process with pid: " << getpid() << endl;
        while(1){
            C_process = 1;
            // Can i receive another message? 
            if( msg_to_receive <= 0 ) 
                break;

            // Preparations to receive message from P processes 
            int pid_sender;
            int line_num;

            char * str_var = (char *) malloc(line_max_length);

            // Using semaphores to make sure that we receive each P processes's message
            
            semaphore_P(ds_in_full);
            semaphore_P(ds_in_mutex);

            memcpy(&pid_sender,ds_in,sizeof(int));
            memcpy(&line_num,ds_in+sizeof(int),sizeof(int));
            //cout << "[MESSAGE]C process (" << getpid() << ") received ruquest from " << pid_sender <<" for line " << *line_num << " of file" <<"." << endl;

            semaphore_V(ds_in_mutex);
            semaphore_V(ds_in_empty);


            // Send desired line from file as a message 
            string text_line;
            input_file.clear();
            input_file.seekg(0,ios::beg);
            
            //reach the desired line of the file
            for (unsigned int i = 0; i < line_num; ++i){
                if (std::getline(input_file,text_line)){
                    //ignore line
                }else{
                    break;
                }
            }
            strcpy(str_var,text_line.c_str());
            //cout << "C PROCESS : " << str_var << " to pid " << pid_sender << " " << line_num << endl;

            // Send Response to all of the P processes using ds_out 
            semaphore_P(ds_out_empty);
            semaphore_P(ds_out_mutex);

            //reset shared memory segment from previous message
            //memset(ds_out+sizeof(int),'\0',line_max_length);

            //send pid of the process that requested one line from
            //useful for error checking
            memcpy(ds_out,&pid_sender,sizeof(int));
            memcpy(ds_out+sizeof(int),str_var,strlen(str_var));

            semaphore_V(ds_out_mutex);
            semaphore_V(ds_out_full);

            // Free dynamically allocated memory 
            free(str_var);
            text_line.clear();
            
            // Mark another message as received
            msg_to_receive--;
        }
    }else{
            // Lets fork the P processes ( K of them)
            for(int i=0;i<K;i++){
                child_pids[i] = fork();
                if( getpid() != pid_main ) {
                    break;
                }
            }
    }


    if( getpid() != pid_main && C_process == 0 ){
        int count_loop = 0;
        // Only P process enter the section
        cout << "[MESSAGE] Forked one P process with pid: " << getpid() << endl;
    
        while(1){

            // Can i send a message? Lets check the count_messg shared memory and find out
            bool exit = 0;
            semaphore_P(count_messg_mutex);

            int left_messg=0;
            memcpy(&left_messg,count_messg,sizeof(int));

            if( left_messg == 0 ){
                exit = 1;
            }
            else{
                left_messg--;
                memcpy(count_messg,&left_messg,sizeof(int));
            }

            semaphore_V(count_messg_mutex);
            
            if( exit == 1 ) 
                break; // We have sent K messages - time to go! 

            // Message Preparations - get pid and get a random line from input text 
            string str_var;
            input_file.clear();
            input_file.seekg(0,ios::beg);
            std::getline(input_file,str_var);
            int p_pid = getpid();

            int seed = rand() % input_file_lines;

            // Use seamphores to sync and make sure every P process gets their message sent 
            semaphore_P(ds_in_empty);
            semaphore_P(ds_in_mutex);

            memcpy(ds_in+sizeof(int),&seed,sizeof(int));
            memcpy(ds_in,&p_pid,sizeof(int));

            semaphore_V(ds_in_mutex);
            semaphore_V(ds_in_full);

            // Wait for C Process Response from ds_out 
            char * received_str = (char *) malloc(line_max_length);
            //memset(received_str,'\0',line_max_length);
            int msg_pid;
            int prev_same_pid = same_pid;

            while( same_pid == prev_same_pid ){

                semaphore_P(ds_out_full);          
                semaphore_P(ds_out_mutex);

                memcpy(&msg_pid,ds_out,sizeof(int));
                memcpy(received_str,ds_out+sizeof(int),line_max_length);

                // Is the message in ds_out a message sent by this process? 
                if( msg_pid == getpid() ){ 
                    same_pid++;
                    semaphore_V(ds_out_mutex);
                    semaphore_V(ds_out_empty);

                }else{ 
                    // Turns out its not, restore semaphore state 
                    cout << "[MESSAGE] Process with pid " << getpid() << " found a message from " << msg_pid << " in out-ds: " << received_str << endl;
                    semaphore_V(ds_out_mutex);
                    semaphore_V(ds_out_full);
                }
            }

            cout << "[~SUCCESS~] same_pid happened: " << received_str << " (" << msg_pid << ")" << endl;
            // Free dynamically allocated Memory 
            std::free(received_str);
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
        cout << "~~~~~~~~~~~~~~~~~~~~~~~STATISTICS~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "Number of P Processes: " << K << " PIDs:{";
        for(int j=0;j<K;j++) 
            cout << " " << child_pids[j] << " ";
        cout << "}" << endl;
        cout << "Number of total messages sent: " << N << endl;
        cout << "Number of total same_pids: " << total_same_pids << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;

        cout << "\n\n";

        // count_messg 
        semaphore_delete(count_messg_mutex);

        // Clear shared Memory Segments
        shared_memory_delete(ds_out_id); 
        shared_memory_delete(ds_in_id);
        shared_memory_delete(count_messg_id);

        // Clear Semaphores 
        // ds_out 
        semaphore_delete(ds_out_full);
        semaphore_delete(ds_out_empty);
        semaphore_delete(ds_out_mutex);

        // ds_in 
        semaphore_delete(ds_in_full); 
        semaphore_delete(ds_in_empty);
        semaphore_delete(ds_in_mutex);

        // Close Input File 
        close_file();

    }

    // Deallocate Memory 
    free(child_pids);   
    cout << "[MESSAGE] Process with pid " << getpid() << " and same_pid " << same_pid << " just exited." << endl;

    return same_pid;
}