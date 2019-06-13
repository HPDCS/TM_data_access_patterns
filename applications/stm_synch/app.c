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

int shared_int=0;
unsigned pseed;
double dummy_sum;
int dummy_cycles=0;
int another_shared_int=0;
long* vals;
long sum = 0;
int blocks = 0;
int total_threads = 0;

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

	long* my_variables = calloc(blocks*4, sizeof(long));
	int steps;
	
	int cell = 0;
	TM_THREAD_ENTER();
	for(steps = 0; steps < data.num_steps; steps++){
		
		//start transaction
		TM_BEGIN();
		int current_block = 0;
		if (data.rw == 1){
			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();
		}
		else if (data.rw == 2){
			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();
		}
		else if (data.rw == 3){
			spend_some_time();

			cell = (random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();
		}
		else{
			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();
		}
		TM_END(); //end transaction

		int i;
		sum = 0;
		for (i = 0; i < blocks*4; i++){
			sum += my_variables[i];
		}
		spend_some_time();
		/*spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();*/
	}

    TM_THREAD_EXIT();
}


void run2 (void* argPtr) {

	struct data data = *((struct data *) argPtr);

	long* my_variables = calloc(blocks*4, sizeof(long));
	int steps;
	int thread_number = thread_getId();
	//printf("%d\n", thread_number);
	int cell = 0;
	TM_THREAD_ENTER();
	for(steps = 0; steps < data.num_steps; steps++){
		int current_block;
		
		
		//printf("thread_number %d, current_block %d\n", thread_number, current_block);
		//start transaction
		TM_BEGIN();
		if (thread_number < total_threads/2)
			current_block = 0;
		else
			current_block = 3*blocks;
		
		if (data.rw == 1 && thread_number < total_threads/2){
			
			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();
		}

		else if (data.rw == 1 && thread_number >= total_threads/2){
			
			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;
			//printf("Instruction 1, thread_number %d, cell %d\n", thread_number, cell);

			my_variables[cell] = TM_SHARED_READ(vals[cell]);
			
			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;
			//printf("Instruction 2, thread_number %d, cell %d\n", thread_number, cell);

			my_variables[cell] = TM_SHARED_READ(vals[cell]);
			
			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;
			//printf("Instruction 3, thread_number %d, cell %d\n", thread_number, cell);

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			
			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;
			//printf("Instruction 4, thread_number %d, cell %d\n", thread_number, cell);

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();
		}

		else if (data.rw = 2 && thread_number < total_threads/2){

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);
			
			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);
			
			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			
			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();
		}

		else if (data.rw == 2 && thread_number >= total_threads/2){

			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			//TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			//TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			//my_variables[cell] = TM_SHARED_READ(vals[cell]);
			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			//my_variables[cell] = TM_SHARED_READ(vals[cell]);
			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();
		}
		
		else if (data.rw == 3 && thread_number < total_threads/2){

			spend_some_time();

			cell = (random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();
		}

		else if (data.rw == 3 && thread_number >= total_threads/2){

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			
			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);
			
			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();
		}
		
		else if (data.rw == 4 && thread_number < total_threads/2){

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			
			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);
			
			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();
		}

		else if (data.rw == 4 && thread_number >= total_threads/2){

			spend_some_time();

			cell = (random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			TM_SHARED_WRITE(vals[cell], my_variables[cell]);

			spend_some_time();
		}
		
		TM_END(); //end transaction

		int i;
		sum = 0;
		for (i = 0; i < blocks*4; i++){
			sum += my_variables[i];
		}
		spend_some_time();
		/*spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();*/
	}

    TM_THREAD_EXIT();
}


void run3 (void* argPtr) {

	struct data data = *((struct data *) argPtr);

	long* my_variables = calloc(blocks*4, sizeof(long));
	int steps;
	int thread_number = thread_getId();
	int cell = 0;
	TM_THREAD_ENTER();
	for(steps = 0; steps < data.num_steps; steps++){
		//start transaction
		TM_BEGIN();
		int current_block = 0;
		
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

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			if (random_number(&pseed) > 0.5)
				TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			else
				my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			if (random_number(&pseed) > 0.5)
				TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			else
				my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			if (random_number(&pseed) > 0.5)
				TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			else
				my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();
		
		TM_END(); //end transaction

		int i;
		sum = 0;
		for (i = 0; i < blocks*4; i++){
			sum += my_variables[i];
		}
		spend_some_time();
		/*spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();*/
	}

    TM_THREAD_EXIT();
}

