#!/bin/bash

cd applications/stm_synch
REPETITIONS_ARRAY=(0 5 10 15 20 25 30 35 40 41 42 43 44 45 50 55 60 65 70)


		for I in ${REPETITIONS_ARRAY[@]}
		do
			let OPS_PT=(OPS*ITEMS)/10
			echo " "
			echo "---- THREADS 1 ITEMS 10 OPS 5 TRANS 1000 PROB 0.5 ----"
			echo " "
			./app 1 $I 0 1000 10 1 5 0.5;
		done
