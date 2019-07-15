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

out.write("FUNCTION;THREADS;ITEMS;OPS;COMMITS;ABORTS;TIME (s);;;FUNCTION;THREADS;ITEMS;OPS;COMMITS;ABORTS;TIME (s);\n")

for line in file:
	splitted = line.split()
	if "ITEMS" in line:
		threads = splitted[2]
		items = splitted[4]
		operations = splitted[6]
		run = "RUN3"
	if "pools" in line:
		run = "RUN4"
	if "Commits" in line:
		commits = splitted[1]
		aborts = splitted[3]
	if "time" in line:
		time = splitted[4].replace(".",",")
		if run == "RUN3":
			left.append(""+run+";"+threads+";"+items+";"+operations+";"+commits+";"+aborts+";"+time)
		else:
			right.append(""+run+";"+threads+";"+items+";"+operations+";"+commits+";"+aborts+";"+time)

for i in range(0,len(left)):
	out.write(left[i]+";;;"+right[i]+"\n")

file.close()
out.close()