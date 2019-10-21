import sys
import matplotlib.pyplot as plt

seq_threads = []
seq_items = []
seq_ops = []
seq_commits = []
seq_aborts = []
seq_time = []
seq_prob = []

mix_threads = []
mix_items = []
mix_ops = []
mix_commits = []
mix_aborts = []
mix_time = []
mix_prob = []

ran_threads = []
ran_items = []
ran_ops = []
ran_commits = []
ran_aborts = []
ran_time = []
ran_prob = []

threads_values = [1,2,3,4,5,6,7,8]
items_values = [10,100]
ops_values = [0.2,0.5]
readp_values = [0.8,0.5]


file_to_open = sys.argv[1]
tests = sys.argv[2]
thread = sys.argv[3]
item = sys.argv[4]
operation = sys.argv[5]
prob = sys.argv[6]
file = open(str(file_to_open)+".csv", 'r', encoding = "utf-8")


def variable_prob():
    xline_seq=[]
    xline_mix=[]
    xline_ran=[]
    yline1_seq=[]
    yline1_mix=[]
    yline1_ran=[]
    yline2_seq=[]
    yline2_mix=[]
    yline2_ran=[]
    for i in range(1,len(seq_threads)):
        if(int(seq_threads[i])==int(thread) and int(seq_items[i])==int(item) and int(seq_ops[i])==int(operation)):
            yline1_seq.append(seq_time[i]) 
            yline2_seq.append(float(int(seq_aborts[i])/(int(seq_aborts[i])+int(seq_commits[i]))))
            xline_seq.append(seq_prob[i])
        if(int(mix_threads[i])==int(thread) and int(mix_items[i])==int(item) and int(mix_ops[i])==int(operation)):
            yline1_mix.append(mix_time[i])
            yline2_mix.append(float(int(mix_aborts[i])/(int(mix_aborts[i])+int(mix_commits[i]))))
            xline_mix.append(mix_prob[i])
        if(int(ran_threads[i])==int(thread) and int(ran_items[i])==int(item) and int(ran_ops[i])==int(operation)):
            yline1_ran.append(ran_time[i]) 
            yline2_ran.append(float(int(ran_aborts[i])/(int(ran_aborts[i])+int(ran_commits[i]))))
            xline_ran.append(ran_prob[i])
    plt.figure(str(file_to_open)+" Variable Probability",figsize=(12,6))
    plt.subplot(1, 2, 1)
    plt.plot(xline_seq,yline1_seq,c='blue',label="sequential")
    plt.plot(xline_mix,yline1_mix,c='green',label="mixed")
    plt.plot(xline_ran,yline1_ran,c='red',label="random")
    plt.ylabel("Time")
    plt.xlabel("Read probability")
    plt.subplot(1, 2, 2)
    plt.plot(xline_seq,yline2_seq,c='blue',label="sequential")
    plt.plot(xline_mix,yline2_mix,c='green',label="mixed")
    plt.plot(xline_ran,yline2_ran,c='red',label="random")
    plt.legend(loc='upper left',ncol=3, bbox_to_anchor=(-0.75, 1.12))
    plt.ylabel("Abort rate")
    plt.xlabel("Read probability")
    plt.show()

