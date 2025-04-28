#ifndef __element__
#define __element__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//globals
int counter;

typedef struct data {
        /*  members for airport data in array */
        
        char *id;
        char *name;
        char *surname;
        int age;
        char gender;
        int pc;
        
} Elem_t;

char * readline(FILE *fp);

void lineByline(FILE * file,Elem_t *Elem);

int countlines(FILE * file);

void parse_line(Elem_t *Elem,char *buffer);

void parse_line_routes(char *buffer,int *source_id,int *dest_id);

void destruct_copies(int max_routes,Elem_t *Elem);

#endif
