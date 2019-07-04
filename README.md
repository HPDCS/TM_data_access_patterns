# STM Benchmark

This benchmark application uses TinySTM as TM library.

To compile and run the application run compile.sh. It compiles TinySTM library and the benchmark.

To execute the benchmark go in applications/stm_synch and run "app" executable

The usage for the benchmark is

`./app [number_of_threads] [dummy_time] [number_of_wait_cycles] [number_of_transaction_per_thread] [shared_memory_size] [number_of_test_repetitions]`

- **number_of_threads** is the number of thread the benchmark will spawn
- **dummy_time** is the time spent doing nothing (transaction padding time)
- **number_of_wait_cycles** (num_waits) is how many times a transaction has to wait dummy_time
- **number_of_transactions_per_thread** (num_steps) is how many transactions each thread executes
- **shared_memory_size** (num_blocks) indicates the size in double words (8 bytes) of the shared memory block accessed by the transactions. It should be a multiple of num_ops, since each instruction of a transaction accesses randomly an memory area in a sub-block that is 1/num_ops the original block.
- **number_of_test_repetition** (test_repetitions) indicates the times the test will run (in order to have good statistical data on transactions time and aborts number)
- **number_of_operations** (num_ops) is the number of operations per transactions.

To better visualize the result of the benchmark it is possible to copy and paste the output of the benchmark in a text file, save it and run `python3 results.py [file_name]`
It would print the time plot and then the aborts plot.


To disable Intel Hyperthreading you can use applications/stm_synch/stop_ht.sh (and start_ht.sh to restart it), modifying the logical processors number for your architecture (it is possible to see which are the HT logical cores with `grep -H . /sys/devices/system/cpu/cpu*/topology/thread_siblings_list`, that are the second number in each printed line; more information [here](https://www.golinuxhub.com/2018/01/how-to-disable-or-enable-hyper.html)) 
