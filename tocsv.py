import sys

file_input = sys.argv[1]
file_output = sys.argv[2]

file = open(file_input, 'r', encoding = "utf-8")

out = open(file_output,'w')

run = ""
commits = ""
aborts = ""
time = ""
threads = ""
items = ""
operations = ""
time = ""
prob = ""

sequential_run = []
pool_run = []
random_run = []

out.write("FUNCTION;THREADS;ITEMS;OPS;COMMITS;ABORTS;TIME (s);PROB;;;FUNCTION;THREADS;ITEMS;OPS;COMMITS;ABORTS;TIME (s);PROB;;;FUNCTION;THREADS;ITEMS;OPS;COMMITS;ABORTS;TIME (s);PROB\n")

for line in file:
	splitted = line.split()
	if "ITEMS" in line:
		threads = splitted[2]
		items = splitted[4]
		operations = splitted[6]
		prob = splitted[10]
		run = "SEQUENTIAL"
	if "pools" in line:
		run = "MIXED"
	if "accesses" in line:
		run = "RANDOM"
	if "Commits" in line:
		commits = splitted[1]
		aborts = splitted[3]
	if "time" in line:
		time = splitted[4]
		time = time.replace(",",".")
		if run == "SEQUENTIAL":
			sequential_run.append(""+run+";"+threads+";"+items+";"+operations+";"+commits+";"+aborts+";"+time+";"+prob)
		if run == "MIXED":
			pool_run.append(""+run+";"+threads+";"+items+";"+operations+";"+commits+";"+aborts+";"+time+";"+prob)
		if run == "RANDOM":
			random_run.append(""+run+";"+threads+";"+items+";"+operations+";"+commits+";"+aborts+";"+time+";"+prob)
			

for i in range(0,len(sequential_run)):
	out.write(sequential_run[i]+";;;"+pool_run[i]+";;;"+random_run[i]+"\n")

file.close()
out.close()
