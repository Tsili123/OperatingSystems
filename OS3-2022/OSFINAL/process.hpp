#ifndef PROCESSFLAG
#define PROCESSFLAG

class process {
private:
    int pid;
    int priority;
    int remaining_life;

    int num_semuse; // -1 means critical section not used
    int rem_cs_usg;
    unsigned long long int total_waited;
    unsigned long long int waiting_start;
    int blocked;
    int slots_blocked_by_lower_process;
public:
    process(int, int, int,int);
    ~process();

    
    int getpid();
    int getpriority();
    int get_remaininglife();
    int get_rem_csusg();
    int get_total_waited();
    int getnum_semuse();
    int get_block();
    int get_slots_blocked();
    
    void set_slots_blocked();
    void set_block(int);
    void set_num_semuse(int);
    void set_rem_csusg(int);
    void set_waiting_start(unsigned long long int);
    void calc_totaltime_waited(unsigned long long int);
    void executep(int);
    void set_priority(int pr);
    void set_remlife(int rmf);
    void set_totalwait(int rmf);
    
};

#endif
