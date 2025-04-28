#include <iostream>
#define MAX_SIZE 100

using namespace std;

class Queue {
    private:
        int  frontnum, procqueue[MAX_SIZE], rearnum,curr_size;
     
    public:
        Queue(){
            curr_size= 0;
            frontnum = -1;
            rearnum = -1;
        }
     

    int getSize(){
        return curr_size;
    }

    int getFront(){
        if(frontnum==-1)
            return -1;
        else
            return procqueue[frontnum];
    }

    bool isFull(){
        if(rearnum == MAX_SIZE - 1 && frontnum == 0){
            return true;
        }
        
        return false;
    }
        
    bool isEmpty(){
        if(frontnum == -1) 
            return true;
        else return false;
    }
     
    void inQueue(int val){
        if(isFull()){
            cout << endl<< "Queue is full of processes!!";
        }else {
            if(frontnum == -1) 
                frontnum = 0;
            rearnum++;
            procqueue[rearnum] = val;
            curr_size++;
            cout << val << " ";
        }
    }

    int outQueue(){
        int val;
        if(isEmpty()){
            cout << "Queue empty !" << endl; 
                return -1 ; 
        } 
        else { 
            val = procqueue[frontnum];   

            if(frontnum < rearnum){
                frontnum++;
            }else{
                rearnum = -1; //only one element in queue
                frontnum = -1;
            }

            cout <<  "\nDeleted -> " << val << " from procqueue";
            curr_size--;
            return val;
        }
    }
        
    void Queuedisplay(){

        int i;

        if(isEmpty()) {
            cout << "\nQueue is empty !" << endl;
        }else {
            cout << "\nElements of queue : ";
            for(i=frontnum; i<=rearnum; i++)
                cout << procqueue[i] << "   ";
                cout << "\nRear process = " << rearnum << endl;
            }
            cout << "\nFront process = " << frontnum;
        }
};