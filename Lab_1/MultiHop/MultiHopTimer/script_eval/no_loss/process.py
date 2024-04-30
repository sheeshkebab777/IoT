import matplotlib.pyplot as plt
def get_rel_lat(send,recv):
    rel = 0
    recv_counts = [c for _,c in recv]
    for time,counter in send:
        if counter in recv_counts:
            rel+=1
    
    rel/=len(send)
    recv_counts_for_sender = [(time,c) for time,c in send if c in recv_counts]
    lat = [abs(x-y) for (y,_),(x,_) in zip(recv_counts_for_sender,recv) ]
    
    return rel,lat
def parse(lines):
    lines = list(map(lambda x: x.replace("\n",""), lines))
    lines = list(filter(lambda x: x != '',lines))
    sendings = []
    receivings = []
    
    for line in lines:
        time, sender,_,counter = line.split()
        time = int(time[:-1])
        sender = True if sender == "Sending" else False
        counter = int(counter)
        
        if sender:
            sendings.append((time,counter))
        else:
            receivings.append((time,counter))
    return sendings,receivings

files = ["p1.txt","p2.txt","p3.txt"]
files = map(lambda x: open(x,"r"),files)
reliability = []
latency = []
for file in files:
    lines = file.readlines()
    send,recv = parse(lines)
    rel, lat = get_rel_lat(send,recv)
    reliability.append(rel)
    latency.append(lat)
    file.close()



'''
plt.hist(latency, bins=50, color=['blue', 'orange','green'], label=['Node1','Node2','Node3'])

# Add labels and title
plt.xlabel('Latency in MSecs')
plt.ylabel('Packets')
plt.title('Latency Distribution')
plt.legend()

# Show plot
plt.show()

#########################################
'''
categories = ['Node1','Node2','Node3']
values = list(map(lambda x: x* 100, reliability))


fig, ax = plt.subplots()
x_labels = categories
ax.bar(x_labels, values, color='blue')
ax.set_ylabel('Reliability in %')
ax.set_xlabel('Directions')
ax.set_title('Reliability')
ax.set_ylim(ymin=0, ymax=100)
plt.show()

