import matplotlib.pyplot as plt
#parse right datatypes
def parse(line):
    time_ms, mac, sender, _, value = list(line.split())
    return int(time_ms[:-1]),mac, True if sender == "Sending" else False,int(value)

#calculate latency, reliability
def lat_rel(dic,macs):
    
    mac1, mac2 = macs
    send = dic[mac1]["send"].copy()
    recv = dic[mac2]["recv"].copy()
    #reliability
    sent_values = [y for x,y in send]
    recv_values = [y for x,y in recv]
    
    reliability = len(recv_values)/len(sent_values)
    
    delete_vals = []
    #latency
    #remove all sent values which were not received
    for tup in send:
        x,y = tup
        
        if y not in recv_values:
            delete_vals.append(tup)
    
    for val in delete_vals:
        send.remove(val)
    
    latency_values = [abs(y-x) for ((x,_),(y,_)) in zip(send,recv)]
    
    
    return reliability, latency_values

file = open("record.txt","r")
#remove all \n chars
Lines = list(filter(lambda x: x != '\n',file.readlines()))
Lines = [x.replace("\n","").replace("(random)","") for x in Lines]

dic = {}

for line in Lines:
    time_ms,mac,sender,value = parse(line)
    role = "send" if sender else "recv"
    l = dic.get(mac)
    
    if l == None:
        l = {"send":[],"recv":[]}
        dic[mac] = l
        
    
    if value not in [y for x,y in l[role]]:
        l[role].append((time_ms,value))

macs = list(dic.keys())

#1. direction
rel1, lat1 = lat_rel(dic,macs)
rel2, lat2 = lat_rel(dic,list(reversed(macs)))

'''
plt.hist([lat1, lat2], bins=30, color=['blue', 'orange'], label=['Direction 1', 'Direction 2'])

# Add labels and title
plt.xlabel('Latency in MSecs')
plt.ylabel('Packets')
plt.title('Latency Distribution')
plt.legend()

# Show plot
plt.show()
'''
#########################################

categories = ['Dir_1', 'Dir_2']
values = [rel1*100,rel2*100]


fig, ax = plt.subplots()
x_labels = categories
ax.bar(x_labels, values, color='blue')
ax.set_ylabel('Reliability in %')
ax.set_xlabel('Directions')
ax.set_title('Reliability')
ax.set_ylim(ymin=0, ymax=100)
plt.show(block=False)

