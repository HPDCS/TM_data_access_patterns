import matplotlib.pyplot as plt
import sys
import numpy as np

#input_file_path = "gl_wt_10000"
if len(sys.argv)<=1:
    print("Usage python results.py file")
    exit()
input_file_path = sys.argv[1]
input_file = open(input_file_path, "r");

mode_1_plot = list()
mode_2_plot = list()
mode_3_plot = list()
aborts_1_plot = list()
aborts_2_plot = list()
aborts_3_plot = list()

use_mode_1 = True
use_mode_2 = True

counter = 0
last_abort = -1

for line in input_file:
    if "./app" in line or "writes" in line:
        continue
    elif "Commits" in line:
        st = line.split(":")
        if (last_abort == -1):
            if (use_mode_1 == True):
                aborts_1_plot.append(int(st[2].strip()))
            elif (use_mode_2 == True):
                aborts_2_plot.append(int(st[2].strip()))
            else:
                aborts_3_plot.append(int(st[2].strip()))
        else:
            if (use_mode_1 == True):
                aborts_1_plot.append(int(st[2].strip()) - last_abort)
            elif (use_mode_2 == True):
                aborts_2_plot.append(int(st[2].strip()) - last_abort)
            else:
                aborts_3_plot.append(int(st[2].strip()) - last_abort)
        
        last_abort = int(st[2].strip())

    elif "******" in line:
        counter += 1
        if (counter == 4):
            use_mode_1 = False
        elif (counter == 6):
            use_mode_2 = False
        continue
    else:
        #this line is similar to:    The elapsed time is 8.418048 seconds
        
        seconds = line.replace("The elapsed time is", "").strip()
        seconds = seconds.replace("seconds", "").strip()
        if(seconds == ""):
            continue
        
        seconds = float(seconds)
        if (use_mode_1 == True):
            mode_1_plot.append(seconds)
        elif (use_mode_2 == True):
            mode_2_plot.append(seconds)
        else:
            mode_3_plot.append(seconds)


plot_list = [mode_1_plot, mode_2_plot, mode_3_plot]
plt.boxplot(plot_list, showmeans=True)
plt.title("TinySTM transactions time with respect to read-writes patterns")
plt.xlabel("Pattern type")
plt.ylabel("Time (seconds)")
plt.xticks([1,2,3],["all the same", "thread pools", "random"])
plt.yticks(np.arange(0,0.1, step = 0.2))
plt.show()


abort_plot_list = [aborts_1_plot, aborts_2_plot, aborts_3_plot]
plt.boxplot(abort_plot_list, showmeans=True)
plt.title("TinySTM transaction abort number")
plt.xticks([1,2,3],["all the same", "thread pools","random"])
plt.ylabel("# aborts")
plt.yticks(np.arange(0,100000, step = 1000))
plt.show()