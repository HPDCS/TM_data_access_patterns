#!/bin/bash

cd tinySTM
make clean
make
cd ..
cd applications/synthetic_bench
make -f Makefile.stm clean
make -f Makefile.stm