void run4 (void* argPtr) {

	struct data data = *((struct data *) argPtr);

	long* my_variables = calloc(blocks*4, sizeof(long));
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
			current_block = 3*blocks;
		if (thread_number >= total_threads/2){
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

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			if (random_number(&pseed) > 0.5)
				TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			else
				my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			if (random_number(&pseed) > 0.5)
				TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			else
				my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block+=blocks;

			if (random_number(&pseed) > 0.5)
				TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			else
				my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();
		}
		else{
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

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			if (random_number(&pseed) > 0.5)
				TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			else
				my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();
			
			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			if (random_number(&pseed) > 0.5)
				TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			else
				my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();

			cell = (int)(random_number(&pseed)*blocks);
			if (cell > blocks-1)
				cell = blocks-1;
			cell += current_block;
			current_block-=blocks;

			if (random_number(&pseed) > 0.5)
				TM_SHARED_WRITE(vals[cell], my_variables[cell]);
			else
				my_variables[cell] = TM_SHARED_READ(vals[cell]);

			spend_some_time();
		}
		TM_END(); //end transaction

		int i;
		sum = 0;
		for (i = 0; i < blocks*4; i++){
			sum += my_variables[i];
		}
		spend_some_time();
		/*spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();
		spend_some_time();*/
	}

    TM_THREAD_EXIT();
}




