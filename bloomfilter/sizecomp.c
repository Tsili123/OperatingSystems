#include "sizecomp.h"



unsigned long long compute_size(unsigned int voters,float *fpp)
{
	
	unsigned long long bit_size=0,c;

	double bit = - ((voters * log(*fpp))/pow(log(2),2));
	
	c=round((bit));
	
	printf("%llu \n",c);
	return c;
}


unsigned long long	fix_bit_size(unsigned long long	bit_size)
{
	unsigned long long byte_size=bit_size/8;
	
	return byte_size+1;
}


//Fuction to find (x^y)%p 
unsigned int  modexp( unsigned long long x,int  y,unsigned int p) 
//Implementing modular exponentiation algorithm
//with right to left binary method and bitwise operators

//With this method we solve the overflow problem when we calculate modulo
{
	unsigned  int result=1;//initialize result
	x = x % p;     //initialize with modulo x
	while(y > 0)  //Examine y's bits until we check all of them(All 0s=0)
	{
		if(y & 1) //Checking the least-significant bit.If the current exponent
				 // is odd we multiply with current x.If it is even we don't.
				 // If y is odd, multiply x with result
		{
			result = (result * x) % p;
		}
		
		// n must be even now
		y = y>>1;  // y = y/2
		x = (x * x) % p; //building exponent avoiding continuous multiplications.
				        //(ex á^5-->á^10 without multiplying 5 times)
						//Shifting the bits of the index-exponent to get the
						//desired modulo of the x^y 

	}
	return result;
}
//Fuction miller-rabin  arguments-> d,current odd number,r
bool milrab(unsigned int  d,unsigned int  n,int  r)
{
	int i,a,j; //Declaring variables 
	unsigned long long  x;
	for(i=0; i<3;i++)
	{
		if(i==0)a=2; // á  can take values 2,7,61 which according to Miller-Rabin are enough 
		if(i==1)a=7; //to determine if the number is prime
		if(i==2)a=61;
		
		x= modexp( a, d, n); //x=a^d(mod n) [1]
		if(x==1||x==n-1)/*Miller-Rabin hypothesis is that for n to be prime either*/
	       continue; //a^d=1(mod n) [ or ] a^((2^r)*d)=-1(mod n)
		
		//if the first equation  holds(1 true) then the or is satisfied and we check another a
		//Note that for an OR to be false we need 2 false values
		//Go to the second part of the or if the first equation doesn't hold(1 false)
		for(j=1;j<r;j++)//j can take values  in range [1,r-1]
		{
			x=modexp(x, 2, n); //x=a^((2^r)*d)(mod n) [2]
			if(x==1)    
			  return false;    //If x never becomes n-1 then n is not prime 
			if(x==n-1)      //Note that if  only for one specific a the equation doesn't hold 
			  break;         //then  a condition returns that the number is composite
		}				//If only for a pair a,j the equation holds then the number is propably prime and either the external loop for a's continues or we get the prime.
		  if(x==n-1)
		  continue;
		  else
		  return false; // None of the steps made x equal n-1.We need to check every possible r for that.
	}
	return true; //If the number pass the miller-rabin set of equations then it's prime.We need to check every possible a for that.
}
//Fuction that prepares values for miller-rabin set of equations
bool isPrime_Propab(unsigned int  n)
{
	if (n<=1||n==4) return false; //Base cases
	if (n<=3) return true;
	
	unsigned int  d ; //Declaring values
	int r=0;
	
	if(n & 1)  // Checking number if only it is odd.Even numbers can't be primes
	{
		d=n-1;  //Making the [odd] number [even]
		while(!(d&1))//Dividing d with 2 until it becomes odd(d % 2 == 0)
	    {
			d = d >> 1;      //We know every even number can be written in the form
	 		r+= 1;        // (2^r)*d  
	    }
         if (!milrab(d, n, r)) //Calling miller-rabin fuction
            return false;   //If it returns false,then the number is not prime.
		else//So Propabilistic fuctions exit with false.
			return true;
			//If it returns true then it's prime
	}
	else
	return false;//If number is even for sure its not prime
}


unsigned long long compute_prime(unsigned long long	max)
{
	unsigned long long  num,prime;
	for(num=0;num<=max;num++) //Searching for primes in the given range of numbers
	{							//using miller-rabin propabilistic fuction
		if(isPrime_Propab(num))
		{
			prime=num;
		}
	}
	return prime;
}
/*
int main()
{
	unsigned long long num1=compute_size(2000000,0.001);
	printf("%ld\n",num1);
	unsigned long long num2=num1-6582144;//longest known prime gap
	unsigned long long size=compute_prime(num2,num1);
	printf("%ld\n",size);
	unsigned long long bytes=fix_bit_size(size);
	printf("%ld\n",bytes);
	return 0;
}
//*/