def variable_threads():
    xline_seq=[]
    xline_mix=[]
    xline_ran=[]
    yline1_seq=[]
    yline1_mix=[]
    yline1_ran=[]
    yline2_seq=[]
    yline2_mix=[]
    yline2_ran=[]
    for i in range(1,len(seq_threads)):
        if(float(seq_prob[i])==float(prob) and int(seq_items[i])==int(item) and int(seq_ops[i])==int(operation)):
            yline1_seq.append(seq_time[i]) 
            yline2_seq.append(float(int(seq_aborts[i])/(int(seq_aborts[i])+int(seq_commits[i]))))
            xline_seq.append(seq_threads[i])
        if(float(mix_prob[i])==float(prob) and int(mix_items[i])==int(item) and int(mix_ops[i])==int(operation)):
            yline1_mix.append(mix_time[i])
            yline2_mix.append(float(int(mix_aborts[i])/(int(mix_aborts[i])+int(mix_commits[i]))))
            xline_mix.append(mix_threads[i])
        if(float(ran_prob[i])==float(prob) and int(ran_items[i])==int(item) and int(ran_ops[i])==int(operation)):
            yline1_ran.append(ran_time[i]) 
            yline2_ran.append(float(int(ran_aborts[i])/(int(ran_aborts[i])+int(ran_commits[i]))))
            xline_ran.append(ran_threads[i])
    plt.figure(str(file_to_open)+" Variable Threads",figsize=(12,6))
    plt.subplot(1, 2, 1)
    plt.plot(xline_seq,yline1_seq,c='blue',label="sequential")
    plt.plot(xline_mix,yline1_mix,c='green',label="mixed")
    plt.plot(xline_ran,yline1_ran,c='red',label="random")
    plt.ylabel("Time")
    plt.xlabel("Threads")
    plt.subplot(1, 2, 2)
    plt.plot(xline_seq,yline2_seq,c='blue',label="sequential")
    plt.plot(xline_mix,yline2_mix,c='green',label="mixed")
    plt.plot(xline_ran,yline2_ran,c='red',label="random")
    plt.ylabel("Abort rate")
    plt.xlabel("Threads")
    plt.legend(loc='upper left',ncol=3, bbox_to_anchor=(-0.75, 1.12))
    plt.show()

def variable_items():
    xline_seq=[]
    xline_mix=[]
    xline_ran=[]
    yline1_seq=[]
    yline1_mix=[]
    yline1_ran=[]
    yline2_seq=[]
    yline2_mix=[]
    yline2_ran=[]
    for i in range(1,len(seq_threads)):
        if(float(seq_prob[i])==float(prob) and int(seq_threads[i])==int(thread) and int(seq_ops[i])==int(operation)):
            yline1_seq.append(seq_time[i]) 
            yline2_seq.append(float(int(seq_aborts[i])/(int(seq_aborts[i])+int(seq_commits[i]))))
            xline_seq.append(seq_items[i])
        if(float(mix_prob[i])==float(prob) and int(mix_threads[i])==int(thread) and int(mix_ops[i])==int(operation)):
            yline1_mix.append(mix_time[i])
            yline2_mix.append(float(int(mix_aborts[i])/(int(mix_aborts[i])+int(mix_commits[i]))))
            xline_mix.append(mix_items[i])
        if(float(ran_prob[i])==float(prob) and int(ran_threads[i])==int(thread) and int(ran_ops[i])==int(operation)):
            yline1_ran.append(ran_time[i]) 
            yline2_ran.append(float(int(ran_aborts[i])/(int(ran_aborts[i])+int(ran_commits[i]))))
            xline_ran.append(ran_items[i])
    plt.figure(str(file_to_open)+" Variable Items",figsize=(12,6))
    plt.subplot(1, 2, 1)
    plt.plot(xline_seq,yline1_seq,c='blue',label="sequential")
    plt.plot(xline_mix,yline1_mix,c='green',label="mixed")
    plt.plot(xline_ran,yline1_ran,c='red',label="random")
    plt.ylabel("Time")
    plt.xlabel("Items")
    plt.subplot(1, 2, 2)
    plt.plot(xline_seq,yline2_seq,c='blue',label="sequential")
    plt.plot(xline_mix,yline2_mix,c='green',label="mixed")
    plt.plot(xline_ran,yline2_ran,c='red',label="random")
    plt.ylabel("Abort rate")
    plt.xlabel("Items")
    plt.legend(loc='upper left',ncol=3, bbox_to_anchor=(-0.75, 1.12))
    plt.show()

