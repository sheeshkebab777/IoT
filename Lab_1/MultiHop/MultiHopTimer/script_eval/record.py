
import telnetlib
import threading
import time

def read_data(telnet_conn,files):
    start = time.time()
    while True:
        data = telnet_conn.read_some().decode()
        diff = time.time() - start
        #record for at least 300 seconds
        if not data or diff > 300:
            break
        print(data, end='')
        for f in files:
            f.write(data)

nodes = 4

ip_ports = [("localhost",60001+i) for i in range(nodes)]
files = [open(f"loss\p{i+1}.txt","w") for i in range(nodes-1)]
telnets = [telnetlib.Telnet(term[0], term[1]) for term in ip_ports]





t1 = threading.Thread(target=read_data, args=(telnets[0],files))
t2 = threading.Thread(target=read_data, args=(telnets[1],[files[0]]))
t3 = threading.Thread(target=read_data, args=(telnets[2],[files[1]]))
t4 = threading.Thread(target=read_data, args=(telnets[3],[files[2]]))
threads = [t1,t2,t3,t4]

for thread in threads:
    thread.start()
    
for thread in threads:
    thread.join()

for telnet in telnets:
    telnet.close()
    
for file in files:
    file.close()