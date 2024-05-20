import telnetlib
import threading
import time

def read_data(telnet_conn,f):
    start = time.time()
    while True:
        try:
            data = telnet_conn.read_some().decode()
            diff = time.time() - start
            
            print(data, end='')
            
            f.write(data)
        except:
            print("Done")
            break


#ip_ports = [("localhost",60001+i) for i in range(nodes)]
#files = [open(f"loss\p{i+1}.txt","w") for i in range(nodes-1)]
#telnets = [telnetlib.Telnet(term[0], term[1]) for term in ip_ports]
file1 = open(f"loss\p.txt","w")
#file2 = open(f"no_loss_interferer\p2.txt","w")
ip_port1 = telnetlib.Telnet("localhost",60001)
#ip_port2 = telnetlib.Telnet("localhost",60005)


#t2 = threading.Thread(target=read_data, args=(ip_port2,file2))
t1 = threading.Thread(target=read_data, args=(ip_port1,file1))

#t3 = threading.Thread(target=read_data, args=(telnets[2],[files[1]]))
#t4 = threading.Thread(target=read_data, args=(telnets[3],[files[2]]))
#threads = [t1,t2,t3,t4]



t1.start()
#t2.start()
t1.join()
#t2.join()
ip_port1.close()
file1.close()
#ip_port2.close()
#file2.close()