def variable_ops():
    xline_seq=[]
    xline_mix=[]
    xline_ran=[]
    yline1_seq=[]
    yline1_mix=[]
    yline1_ran=[]
    yline2_seq=[]
    yline2_mix=[]
    yline2_ran=[]
    for i in range(1,len(seq_threads)):
        if(float(seq_prob[i])==float(prob) and int(seq_threads[i])==int(thread) and int(seq_items[i])==int(item)):
            yline1_seq.append(float(seq_time[i]))
            yline2_seq.append(float(int(seq_aborts[i])/(int(seq_aborts[i])+int(seq_commits[i]))))
            xline_seq.append(seq_ops[i])
        if(float(mix_prob[i])==float(prob) and int(mix_threads[i])==int(thread) and int(mix_items[i])==int(item)):
            yline1_mix.append(mix_time[i])
            yline2_mix.append(float(int(mix_aborts[i])/(int(mix_aborts[i])+int(mix_commits[i]))))
            xline_mix.append(mix_ops[i])
        if(float(ran_prob[i])==float(prob) and int(ran_threads[i])==int(thread) and int(ran_items[i])==int(item)):
            yline1_ran.append(ran_time[i]) 
            yline2_ran.append(float(int(ran_aborts[i])/(int(ran_aborts[i])+int(ran_commits[i]))))
            xline_ran.append(ran_ops[i])
    plt.figure(str(file_to_open)+" Variable Operations",figsize=(12,6)) 
    plt.subplot(1, 2, 1)
    plt.plot(xline_seq,yline1_seq,c='blue',label="sequential")
    plt.plot(xline_mix,yline1_mix,c='green',label="mixed")
    plt.plot(xline_ran,yline1_ran,c='red',label="random")
    plt.ylabel("Time")
    plt.subplot(1, 2, 2)
    plt.plot(xline_seq,yline2_seq,c='blue',label="sequential")
    plt.plot(xline_mix,yline2_mix,c='green',label="mixed")
    plt.plot(xline_ran,yline2_ran,c='red',label="random")
    plt.ylabel("Abort rate")
    plt.xlabel("Operations per trans.")
    plt.legend(loc='upper left',ncol=3, bbox_to_anchor=(-0.75, 1.12))
    plt.show()

counter=0
i=-1
for line in file:
    words = line.split(";")
    if(words[1]=="THREADS"): continue
    if(int(counter)%int(tests) == 0):
        seq_threads.append(int(words[1]))
        seq_items.append(int(words[2]))
        seq_ops.append(int(words[3]))
        seq_commits.append(int(words[4]))
        seq_aborts.append(int(words[5]))
        seq_time.append(float(words[6]))
        seq_prob.append(float(words[7]))

        mix_threads.append(int(words[11]))
        mix_items.append(int(words[12]))
        mix_ops.append(int(words[13]))
        mix_commits.append(int(words[14]))
        mix_aborts.append(int(words[15]))
        mix_time.append(float(words[16]))
        mix_prob.append(float(words[17]))

        ran_threads.append(int(words[21]))
        ran_items.append(int(words[22]))
        ran_ops.append(int(words[23]))
        ran_commits.append(int(words[24]))
        ran_aborts.append(int(words[25]))
        ran_time.append(float(words[26]))
        ran_prob.append(float(words[27].strip("\n")))
        i += 1
    else:
        seq_threads[i] = int(words[1])
        seq_items[i] = int(words[2])
        seq_ops[i] = int(words[3])
        seq_commits[i] += int(words[4])
        seq_aborts[i] += int(words[5])
        seq_time[i] += float(words[6])
        seq_prob[i] = float(words[7])

        mix_threads[i] = int(words[11])
        mix_items[i] = int(words[12])
        mix_ops[i] = int(words[13])
        mix_commits[i] += int(words[14])
        mix_aborts[i] += int(words[15])
        mix_time[i] += float(words[16])
        mix_prob[i] = float(words[17])

        ran_threads[i] = int(words[21])
        ran_items[i] = int(words[22])
        ran_ops[i] = int(words[23])
        ran_commits[i] += int(words[24])
        ran_aborts[i] += int(words[25])
        ran_time[i] += float(words[26])
        ran_prob[i] = float(words[27].strip("\n"))
    counter += 1

    



if(str(prob) == "X"):
    variable_prob()
elif(str(thread) == "X"):
    variable_threads()
elif(str(item) == "X"):
    variable_items()
elif(str(operation) == "X"):
    variable_ops()
else:
    variable_prob()
    variable_threads()
    variable_items()
    variable_ops()

  

   


