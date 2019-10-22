# STM Benchmark

This benchmark application uses TinySTM as TM library.

To compile and run the application run compile.sh. It compiles TinySTM library and the benchmark.

To execute the benchmark go in applications/stm_synch and run "app" executable

The usage for the benchmark is

`./app [number_of_threads] [dummy_time] [num_waits] [num_trans] [num_items] [test_repetitions] [num_ops_per_tran] [read_probability]`

- **number_of_threads** is the number of thread the benchmark will spawn
- **dummy_time** is the time spent doing nothing (transaction padding time)
- **number_of_wait_cycles** (num_waits) is how many times a transaction has to wait dummy_time
- **number_of_transactions** (num_trans) is how many transactions are executed
- **shared_memory_size** (num_items) indicates the size in double words (8 bytes) of the shared memory block accessed by the transactions. It should be a multiple of num_ops, since each instruction of a transaction accesses randomly an memory area in a sub-block that is 1/num_ops the original block.
- **number_of_test_repetition** (test_repetitions) indicates the times the test will run (in order to have good statistical data on transactions time and aborts number)
- **number_of_operations** (num_ops_per_tran) is the number of operations per transactions.
- **probability** (read_probability) used for checking the random number which will decide a TM_SHARED_READ or TM_SHARED_WRITE.

Given those, parameters, the test will execute three types of runs:
- **sequential run**: all the threads execute transactions from the first to the last element of the array, in a sequential way.
- **mixed run**: half of the threads exectutes transactions from the first to the last element of the array; the other half executes transactions the other way around.
- **random run**: all the threads execute transactions accessing random array elements. 
The test will register the commits, the aborts and the time for each run. 

The script `do_tests.sh` executes a set of tests spawning up to 8 threads and doing for each of them an increasing number of operations to a vector of a increasing size and a decreasing read probability. In particular, 10 tests of this kind:
- **number_of_threads** assumes the following values: 1,2,3,4,5,6,7,8
- **dummy_time** is set to be 45
- **num_waits** is set to be 45
- **num_trans** is set to be 4000
- **num_items** assumes the following values: 10,30,50,70,90
- **num_ops_per_trans** assumes the following values: 2,5,10
- **read_probability** assumes the following values: 1,0.8,0.6,0.4,0.3,0.2,0

To better visualize the result of the benchmark it is possible to execute the script `./generate_files.sh [output_file_name].csv`
It will first compile all the benchmark, then it will execute `do_tests.sh`, saving the results in a txt file, subsequently passed to a python script that turns it into a CSV table. 
You can show a graphical view of the data with:
`python3 plot.py [output_file_name] [test_repetitions] [number_of_threads] [num_items] [num_ops_per_trans] [read_probability]`

Two graphics will appear: one showing the overall time on the y-axis, the other showing the abort rate on the y-axis. On the x-axis, the independent variable will be set putting an `X` at the position of the parameter we would see in the graphic. For example:
`./generate_files.sh ETL.csv`
`python3 plot.py ETL 10 X 10 5 0.8`

It will show the graphic with the data printed on the file **ETL.csv**, whith **10 test repetitions**, **10 array items**, **5 operations per transaction** and **80% of read operation**; the dependent variable will be the **number of threads** and the independent variables will be the **time** and the **abort rate**. 
The graphic will be composed by three lines, one for the data concerning the **sequential run**, one for the data concerning the **mixed run** and one for the data concerning the **random run**.

To disable Intel Hyperthreading you can use applications/stm_synch/stop_ht.sh (and start_ht.sh to restart it), modifying the logical processors number for your architecture (it is possible to see which are the HT logical cores with `grep -H . /sys/devices/system/cpu/cpu*/topology/thread_siblings_list`, that are the second number in each printed line; more information [here](https://www.golinuxhub.com/2018/01/how-to-disable-or-enable-hyper.html)) 
