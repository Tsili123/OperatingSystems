#pragma once
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

extern unsigned int N; // Number of messages to be sent from  
extern unsigned int K; // Number of K processes 
extern ifstream input_file; // Input file 

bool check_string(const char* string) ;
unsigned int get_lines_num(void); 
int cmd_handler(int argc, char *argv[]);
unsigned int get_line_maximum_length(void); 
void close_file(void); 