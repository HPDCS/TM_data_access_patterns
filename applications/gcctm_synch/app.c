#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "thread.h"
#include "timer.h"

#define NORM		0x7fffffff

int shared_int=0;
unsigned pseed;
double dummy_sum;
int dummy_cycles=0;
pthread_spinlock_t spinlock;

// random number generator between 0 and 1
double random_number(unsigned *pseed){
   	unsigned   temp;
   	double     temp1;
   	temp = (1220703125 * *pseed) & NORM;
   	*pseed = temp;
   	temp1 = temp;
   	temp1 *= 0.465613e-9;
   	return  temp1;

}

//a function for spending some time
void spend_some_time(){
	int cycles=0;
	while(cycles<dummy_cycles) {	
		dummy_sum+=random_number(&pseed);
	 	cycles++;
	}
}


void run (void* argPtr) {

	int val;

	//start transaction
	__transaction_atomic {	//remark: export ITM_DEFAULT_METHOD=ml_wt
	
	//read a shared variable
	val=shared_int;

	//simulating some computation..
	spend_some_time();

	val++;		

	//update the shared variable
	shared_int=val;

	} //end transaction

}


int main(int argc, char **argv)
{
    	TIMER_T start, stop;

	printf("*************************");
	printf("\n Thread concurrency test ");
	printf("\n*************************");

	if(argc<=1) {
	        printf("\nYou did not feed me the correct number of arguments.");
		printf("\nUsage: program_name number_of_threads dummy_time\nBye bye :( ...\n");
	        exit(1);
	     }  
	int numThread = atoi(argv[1]);
	dummy_cycles = atoi(argv[2]);
	printf("\nThe initial value of shared_int was %i ",shared_int);
	pseed=time(NULL);

	//initialize spinlock
	pthread_spin_init(&spinlock, 0);
	
    	thread_startup(numThread);

    	TIMER_READ(start);
	//run all threads
	thread_start(run, (void*)NULL);
    	TIMER_READ(stop);
   
    	thread_shutdown();

	printf("\nThe final value of shared_int is %i (expected: %i)", shared_int, numThread);
	printf("\nThe final value of dummy_sum is %f", dummy_sum);
    	printf("\nThreads: %i\tThe elapsed time is %f seconds\n", numThread, TIMER_DIFF_SECONDS(start, stop));


    return 0;
}


