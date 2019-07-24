# STM Benchmark

This benchmark application uses TinySTM as TM library.

To compile and run the application run compile.sh. It compiles TinySTM library and the benchmark.

To execute the benchmark go in applications/stm_synch and run "app" executable

The usage for the benchmark is

`./app [number_of_threads] [dummy_time] [num_waits] [num_trans] [num_items] [test_repetitions] [num_ops_per_tran] [read_probability]`

- **number_of_threads** is the number of thread the benchmark will spawn
- **dummy_time** is the time spent doing nothing (transaction padding time)
- **number_of_wait_cycles** (num_waits) is how many times a transaction has to wait dummy_time
- **number_of_transactions_per_thread** (num_trans) is how many transactions each thread executes
- **shared_memory_size** (num_items) indicates the size in double words (8 bytes) of the shared memory block accessed by the transactions. It should be a multiple of num_ops, since each instruction of a transaction accesses randomly an memory area in a sub-block that is 1/num_ops the original block.
- **number_of_test_repetition** (test_repetitions) indicates the times the test will run (in order to have good statistical data on transactions time and aborts number)
- **number_of_operations** (num_ops_per_tran) is the number of operations per transactions.
- **probability** (read_probability) used for checking the random number which will decide a TM_SHARED_READ or TM_SHARED_WRITE.

The script `run_tests.sh` executes a set of tests spawning up to 32 threads and doing for each of them an increasing number of operations to a vector of a increasing size.

To better visualize the result of the benchmark it is possible to copy and paste the output of the benchmark in a text file, save it and run `python3 results.py [file_name]`
It would print the time plot and then the aborts plot.

To disable Intel Hyperthreading you can use applications/stm_synch/stop_ht.sh (and start_ht.sh to restart it), modifying the logical processors number for your architecture (it is possible to see which are the HT logical cores with `grep -H . /sys/devices/system/cpu/cpu*/topology/thread_siblings_list`, that are the second number in each printed line; more information [here](https://www.golinuxhub.com/2018/01/how-to-disable-or-enable-hyper.html)) 
