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
long vals[10] = {0};
long sum = 0;

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

struct data{
	int rw;
	int num_steps;
};

void run (void* argPtr) {

	struct data data = *((struct data *) argPtr);

	long my_variables[10] = {0};
	int steps;

	for(steps = 0; steps < data.num_steps; steps++){
	//start transaction
	__transaction_atomic {	//remark: export ITM_DEFAULT_METHOD=ml_wt

		/*spend_some_time();
	
		//read a shared variable

		vals[1]=shared_int;

		//simulating some computation..
		spend_some_time();

		vals[1]++;		
		
		spend_some_time();

		//update the shared variable
		shared_int=vals[2];

		spend_some_time();
		
		vals[2]++;

		spend_some_time();*/
		if ( data.rw == 1){
			spend_some_time();
			vals[3] = my_variables[3];
			spend_some_time();
			vals[4] = my_variables[4];
			spend_some_time();
			my_variables[1] = vals[1];
			spend_some_time();
			my_variables[2] = vals[2];
			spend_some_time();
		}
		else if (data.rw == 2){
			spend_some_time();
			my_variables[1] = vals[1];
			spend_some_time();
			my_variables[2] = vals[2];
			spend_some_time();
			vals[3] = my_variables[3];
			spend_some_time();
			vals[4] = my_variables[4];
			spend_some_time();
		}
		else if (data.rw == 3){
			spend_some_time();
			my_variables[1] = vals[1];
			spend_some_time();
			my_variables[2] = vals[2];
			spend_some_time();
			my_variables[3] = vals[3];
			spend_some_time();
			vals[4] = my_variables[4];
			spend_some_time();
		}
		else{
			spend_some_time();
			vals[4] = my_variables[4];
			spend_some_time();
			my_variables[1] = vals[1];
			spend_some_time();
			my_variables[2] = vals[2];
			spend_some_time();
			my_variables[3] = vals[3];
			spend_some_time();
		}
		

	} //end transaction

	int i;
	sum = 0;
	for (i = 0; i < 10; i++){
		sum += my_variables[i];
	}
	//printf("%ld\n", sum);
	}
}


int main(int argc, char **argv)
{
    	TIMER_T start, stop;

	if(argc<=3) {
	        printf("\nYou did not feed me the correct number of arguments.");
		printf("\nUsage: program_name number_of_threads dummy_time num_steps global_lock_flag\nBye bye :( ...\n");
	        exit(1);
	     }  
	int numThread = atoi(argv[1]);
	dummy_cycles = atoi(argv[2]);
	int num_steps = atoi(argv[3]);
	//printf("\nThe initial value of shared_int was %i ",shared_int);
	int global_lock = atoi(argv[4]);
	if (global_lock == 1)
		setenv("ITM_DEFAULT_METHOD", "gl_wt", 1);
	else
		setenv("ITM_DEFAULT_METHOD", "ml_wt", 1);
	int rw = 3;	//1

	struct data data= {
		.rw = rw,
		.num_steps = num_steps,
	};

	printf("*************************");
	printf("\n Thread concurrency test, reads before writes, %s", getenv("ITM_DEFAULT_METHOD"));
	printf("\n*************************\n");

	int counter;
	for (counter = 0; counter < 15; counter++){
		pseed=time(NULL);

		//initialize spinlock
		pthread_spin_init(&spinlock, 0);
		
		thread_startup(numThread);

		TIMER_READ(start);
		//run all threads
		thread_start(run, (void*)&data);
		TIMER_READ(stop);

		thread_shutdown();

		//printf("\nThe final value of shared_int is %i (expected: %i)", shared_int, numThread);
		//printf("\nThe final value of dummy_sum is %f", dummy_sum);
		printf("The elapsed time is %f seconds\n", TIMER_DIFF_SECONDS(start, stop));
	}

	//again but with writes before reads
	data.rw = 4;	//2

	printf("*************************");
	printf("\n Thread concurrency test, writes before reads, %s", getenv("ITM_DEFAULT_METHOD"));
	printf("\n*************************\n");

	for (counter = 0; counter < 15; counter++){
		pseed=time(NULL);

		//initialize spinlock
		pthread_spin_init(&spinlock, 0);
		
		thread_startup(numThread);

		TIMER_READ(start);
		//run all threads
		thread_start(run, (void*)&data);
		TIMER_READ(stop);

		thread_shutdown();

		//printf("\nThe final value of shared_int is %i (expected: %i)", shared_int, numThread);
		//printf("\nThe final value of dummy_sum is %f", dummy_sum);
		printf("The elapsed time is %f seconds\n", TIMER_DIFF_SECONDS(start, stop));
	}

    return 0;
}


