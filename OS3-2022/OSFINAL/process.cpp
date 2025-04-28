#include <iostream>
#include "process.hpp"

using namespace std;

process::process(int curr_slottime,int p_num, int dur,int blk)
                : pid(p_num), remaining_life(dur),blocked(blk) {

    priority = (rand() % 8) ;
    priority+=1;
    rem_cs_usg = 0;
    total_waited=0;
    waiting_start = curr_slottime;
    num_semuse = -1;
    slots_blocked_by_lower_process = 0;

    cout << "process Num: " << pid << " created";
	cout << "  priority " << priority << " ";
    cout << "life: " << remaining_life << endl;
}

process::~process() {
//  cout << "Deleting the process " << pid << endl;
}

void process::set_block(int blk) {
    blocked = blk;
}

void process::set_rem_csusg(int csusg) {
    rem_cs_usg = csusg;
}

void process::set_priority(int pr) {
    priority = pr;
}

void process::set_slots_blocked() {
    slots_blocked_by_lower_process++;
}

void process::set_waiting_start(unsigned long long int curr_slottime) {
    waiting_start = curr_slottime;
}

void process::set_remlife(int rmf) {
    remaining_life = rmf;
}

void process::set_totalwait(int rmf) {
    total_waited=rmf;
}

void process::calc_totaltime_waited(unsigned long long int curr_slottime) {
    total_waited += curr_slottime - waiting_start;
}

void process::executep(int curr_slottime) {
    // If the process is not using a critical section decrement remaining life and cs use
    if (num_semuse != -1){
        remaining_life -= 1;
        rem_cs_usg -= 1;
    }// If the process is not using a critical section decrement remaining life
    else  {
        remaining_life -= 1;
    }

    // Update total waited slottimes for this process
    calc_totaltime_waited(curr_slottime);
    // It will start waiting from the next slottime
    waiting_start = curr_slottime + 1;
}

int process::getpid() {
    return pid;
}

int process::getpriority() {
    return priority;
}

int process::getnum_semuse() {
    return num_semuse;
}

int process::get_slots_blocked() {
    return slots_blocked_by_lower_process;
}

int process::get_remaininglife() {
    return remaining_life;
}

int process::get_rem_csusg() {
    return rem_cs_usg;
}

int process::get_total_waited() {
    return total_waited;
}

void process::set_num_semuse(int input) {
    num_semuse = input;
}

int process::get_block() {
    return blocked;
}



