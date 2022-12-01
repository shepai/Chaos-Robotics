import adafruit_ntp
import socketpool
import time
import wifi
from Robot_Lib import *
# Get wifi details and more from a secrets.py file
try:
    from secrets import secrets
except ImportError:
    print("WiFi secrets are kept in secrets.py, please add them there!")
    raise


wifi.radio.connect(secrets["ssid"], secrets["password"])

print("My IP address is", wifi.radio.ipv4_address)
pool = socketpool.SocketPool(wifi.radio)
con=False
while not con:
    try:
        conns=pool.getaddrinfo(host="192.168.1.20",port=80)
        print(conns[0][4])
        socket=pool.socket()
        socket.connect(conns[0][4]) #connect to the first device
        con=True
    except OSError:
        time.sleep(1)
print("Running...")
arr=None
while True:
    b=bytearray(100) #empty listener
    data=socket.recvfrom_into(b) #wait for device data
    if data: #device data came through
        info=b.decode()
        info=str(info.split("\x00")[0])
        if info=="RUN": #run trial
            run(values=arr) #run a trial from the robot lib
        elif info=="GET": #get the data stored
            file = open("/sd/LogBB"+".csv", "r")
            r=file.read()
            file.close()
            batch_num=10
            left_over=len(r)%batch_num
            for i in range(0,len(r),batch_num):
                socket.send(str(r[i:i+batch_num]).encode())
            if left_over!=0:
                socket.send(str(r[left_over:-1]).encode()) #send remainder over
            socket.send("END".encode())
            print("sent")
        else: #data is commands for weights
            dat=info.split(",")
            arr_=[]
            for i in dat:
                if i!="":
                    arr_.append(float(i))
            if len(arr_)>=6:
                arr=np.array(arr_)
            print("brain weights and thetas:",arr)
    
    #socket.send("data".encode())