int main(int argc, char **argv)
{
    	TIMER_T start, stop;

	if(argc<=4) {
	        printf("\nYou did not feed me the correct number of arguments.");
		printf("\nUsage: program_name number_of_threads dummy_time num_steps num_blocks reverse\nBye bye :( ...\n");
	        exit(1);
	     }  
	int numThread = atoi(argv[1]);
	total_threads = numThread;
	dummy_cycles = atoi(argv[2]);
	int num_steps = atoi(argv[3]);
	//printf("\nThe initial value of shared_int was %i ",shared_int);
	setenv("STM_STATS", "1", 1);
	int num_blocks = atoi(argv[4]);
	blocks = num_blocks / 4;
	vals = calloc(num_blocks, sizeof(long));
	int rw = 2;	//3
	int reverse = atoi(argv[5]);

	struct data data= {
		.rw = rw,
		.num_steps = num_steps,
	};

	

	int counter;
	if (reverse == 0){
		printf("*************************");
		printf("\n Thread concurrency test, reads before writes");
		printf("\n*************************\n");
		for (counter = 0; counter < 15; counter++){
			pseed=time(NULL);

			//initialize spinlock

			TM_STARTUP(numThread);
			
			thread_startup(numThread);

			TIMER_READ(start);
			//run all threads
			thread_start(run, (void*)&data);
			TIMER_READ(stop);

			TM_SHUTDOWN();

			thread_shutdown();

			//printf("\nThe final value of shared_int is %i (expected: %i)", shared_int, numThread);
			//printf("\nThe final value of dummy_sum is %f", dummy_sum);
			printf("\nThe elapsed time is %f seconds\n", TIMER_DIFF_SECONDS(start, stop));
		}

		//again but with writes before reads
		data.rw = 4;	//2

		printf("*************************");
		printf("\n Thread concurrency test, writes before reads");
		printf("\n*************************\n");

		for (counter = 0; counter < 15; counter++){
			pseed=time(NULL);
			
			TM_STARTUP(numThread);
			thread_startup(numThread);

			TIMER_READ(start);
			//run all threads
			thread_start(run, (void*)&data);
			TIMER_READ(stop);

			TM_SHUTDOWN();

			thread_shutdown();

			//printf("\nThe final value of shared_int is %i (expected: %i)", shared_int, numThread);
			//printf("\nThe final value of dummy_sum is %f", dummy_sum);
			printf("\nThe elapsed time is %f seconds\n", TIMER_DIFF_SECONDS(start, stop));
		}
	}
	else if (reverse == 1){
		printf("*************************");
		printf("\n Thread concurrency test, reads before writes, reverse order");
		printf("\n*************************\n");
		for (counter = 0; counter < 4; counter++){
			pseed=time(NULL);

			//initialize spinlock

			TM_STARTUP(numThread);
			
			thread_startup(numThread);

			TIMER_READ(start);
			//run all threads
			thread_start(run2, (void*)&data);
			TIMER_READ(stop);

			

			thread_shutdown();
			TM_SHUTDOWN();

			//printf("\nThe final value of shared_int is %i (expected: %i)", shared_int, numThread);
			//printf("\nThe final value of dummy_sum is %f", dummy_sum);
			printf("\nThe elapsed time is %f seconds\n", TIMER_DIFF_SECONDS(start, stop));
		}

		//again but with writes before reads
		data.rw = 2;	//4

		printf("*************************");
		printf("\n Thread concurrency test, writes before reads, reverse order");
		printf("\n*************************\n");

		for (counter = 0; counter < 4; counter++){
			pseed=time(NULL);
			
			TM_STARTUP(numThread);
			thread_startup(numThread);

			TIMER_READ(start);
			//run all threads
			thread_start(run, (void*)&data);
			TIMER_READ(stop);

			

			thread_shutdown();
			TM_SHUTDOWN();

			//printf("\nThe final value of shared_int is %i (expected: %i)", shared_int, numThread);
			//printf("\nThe final value of dummy_sum is %f", dummy_sum);
			printf("\nThe elapsed time is %f seconds\n", TIMER_DIFF_SECONDS(start, stop));
		}
	}
	else{
		printf("*************************");
		printf("\n Thread concurrency test, reads before writes, random order");
		printf("\n*************************\n");
		for (counter = 0; counter < 40; counter++){
			pseed=time(NULL);

			//initialize spinlock

			TM_STARTUP(numThread);
			
			thread_startup(numThread);

			TIMER_READ(start);
			//run all threads
			thread_start(run3, (void*)&data);
			TIMER_READ(stop);

			

			thread_shutdown();
			TM_SHUTDOWN();

			//printf("\nThe final value of shared_int is %i (expected: %i)", shared_int, numThread);
			//printf("\nThe final value of dummy_sum is %f", dummy_sum);
			printf("\nThe elapsed time is %f seconds\n", TIMER_DIFF_SECONDS(start, stop));
		}
		printf("*************************");
		printf("\n Thread concurrency test, reads before writes, random order, thread pools");
		printf("\n*************************\n");
		for (counter = 0; counter < 40; counter++){
			pseed=time(NULL);

			//initialize spinlock

			TM_STARTUP(numThread);
			
			thread_startup(numThread);

			TIMER_READ(start);
			//run all threads
			thread_start(run4, (void*)&data);
			TIMER_READ(stop);

			

			thread_shutdown();
			TM_SHUTDOWN();

			//printf("\nThe final value of shared_int is %i (expected: %i)", shared_int, numThread);
			//printf("\nThe final value of dummy_sum is %f", dummy_sum);
			printf("\nThe elapsed time is %f seconds\n", TIMER_DIFF_SECONDS(start, stop));
		}
	}

    return 0;
}




/*int main(int argc, char **argv)
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

    	TM_STARTUP(numThread);
  
    	thread_startup(numThread);

    	TIMER_READ(start);
	//run all threads
    	thread_start(run, (void*)NULL);

    	TIMER_READ(stop);
   
    	TM_SHUTDOWN();

    	thread_shutdown();

	printf("\nThe final value of shared_int is %i (expected: %i)", shared_int, numThread);
	printf("\nThe final value of dummy_sum is %f", dummy_sum);
    	printf("\nThreads: %i\tThe elapsed time is %f seconds\n", numThread, TIMER_DIFF_SECONDS(start, stop));


    return 0;
}*/
