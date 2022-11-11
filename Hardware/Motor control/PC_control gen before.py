import numpy as np
import matplotlib.pyplot as plt
import math as maths
import random
from mpremote import pyboard
import serial, time
import sys
import glob

"""
Setup control with micropython device
"""

class Board:
    def __init__(self):
        self.COM=None
        self.VALID_CHANNELS=[i for i in range(10)]
    def connect(self,com):
        """
        Connect to a com given
        @param com of serial port
        @param fileToRun is the file that executes on board to allow hardware interfacing
        """
        self.COM=pyboard.Pyboard(com) #connect to board
        self.COM.enter_raw_repl() #open for commands
        print("Successfully connected")
    def runFile(self,fileToRun='main.py'):
        """
        runFile allows the user to send a local file to the device and run it
        @param fileToRun is the file that will be locally installed
        """
        if self.COM==None:
            raise OSError("Connect to device before running file")
        self.COM.execfile(fileToRun) #run the file controlling the sensors
    def serial_ports(self):
        """
        Read the all ports that are open for serial communication
        @returns array of COMS avaliable
        """
        if sys.platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            ports = glob.glob('/dev/tty[A-Za-z]*')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform') #if the OS is not one of the main

        result = []
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
        return result
    def move(self,motor,angle):
        self.COM.exec_raw_no_follow('moveMotor('+str(motor)+','+str(angle)+')')#.decode("utf-8").replace("\r\n","")
    def close(self):
        self.COM.close()

    
B=Board()
#get serial boards and connect to first one
COM=""
while COM=="":
    try:
        res=B.serial_ports()
        print("ports:",res)
        B.connect(res[0])
        B.runFile("C:/Users/dexte/github/Chaos-Robotics/Hardware/Motor control/main_program.py")
        COM=res[0]
    except IndexError:
        time.sleep(1)


#set up chaos

def sigmoid(x):
    with np.errstate(invalid='ignore'):
        return (1/(1+np.exp(-1*x)))

def step_e(i,t,mus,Cp,x,j,lambda_,p):
    global p_count
    #apply mathematic formula
    with np.errstate(invalid='ignore'):
        mus[t+1]=(mus[t-1]**p)+lambda_*(((x[0][t-1]-x[0][t-1-p])**2+(x[1][t-1]-x[1][t-1-p])**2)/p) 
        Cp[:,t]=(mus[t])*np.sum(weights*(x[:,t]-x[:,t-p]),axis=1) 
    a=np.dot(weights,x[:,t])
    x[i][t+1]=sigmoid(thetas[i]+a[i]+Cp[i][t])
    x[j][t+1]=sigmoid(thetas[j]+a[j]+Cp[j][t])
    return x
    
def run(steps,p,lambda_=0.05):
    global thetas
    global weights
    mus=np.zeros((steps,))
    mus[0]=-1
    x=np.zeros((2,steps))
    x[0][0]=random.randint(0,2)
    x[1][0]=random.randint(0,2)
    Cp=np.zeros((2,steps))
    Cp[0][0]=1
    Cp[1][0]=1
    thetas=np.random.normal(1,0.5,(2,1)) #bias terms
    weights=np.random.normal(1,0.5,(2,2))
    t=1
    for st in range(steps-2):
        t=st+1
        x=step_e(0,t,mus,Cp,x,1,lambda_,p)
    avg_out=((x[0]+x[1])/2)[:-1]
    return avg_out,mus

steps=10
genotype=np.array([random.randint(1,9) for i in range(steps)])
new_geno=np.zeros((steps**2,))
#count differences
diff=(np.diff(genotype)!=0).sum()
#steps*=100 #increase overall size
l=0.05
for i in range(len(genotype)):
    avg_out,mus=run(steps,genotype[i],lambda_=l)
    new_geno[0+steps*i:min(steps+steps*i -1,len(new_geno)-1)]=avg_out[0:steps-1]
plt.plot(new_geno)
plt.show()
for sig in new_geno:
    B.move(1,max(sig*180,180))
    time.sleep(0.75)
    print(sig*180)
print("move")
time.sleep(1)

B.close()
