import matplotlib.pyplot as plt
import sys

#input_file_path = "gl_wt_10000"
if len(sys.argv)<=1:
    print("Usage python results.py file")
    exit()
input_file_path = sys.argv[1]
input_file = open(input_file_path, "r");

mode_1_plot = list()
mode_2_plot = list()

use_mode_1 = True
counter = 0

for line in input_file:
    if "./app" in line or "_wt" in line:
        continue
    elif "******" in line:
        counter += 1
        if (counter >=4):
             use_mode_1 = False
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
        else:
            mode_2_plot.append(seconds)

runs = list(range(1, len(mode_1_plot)+1))
plot_list = [mode_1_plot, mode_2_plot]
plt.boxplot(plot_list)
plt.title("GCCTM transactions time with respect to read-writes patterns")
plt.xlabel("Pattern type")
plt.ylabel("Time (seconds)")
plt.xticks([1,2],["reads_before_writes", "writes_before_reads"])
plt.show()