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

pthread_spinlock_t lock;

unsigned pseed; //used by the RNG
double dummy_sum;
int dummy_cycles=0;
long* vals; //holds the values for the array
long sum = 0;
int block_size = 0;
int total_threads = 0;
volatile int running_threads = 0;

int num_ops_per_tran = 0; //represents the number of operations per transaction. It is also used for dividing the block_size
double read_probability = 0; //used for checking the random number which will decide a TM_SHARED_READ or TM_SHARED_WRITE

// random number generator [0,1)
double random_number(unsigned *pseed){
	unsigned   temp;
   	double     temp1;
   	do{
   		temp = (1220703125 * *pseed) & NORM;
   		*pseed = temp;
   		temp1 = temp;
   		temp1 *= 0.465613e-9;
   	} while(temp1 == 1);
   	return  temp1;
}

/*
Returns a random number between 0 and limit inclusive.
Credits https://stackoverflow.com/questions/2999075/generate-a-random-number-within-range/2999130#2999130
 */

int rand_lim(int limit) {

    int divisor = RAND_MAX/(limit+1);
    int retval;

    do { 
        retval = rand() / divisor;
    } while (retval > limit);

    return retval;

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
	int num_trans_per_thread;
	int num_waits;
};


void run3 (void* argPtr) {

	pthread_spin_lock(&lock);

	running_threads++;

	pthread_spin_unlock(&lock);

	struct data data = *((struct data *) argPtr);

	long* my_variables = calloc(block_size*num_ops_per_tran, sizeof(long));
	int steps;
	int thread_number = thread_getId();
	int item = 0;
	TM_THREAD_ENTER();
	for(steps = 0; steps < data.num_trans_per_thread; steps++){
		//start transaction
		TM_BEGIN();

		int i_op;

		for(i_op = 0; i_op < num_ops_per_tran; i_op++){

			spend_some_time();
			
			item = i_op * block_size + (int)(random_number(&pseed)*block_size);
			//printf("item: %d\n",item);

			if(random_number(&pseed) > read_probability)
				TM_SHARED_WRITE(vals[item], my_variables[item]);
			else
				my_variables[item] = TM_SHARED_READ(vals[item]);

			spend_some_time();

		}
		
		TM_END(); //end transaction

		int i;
		sum = 0;
		for (i = 0; i < block_size*num_ops_per_tran; i++){
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

	pthread_spin_lock(&lock);

	running_threads++;

	pthread_spin_unlock(&lock);

	struct data data = *((struct data *) argPtr);

	long* my_variables = calloc(block_size*num_ops_per_tran, sizeof(long));
	int steps;
	int thread_number = thread_getId();
	int item = 0;

	TM_THREAD_ENTER();

	for(steps = 0; steps < data.num_trans_per_thread; steps++){
		
		//start transaction
		TM_BEGIN();

		int i_op;

		if (thread_number >= total_threads/2){		//first thread pool

			for(i_op = 0; i_op < num_ops_per_tran; i_op++){

				spend_some_time();
			
				item = i_op * block_size + (int)(random_number(&pseed)*block_size);
				//printf("item: %d\n",item);

				if(random_number(&pseed) > read_probability)
					TM_SHARED_WRITE(vals[item], my_variables[item]);
				else
					my_variables[item] = TM_SHARED_READ(vals[item]);

				spend_some_time();

			}

		}
		else{										//second thread pool

			for(i_op = num_ops_per_tran-1; i_op >= 0; i_op--){

				spend_some_time();
			
				item = i_op * block_size + (int)(random_number(&pseed)*block_size);
				//printf("item: %d\n",item);

				if(random_number(&pseed) > read_probability)
					TM_SHARED_WRITE(vals[item], my_variables[item]);
				else
					my_variables[item] = TM_SHARED_READ(vals[item]);

				spend_some_time();

			}

		}
		
		TM_END(); //end transaction

		int i;
		sum = 0;
		for (i = 0; i < block_size*num_ops_per_tran; i++){
			sum += my_variables[i];
		}
		int c;
		for (c = 0; c < data.num_waits; c++){
			spend_some_time();
		}
	}

    TM_THREAD_EXIT();
}

//NOT USED - FOR TESTING ONLY - TOTALLY RANDOM FUNCTION
void run5 (void* argPtr){

	struct data data = *((struct data *) argPtr);

	long* my_variables = calloc(block_size*num_ops_per_tran, sizeof(long));
	int steps;
	int thread_number = thread_getId();
	int item = 0;
	TM_THREAD_ENTER();

	for(steps = 0; steps < data.num_trans_per_thread; steps++){
		//start transaction
		TM_BEGIN();
		int current_block = 0;

		int i_op;

		for(i_op = 0; i_op < num_ops_per_tran; i_op++){

			spend_some_time();
			
			item = rand_lim((block_size*num_ops_per_tran)-1);

			if(random_number(&pseed) > read_probability)
				TM_SHARED_WRITE(vals[item], my_variables[item]);
			else
				my_variables[item] = TM_SHARED_READ(vals[item]);

			spend_some_time();

		}
		
		TM_END(); //end transaction

		int i;
		sum = 0;
		for (i = 0; i < block_size*num_ops_per_tran; i++){
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

	if(argc<9) {
	    printf("\nYou did not feed me the correct number of arguments.");
		printf("\nUsage: %s [number_of_threads] [dummy_time] [num_waits] [num_trans] [num_items] [test_repetitions] [num_ops_per_tran] [read_probability]\nBye bye :( ...\n", argv[0]);
	    exit(1);
	}  

	setenv("STM_STATS", "1", 1);

	int numThread = atoi(argv[1]);
	total_threads = numThread;
	dummy_cycles = atoi(argv[2]);
	int num_waits = atoi(argv[3]);
	int num_trans = atoi(argv[4]);
	
	int num_items = atoi(argv[5]);
	
	vals = calloc(num_items, sizeof(long));

	int repetitions = atoi(argv[6]);

	num_ops_per_tran = atoi(argv[7]);

	if (num_ops_per_tran == 0){
		printf("Error: num_ops_per_tran can't be zero.\n");
		exit(-1);
	}
	else if(num_items % num_ops_per_tran != 0){
		printf("Error: num_items has to be a multiple of num_ops_per_tran\n");
		exit(-1);
	}

	block_size = num_items / num_ops_per_tran;

	read_probability = atof(argv[8]);

	pthread_spin_init(&lock,0);

	struct data data= {
		.num_trans_per_thread = num_trans/numThread,
		.num_waits = num_waits,
	};

	int counter, v;
	
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
		stm_get_global_stats("global_reset", &v);

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
		stm_get_global_stats("global_reset", &v);

		printf("\nThe elapsed time is %f seconds\n", TIMER_DIFF_SECONDS(start, stop));
	}
	
	printf("*************************");
	printf("\n Thread concurrency test, random reads/writes, different random accesses");
	printf("\n*************************\n");
	for (counter = 0; counter < repetitions; counter++){
		pseed=time(NULL);

		TM_STARTUP(numThread);
		
		thread_startup(numThread);

		TIMER_READ(start);
		//run all threads
		thread_start(run5, (void*)&data);
		TIMER_READ(stop);

		thread_shutdown();

		TM_SHUTDOWN();

		printf("\nThe elapsed time is %f seconds\n", TIMER_DIFF_SECONDS(start, stop));
	}

    return 0;
}
