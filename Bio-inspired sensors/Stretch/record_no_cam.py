from mpremote import pyboard
import serial, time
import sys
import glob
import numpy as np
import math

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
        ports.append('/dev/ttyACM0')
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except:
                pass
        return result
    def get(self,pos=0):
        self.COM.exec_raw_no_follow('setPosition('+str(pos)+')')
        time.sleep(1)
        val=self.COM.exec('get_raw()').decode("utf-8").replace("/r/n","").replace("\n","")
        return float(val)
    def close(self):
        self.COM.close()

def sigmoid(x):
  return 1 / (1 + math.exp(-x))

B=Board()
#get serial boards and connect to first one
COM=""
while COM=="":
    try:
        res=B.serial_ports()
        print("ports:",res)
        B.connect(res[0])
        COM=res[0]
    except IndexError:
        time.sleep(1)

B.runFile("/its/home/drs25/Documents/GitHub/Chaos-Robotics/Bio-inspired sensors/Stretch/experiment_with_servo.py")
time_delay=0.01
iterations=180
trials=30
array=np.zeros((trials,2,len(list(range(100,iterations)))))

for trial in range(trials):
    print("trial",trial+1)
    for i,ang in enumerate(range(100,iterations)):
        current=B.get(pos=ang)
        array[trial][0][i]=ang
        array[trial][1][i]=current
    print(array[trial])

np.save("/its/home/drs25/Documents/GitHub/Chaos-Robotics/Bio-inspired sensors/Stretch/stretch_data",array)