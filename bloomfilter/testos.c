#include <stdio.h>
#include <stdlib.h>

void * myfunction ( int flag , void * alpha ){
static int mylocal =3;
int aa [10];
if ( flag ==0) {
printf (" return the decrared table ( address of) !\n");
return aa;
}
else if ( flag ==1) {
printf (" change the 2nd function param !\n");
return alpha ;
}
else if ( flag ==2) {
int * myint = malloc ( sizeof ( int )) ;
* myint =1245;
printf (" return the locally declared pointer !\n") ;
return myint ;
}
else {
printf (" return value of mylocal variable !\n");
return & mylocal ;
}
}
int main ( int argc , char * argv []) {
int *ii =( int *) malloc ( sizeof ( int ));
float *yy;
ii= myfunction ( atoi ( argv [1]) , yy);
return 1;
}
