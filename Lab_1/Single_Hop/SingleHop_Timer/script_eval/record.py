
import telnetlib
import threading
import time


f = open("record.txt","w")

term1_ip = 'localhost'
term1_port = 60001

term2_ip = 'localhost'
term2_port = 60002

term1_telnet = telnetlib.Telnet(term1_ip, term1_port)

term2_telnet = telnetlib.Telnet(term2_ip, term2_port)

def read_data(telnet_conn):
    start = time.time()
    while True:
        data = telnet_conn.read_some().decode()
        diff = time.time() - start
        #record for at least 300 seconds
        if not data or diff > 300:
            break
        print(data, end='')
        f.write(data)


t1 = threading.Thread(target=read_data, args=(term1_telnet,))
t2 = threading.Thread(target=read_data, args=(term2_telnet,))

t1.start()
t2.start()

t1.join()
t2.join()


term1_telnet.close()
term2_telnet.close()

f.close()