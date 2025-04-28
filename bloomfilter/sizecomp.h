#ifndef __sizecomp__
#define __sizecomp__
#include<math.h>
#include<stdbool.h>
#include <stdio.h>

unsigned long long compute_size(unsigned int voters,float *fpp);
unsigned long long	fix_bit_size(unsigned long long	bit_size);
unsigned int  modexp( unsigned long long x,int  y,unsigned int p);
bool milrab(unsigned int  d,unsigned int  n,int  r);
bool isPrime_Propab(unsigned int  n);
unsigned long long compute_prime(unsigned long long	max);


#endif
