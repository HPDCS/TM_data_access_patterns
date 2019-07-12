#!/bin/bash

cd applications/stm_synch
THREADS=2
OPS_ARRAY=(10 50 100 500 1000)
while [  $THREADS -le 32 ]; do
	for OPS in ${OPS_ARRAY[@]};
		do
			let ITEMS=$OPS*4
			echo ""
			echo "----THREADS $THREADS ITEMS $ITEMS OPS $OPS----"
			echo ""
			./app $THREADS 2000 2000 10 $ITEMS 10 $OPS 0.5;
		done
	let THREADS=THREADS+2 
done