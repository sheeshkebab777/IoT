from collections import namedtuple
import matplotlib.pyplot as plt

def reliability(d):
    m = [max(d[k]) for k in d.keys()]
    r = [0,0,0]

    for k in d.keys():
        for item in d[k]:
            r[k]+=1
    
    return tuple((i/j for i,j in zip(r,m)))
Entry = namedtuple("Entry",["id","counter","latency"])
file = open("no_loss\p.txt","r")
lines = list(map(lambda x: x.replace("\n","").split(";"),list(filter(lambda x: x!="\n",file.readlines()))))
lines = list(map(lambda x: Entry(int(x[0]),int(x[1]),int(x[5])),lines))

dic = {0:[],1:[],2:[]}
lat_dic = [[],[],[]]
seen_counts = []
for line in lines:
    if line.counter not in dic[line.id]:
        dic[line.id].append(line.counter)
        if abs(line.latency) < 3000:
            lat_dic[line.id].append(abs(line.latency))

print(len(dic[0]),len(dic[2]))

lat = reversed(lat_dic)
rel = reversed(reliability(dic))

plt.hist(lat_dic, bins=50, color=['blue', 'orange','green'], label=['Node3','Node2','Node1'])

# Add labels and title
plt.xlabel('Latency in MSecs')
plt.ylabel('Packets')
plt.title('Latency Distribution: No Loss')
plt.legend()

# Show plot
plt.show()

#########################################
'''
categories = ['Node1','Node2','Node3']
values = list(map(lambda x: x* 100, rel))


fig, ax = plt.subplots()
x_labels = categories
ax.bar(x_labels, values, color='blue')
ax.set_ylabel('Reliability in %')
ax.set_xlabel('Directions')
ax.set_title('Reliability: No Loss')
ax.set_ylim(ymin=0, ymax=100)
plt.show()'''

file.close()