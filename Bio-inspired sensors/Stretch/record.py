from mpremote import pyboard
import serial, time
import sys
import glob
import cv2
import matplotlib.pyplot as plt
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
    def get(self):
        return float(self.COM.exec('get()').decode("utf-8").replace("/r/n","").replace("\n",""))
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
        COM=res[0]
    except IndexError:
        time.sleep(1)

vid = cv2.VideoCapture(1)
print("Loading...")
input("Press Enter to begin")
ret, frame = vid.read()
#make demo
fig, axes = plt.subplots(1,2)
axes[0].set_title("A: Stretch", loc="left")
axes[0].imshow(frame)
#axes[0].set_yticks(axes[0].get_yticks()[::33]) #axes[0].get_xticks()[::100]
#axes[1].figure(figsize=(5,1))
axes[1].set_title("B: Current", loc="left")
axes[1].plot([0],label="Signal")
axes[1].set_xlabel("Time (t)")
axes[1].set_ylabel("value")
fig.savefig("save"+".png")
ad=cv2.imread("save"+".png")
h=ad.shape[0]
w=ad.shape[1]
B.runFile("C:/Users/dexte/github/Chaos-Robotics/Bio-inspired sensors/Stretch/readStretch.py")

out = cv2.VideoWriter("C:/Users/dexte/github/Chaos-Robotics/Assets/"+'stretch_short.avi',cv2.VideoWriter_fourcc(*'DIVX'), 10, (w,h))


time_delay=0.01
iterations=250
a=[]
for i in range(iterations):
    a.append(B.get())
    ret, frame = vid.read()
    fig, axes = plt.subplots(2,1)
    axes[0].set_title("A: Stretch", loc="left")
    axes[0].imshow(frame)
    #axes[0].set_yticks(axes[0].get_yticks()[::33]) #axes[0].get_xticks()[::100]
    #axes[1].figure(figsize=(5,1))
    axes[1].set_title("B: Current", loc="left")
    axes[1].plot(a,label="Signal")
    axes[1].set_xlabel("Time (t)")
    axes[1].set_ylabel("value")
    fig.savefig("save"+".png")
    ad=cv2.imread("save"+".png")
    out.write(ad)
    time.sleep(time_delay)
out.release()
