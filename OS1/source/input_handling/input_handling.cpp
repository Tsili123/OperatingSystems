#include "input_handling.hpp"

bool check_string(const char* string) {
  const int string_len = strlen(string);
  for(int i = 0; i < string_len; ++i) {
    if(!isdigit(string[i])) 
      return false;
  }
  return true;
}

int cmd_handler(int argc, char *argv[]){
    
    short int curr_arg = 1;
    bool N_cmd = 0;    
    bool K_cmd = 0;
    bool txt_cmd = 0;

    while( curr_arg < argc ){
        if( strcmp(argv[curr_arg],"-n") == 0 ){ // Check for number of messages (N) 
            if( curr_arg == (argc - 1) || check_string(argv[curr_arg+1]) ==  false ){
                cout << "[ERROR] Please specify the number of messages. (N)" << endl;
                return -1;
            }else{
                N = atoi(argv[curr_arg+1]);
                N_cmd = 1;
            }
        }else if( strcmp(argv[curr_arg],"-k") == 0 ){ // Check for number of processes (K) 
            if( curr_arg == (argc - 1) || check_string(argv[curr_arg+1]) ==  false){
                cout << "[ERROR]Please specify a number of P processes. (K)" << endl;
                return -1;
            }else{    
                K = atoi(argv[curr_arg+1]);
                K_cmd = 1;
            }
        }else if( strcmp(argv[curr_arg],"-x") == 0 ){ // Check for input file name 
            if( curr_arg == (argc - 1) || check_string(argv[curr_arg+1]) == true ||  strcmp(argv[curr_arg+1],"-k") == 0 || strcmp(argv[curr_arg+1],"-n") == 0 || strcmp(argv[curr_arg+1],"-x") == 0 ){                
                cout << "[ERROR] Please specify an input file." << endl;
                return -1;
            }else{
                input_file.open(argv[curr_arg+1]);
                txt_cmd = 1;
                if(!input_file){
                    cout << "[ERROR]Cannot open the specified file." << endl;
                    return -1;
                }
            }            
        }else{
            cout << "[ERROR] Invalid Arguments. The avalaiable arguments are -: n , -k , -x" << endl;
            return -1;
        }
        curr_arg = curr_arg + 2;
    }

    if( K_cmd == 1 && N_cmd == 1 && txt_cmd == 1 ) 
        return 0;
    else{
        cout << "[ERROR]Missing Arguments." << endl;
        return -1;
    } 
}

void close_file(void){
    // Closes input_file
    input_file.close(); 
}

unsigned int get_lines_num(void){
    // Retruns the number of lines in the file 

    input_file.clear();
    input_file.seekg(0,ios::beg);
    string str_var;
    unsigned int lines_num = 0;

    while( getline(input_file,str_var) ) {
        lines_num++;
    }
    input_file.clear();
    input_file.seekg(0,ios::beg);

    return lines_num;
}

unsigned int get_line_maximum_length(void){
    //Returns max length of line
    input_file.clear();
    input_file.seekg(0,ios::beg);
    unsigned int max_length = 0;
    string str_var;

    while( getline(input_file,str_var) ){
        cout << str_var ;
        if( str_var.length() > max_length ) {
            max_length = str_var.length();
        }
        str_var.clear();
    }

    input_file.clear();
    input_file.seekg(0,ios::beg);
    return max_length;
} 
