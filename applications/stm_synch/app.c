#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include "map.h"
#include "memory.h"
#include "random.h"
#include "thread.h"
#include "timer.h"
#include "tm.h"

#define NORM		0x7fffffff

unsigned pseed;
double dummy_sum;
int dummy_cycles=0;
long* vals;
long sum = 0;
int blocks = 0;
int total_threads = 0;

int num_ops = 0; //represents the number of operations per transaction. It is also used for dividing the blocks.

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
	int num_steps;
	int num_waits;
};


void run3 (void* argPtr) {

	struct data data = *((struct data *) argPtr);

	long* my_variables = calloc(blocks*num_ops, sizeof(long));
	int steps;
	int thread_number = thread_getId();
	int cell = 0;
	TM_THREAD_ENTER();
	for(steps = 0; steps < data.num_steps; steps++){
		//start transaction
		TM_BEGIN();
		int current_block = 0;

		int i_op;

		for(i_op = 0; i_op < num_ops; i_op++){

			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			if(random_number(&pseed) > 0.5)
				TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			else
				my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

		}
		
		TM_END(); //end transaction

		int i;
		sum = 0;
		for (i = 0; i < blocks*num_ops; i++){
			sum += my_variables[i];
		}

		int c;
		for (c = 0; c < data.num_waits; c++){
			spend_some_time();
		}
		
	}

    TM_THREAD_EXIT();
}

void run4 (void* argPtr) {

	struct data data = *((struct data *) argPtr);

	long* my_variables = calloc(blocks*num_ops, sizeof(long));
	int steps;
	int thread_number = thread_getId();
	int cell = 0;
	int current_block = 0;

	TM_THREAD_ENTER();

	for(steps = 0; steps < data.num_steps; steps++){
		
		//start transaction
		TM_BEGIN();
		
		if (thread_number >= total_threads/2)
			current_block = 0;
		else
			current_block = (num_ops-1)*blocks;

		int i_op;

		if (thread_number >= total_threads/2){		//first thread pool

			for(i_op = 0; i_op < num_ops; i_op++){

				spend_some_time();
			
				cell = (int)(random_number(&pseed)*blocks);
				if (cell > blocks-1)
					cell = blocks-1;
				cell += current_block;
				current_block+=blocks;

				if(random_number(&pseed) > 0.5)
					TM_SHARED_WRITE(vals[cell], my_variables[cell]);
				else
					my_variables[cell] = TM_SHARED_READ(vals[cell]);

				spend_some_time();

			}

		}
		else{										//second thread pool

			for(i_op = 0; i_op < num_ops; i_op++){

				spend_some_time();
			
				cell = (int)(random_number(&pseed)*blocks);
				if (cell > blocks-1)
					cell = blocks-1;
				cell += current_block;
				current_block-=blocks;

				if(random_number(&pseed) > 0.5)
					TM_SHARED_WRITE(vals[cell], my_variables[cell]);
				else
					my_variables[cell] = TM_SHARED_READ(vals[cell]);

				spend_some_time();

			}

		}
		
		TM_END(); //end transaction

		int i;
		sum = 0;
		for (i = 0; i < blocks*num_ops; i++){
			sum += my_variables[i];
		}
		int c;
		for (c = 0; c < data.num_waits; c++){
			spend_some_time();
		}
	}

    TM_THREAD_EXIT();
}




int main(int argc, char **argv)
{
    TIMER_T start, stop;

	if(argc<8) {
	    printf("\nYou did not feed me the correct number of arguments.");
		printf("\nUsage: %s [number_of_threads] [dummy_time] [num_waits] [num_steps] [num_blocks] [test_repetitions] [num_ops]\nBye bye :( ...\n", argv[0]);
	    exit(1);
	}  

	setenv("STM_STATS", "1", 1);

	int numThread = atoi(argv[1]);
	total_threads = numThread;
	dummy_cycles = atoi(argv[2]);
	int num_waits = atoi(argv[3]);
	int num_steps = atoi(argv[4]);
	
	int num_blocks = atoi(argv[5]);
	
	vals = calloc(num_blocks, sizeof(long));

	int repetitions = atoi(argv[6]);

	num_ops = atoi(argv[7]);

	if (num_ops == 0){
		printf("Error: num_ops can't be zero.\n");
		exit(-1);
	}
	else if(num_blocks % num_ops != 0){
		printf("Error: num_blocks has to be a multiple of num_ops\n");
		exit(-1);
	}

	blocks = num_blocks / num_ops;

	struct data data= {
		.num_steps = num_steps,
		.num_waits = num_waits,
	};

	int counter;
	
	printf("*************************");
	printf("\n Thread concurrency test, random reads/writes");
	printf("\n*************************\n");
	for (counter = 0; counter < repetitions; counter++){
		pseed=time(NULL);

		TM_STARTUP(numThread);
		
		thread_startup(numThread);

		TIMER_READ(start);
		//run all threads
		thread_start(run3, (void*)&data);
		TIMER_READ(stop);

		thread_shutdown();

		TM_SHUTDOWN();

		printf("\nThe elapsed time is %f seconds\n", TIMER_DIFF_SECONDS(start, stop));
	}
	printf("*************************");
	printf("\n Thread concurrency test, random reads/writes, thread pools");
	printf("\n*************************\n");
	for (counter = 0; counter < repetitions; counter++){
		pseed=time(NULL);

		TM_STARTUP(numThread);
		
		thread_startup(numThread);

		TIMER_READ(start);
		//run all threads
		thread_start(run4, (void*)&data);
		TIMER_READ(stop);

		thread_shutdown();

		TM_SHUTDOWN();

		printf("\nThe elapsed time is %f seconds\n", TIMER_DIFF_SECONDS(start, stop));
	}

    return 0;
}
