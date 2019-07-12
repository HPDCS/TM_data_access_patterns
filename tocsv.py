file = open("test", 'r', encoding = "utf-8")

out = open('test.csv','w')

run = ""
commits = ""
aborts = ""
time = ""
threads = ""
items = ""
operations = ""
time = ""

left = []
right = []

out.write("FUNCTION;COMMITS;ABORTS;TIME (s);THREADS;ITEMS;OPS;;;FUNCTION;COMMITS;ABORTS;TIME (s);THREADS;ITEMS;OPS\n")

for line in file:
	splitted = line.split()
	if "ITEMS" in line:
		threads = splitted[2]
		items = splitted[4]
		operations = splitted[6]
		run = "RUN4"
	if "pools" in line:
		run = "RUN5"
	if "Commits" in line:
		commits = splitted[1]
		aborts = splitted[3]
	if "time" in line:
		time = splitted[4].replace(".",",")
		if run == "RUN4":
			left.append(""+run+";"+commits+";"+aborts+";"+time+";"+threads+";"+items+";"+operations)
		else:
			right.append(""+run+";"+commits+";"+aborts+";"+time+";"+threads+";"+items+";"+operations)

for i in range(0,len(left)):
	out.write(left[i]+";;;"+right[i]+"\n")

file.close()
out.close()