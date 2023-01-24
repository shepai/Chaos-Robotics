from mpremote import pyboard
import serial, time
import sys
import glob
import cv2
import matplotlib.pyplot as plt
from scipy.signal import butter,filtfilt
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
        time.sleep(2)
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

vid = cv2.VideoCapture(0)
print("Loading...")
input("Press Enter to begin")
ret, frame = vid.read()
#make demo
#cv2.imshow("...",frame)
#cv2.waitKey(0)
fig, axes = plt.subplots(1,2)
axes[0].set_title("A: Stretch", loc="left")
axes[0].imshow(frame)
#axes[0].set_yticks(axes[0].get_yticks()[::33]) #axes[0].get_xticks()[::100]
#axes[1].figure(figsize=(5,1))
axes[1].set_title("Servo angle: "+str(100)+" degrees", loc="left")
axes[1].plot([0],label="Signal")
axes[1].set_xlabel("Time (t)")
axes[1].set_ylabel("value")

fig.savefig("save"+".png")
ad=cv2.imread("save"+".png")
h=ad.shape[0]
w=ad.shape[1]
B.runFile("/its/home/drs25/Documents/VSCODE/Chaos-Robotics/Bio-inspired sensors/Stretch/experiment_with_servo.py")

out = cv2.VideoWriter("/its/home/drs25/Documents/VSCODE/Chaos-Robotics/Assets/"+'stretch_servo2.avi',cv2.VideoWriter_fourcc(*'DIVX'), 8, (w,h))
# Filter requirements.
T = 5.0         # Sample Period
fs = 30.0       # sample rate, Hz
cutoff = 2      # desired cutoff frequency of the filter, Hz ,      slightly higher than actual 1.2 Hznyq = 0.5 * fs  # Nyquist Frequencyorder = 2       # sin wave can be approx represented as quadratic
n = int(T * fs) # total number of samples
nyq = 0.5 * fs  # Nyquist Frequency

order = 2       # sin wave can be approx represented as quadratic
n = int(T * fs) # total number of samples

def butter_lowpass_filter(data, cutoff, fs, order):
    normal_cutoff = cutoff / nyq
    # Get the filter coefficients 
    b, a = butter(order, normal_cutoff, btype='low', analog=False)
    y = filtfilt(b, a, data)
    return y

time_delay=0.01
iterations=180
a=[13715.0,13715.0,13715.0,13715.0,13715.0,13715.0,13715.0,13715.0,13715.0,13715.0,13715.0,13715.0]
angle={}
fig, axes = plt.subplots(3,1)

for i in range(120,iterations):
    current=B.get(pos=i)
    angle[str(i)]=current
    a.append(current)
    ret, frame = vid.read()
    axes[0].set_title("A: Stretch", loc="left")
    axes[0].imshow(frame)
    #axes[0].set_yticks(axes[0].get_yticks()[::33]) #axes[0].get_xticks()[::100]
    #axes[1].figure(figsize=(5,1))
    #axes[1].set_title("B: Current", loc="left")
    axes[1].set_title("Servo angle: "+str(i)+" degrees", loc="left")
    axes[1].plot(a[12-i:],label="Signal")
    axes[1].set_xlabel("Time (t)")
    axes[1].set_ylabel("value")
    axes[1].set_title("Filtered signal", loc="left")
    axes[1].set_ylim(min(a[12:]),max(a))
    filtered = butter_lowpass_filter(np.array(a), cutoff, fs, order)
    
    axes[2].cla()
    axes[2].plot((np.array(filtered[12:]))/np.max(filtered),label="filtered")
    axes[2].set_xlabel("Time (t)")
    axes[2].set_ylabel("value")
    axes[2].set_ylim(0.5,1)
    fig.savefig("save"+".png")
    ad=cv2.imread("save"+".png")
    out.write(ad)
    time.sleep(time_delay)

for i in reversed(range(120,iterations)):
    current=B.get(pos=i)
    angle[str(i)]=current
    a.append(current)
    ret, frame = vid.read()
    axes[0].set_title("A: Stretch", loc="left")
    axes[0].imshow(frame)
    #axes[0].set_yticks(axes[0].get_yticks()[::33]) #axes[0].get_xticks()[::100]
    #axes[1].figure(figsize=(5,1))
    #axes[1].set_title("B: Current", loc="left")
    axes[1].set_title("Servo angle: "+str(i)+" degrees", loc="left")
    axes[1].plot(a[12-i:],label="Signal")
    axes[1].set_xlabel("Time (t)")
    axes[1].set_ylabel("value")
    axes[1].set_title("Filtered signal", loc="left")
    axes[1].set_ylim(min(a[12:]),max(a))
    filtered = butter_lowpass_filter(np.array(a), cutoff, fs, order)
    
    axes[2].cla()
    axes[2].plot((np.array(filtered[12:]))/np.max(filtered),label="filtered")
    axes[2].set_xlabel("Time (t)")
    axes[2].set_ylabel("value")
    axes[2].set_ylim(0.5,1)
    fig.savefig("save"+".png")
    ad=cv2.imread("save"+".png")
    out.write(ad)
    time.sleep(time_delay)

print("Saving...")
B.get(pos=90)
out.release()
print(angle)