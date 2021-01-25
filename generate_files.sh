#!/bin/bash


echo "COMPILING ALL"
./compile.sh
echo "_____________________________"
echo "_____________________________"
echo "Executing the the tests"
rm output.csv
rm results.txt
./do-tests.sh >> results.txt
echo "_____________________________"
echo "_____________________________"
echo "Converting the results in CSV"
python3 tocsv.py results.txt $1
echo "_____________________________"
echo "_____________________________"
echo "DONE."
