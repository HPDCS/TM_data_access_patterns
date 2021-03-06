#!/bin/bash

cd applications/stm_synch
#THREADS_ARRAY=(2 4 6 8 10 12 14 16 18 20 22 24 26 28 30 32)
THREADS_ARRAY=(1 2 3 4)
ITEMS_ARRAY=(20 30 50 70 90)
OPS_ARRAY=(2 5 10)
READPROB_ARRAY=(1 0.8 0.6 0.4 0.2 0)
DUMMY_TIME=80
NUM_CYCLES=80
TRANS=4000
NUM_TESTS=1

		for THREADS in ${THREADS_ARRAY[@]}
		do	
			for ITEMS in ${ITEMS_ARRAY[@]}
			do
				for OPS in ${OPS_ARRAY[@]}
				do
					for R_PROBS in ${READPROB_ARRAY[@]}
					do
						echo " "
						echo "---- THREADS $THREADS ITEMS $ITEMS OPS $OPS TRANS $TRANS PROB $R_PROBS ----"
						echo " "
						./app $THREADS $DUMMY_TIME $NUM_CYCLES $TRANS $ITEMS $NUM_TESTS $OPS $R_PROBS;
					done
				done
			done
		done
