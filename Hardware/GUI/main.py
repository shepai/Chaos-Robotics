from tkinter import *
from tkinter import ttk
import socket
import numpy as np

root = Tk()
root.geometry("700x350")
frm = ttk.Frame(root, padding=10)
frm.grid()
frm.pack(side=TOP)
#setup changable label variables
v = StringVar()
noiseV = StringVar()

class network:
    def __init__(self,HOST="192.168.1.20",PORT=80):
        self.s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.bind((HOST, PORT))
        self.noise=""
    def connect(self):
        self.s.listen()
        self.conn, addr = self.s.accept()
        print(f"Connected by {addr}")
        v.set("Connected")
    def disconnect(self): #try and reconnect
        v.set("Disconnected")
        try:
            self.__init__()
            self.connect()
        except ConnectionResetError:
            pass
        except TimeoutError:
            pass
    def setNoise(self,n):
        string=""
        for val in n.flatten():
            string+=str(val)+","
        self.noise=string[:-1]
    def getNoise(self):
        return self.noise
    def sendCommand(self,string):
        self.conn.send(string.encode())
    def haltRobot(self):
        pass
    def get(self):
        a=""
        while True:
            data = self.conn.recv(1024)
            if not data:
                print("data not found")
                break
            elif "END" in data.decode():
                break
            else:
                #print(data.decode())
                a+=data.decode()
        return a

net=network()

def connect():
    net.connect()
    
def noise():
    n=np.random.normal(0.5,1,(6,1))
    net.setNoise(n)
    noiseV.set(net.noise[0:6]+"...")

def send():
    net.sendCommand(net.getNoise())

def getData():
    net.sendCommand("GET")
    #time.sleep(0.1)
    a=net.get()
    file=open("logBB.csv","w")
    file.write(a)
    file.close()
    print(len(a))

def kill():
    pass

def run():
    net.sendCommand("RUN")

ttk.Button(frm, text="Connect", command=connect).grid(column=0, row=0)
ttk.Label(frm, textvariable=v).grid(column=1, row=0)
v.set("Not connected")
ttk.Button(frm, text="Generate noise", command=noise).grid(column=0, row=1)
ttk.Button(frm, text="Send to robot", command=send).grid(column=0, row=2)
ttk.Button(frm, text="Stop robot", command=kill).grid(column=1, row=2)
ttk.Button(frm, text="Retrieve data", command=getData).grid(column=0, row=3)
ttk.Button(frm, text="Run", command=run).grid(column=1, row=3)

ttk.Label(frm, textvariable=noiseV).grid(column=1, row=1)
noiseV.set("No values generated")

root.mainloop()
