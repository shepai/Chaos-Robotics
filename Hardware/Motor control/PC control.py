import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button
import math as maths
import random
from mpremote import pyboard
import serial, time
import sys
import glob
from multiprocessing import Process

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




"""

GUI part of code

"""
# Define initial parameters
a=5
b=2
steps=650
lambda_=0.05
dt=0.01
pp=1

mus=np.zeros((steps,))
mus[0]=-1
x=np.zeros((2,steps))
x[0][0]=random.randint(0,2)
x[1][0]=random.randint(0,2)
Cp=np.zeros((2,steps))
Cp[0][0]=1
Cp[1][0]=1
thetas=np.random.random((2,1)) #bias terms
weights=np.random.random((2,2))
lower=1
outputs=np.zeros((2,steps))


def show_gen_control():
    mus=np.zeros((steps,))
    mus[0]=-1
    x=np.zeros((2,steps))
    x[0][0]=random.randint(0,2)
    x[1][0]=random.randint(0,2)
    Cp=np.zeros((2,steps))
    Cp[0][0]=1
    Cp[1][0]=1
    
    outputs=np.zeros((2,steps))

    for st in range(steps-2):
        """if i%1==0: #random inputs
            x[0][t+1]=random.randint(-3,3)
            x[1][t+1]=random.randint(-3,3)"""
        t=st+1
        x=step_e(0,t,mus,Cp,x,1)
    outputs=x.copy()
    return outputs[:,600:630],x[:,600:630]

#implementing this with random generation

def sigmoid(x):
    return (lower-1)+(1/(1+np.exp(-1*x)))

def step_e(i,t,mus,Cp,x,j):
    p=pp
    #apply mathematic formula
    with np.errstate(invalid='ignore'):
        mus[t+1]=(mus[t-1]**p)+lambda_*(((x[0][t-1]-x[0][t-1-p])**2+(x[1][t-1]-x[1][t-1-p])**2)/p) 
        Cp[:,t]=(mus[t])*np.sum(weights*(x[:,t]-x[:,t-p]),axis=1) 
    a=np.dot(weights,x[:,t])
    x[i][t+1]=sigmoid(thetas[i]+a[i]+Cp[i][t])
    x[j][t+1]=sigmoid(thetas[j]+a[j]+Cp[j][t])
    return x
    
def sender(arr,com):
    #arr,B=arr[0],arr[1]
    B=Board()
    B.connect(com)
    B.runFile("C:/Users/dexte/github/Chaos-Robotics/Hardware/Motor control/main_program.py")
    for i in range(len(arr)):
        print("moving 1 to ",180*arr[i])
        B.move(1,180*arr[i])
        time.sleep(0.5)

if __name__ == '__main__':
    B=Board()
    #get serial boards and connect to first one
    COM=""
    while COM=="":
        try:
            res=B.serial_ports()
            print("ports:",res)
            B.connect(res[0])
            
            COM=res[0]
            B.move(1,180)
            B.close()
        except IndexError:
            time.sleep(1)

    
    print("Starting")
    # Create the figure and the line that we will manipulate
    fig, ax = plt.subplots()
    #fig.subplots_adjust(left=0.05, bottom=0.15, right=0.95, top=0.98, wspace=0.05, hspace=0.05)
    out,x=show_gen_control()
    avg_out=((out[0]+out[1])/2)[:-1]
    line, = ax.plot(avg_out, lw=2)
    line2, = ax.plot(x[0],c="r",label="neuron 2", lw=2)
    line3, = ax.plot(x[1],c="b",label="neuron 1", lw=2)

    p=Process(target=sender, args=[avg_out,COM]) #start playing
    p.start()
    
    ax.set_xlabel('Iteration +600')
    ax.set_ylabel('Neuron output')
    ax.set_ylim(0,1)
    ax.legend(loc="lower right")
    # adjust the main plot to make room for the sliders
    fig.subplots_adjust(left=0.25, bottom=0.25)

    # Make a horizontal slider to control the frequency.
    axfreq = fig.add_axes([0.1, 0.1, 0.65, 0.03])

    freq_slider = Slider(
        ax=axfreq,
        label='lambda',
        valmin=0.01,
        valmax=1,
        valinit=lambda_,
    )

    # Make a vertically oriented slider to control the amplitude
    axamp = fig.add_axes([0.1, 0.15, 0.65, 0.03])
    amp_slider = Slider(
        ax=axamp,
        label="p",
        valmin=1,
        valmax=10,
        valinit=pp,
    )


    # The function to be called anytime a slider's value changes
    def update(val):
        global lambda_
        global pp
        global p
        lambda_ = freq_slider.val
        pp=int(amp_slider.val)
        outputs,x=show_gen_control()
        avg_out=((outputs[0]+outputs[1])/2)[:-1]
        line.set_ydata(avg_out)
        line2.set_ydata(x[0])
        line3.set_ydata(x[1])
        p.kill() #reset background task
        p=Process(target=sender, args=[avg_out,COM]) #start playing
        p.start()   #set new background task
        fig.canvas.draw_idle()


    # register the update function with each slider
    freq_slider.on_changed(update)
    amp_slider.on_changed(update)

    # Create a `matplotlib.widgets.Button` to reset the sliders to initial values.
    resetax = fig.add_axes([0.8, 0.025, 0.1, 0.04])
    button = Button(resetax, 'Reset', hovercolor='0.975')
    resetax1 = fig.add_axes([0.8, 0.001, 0.1, 0.04])
    button1 = Button(resetax1, 'Reset weights', hovercolor='0.975')

    def reset(event):
        freq_slider.reset()
        amp_slider.reset()

    def reset_weight(event):
        thetas=np.random.random((2,1)) #bias terms
        weights=np.random.random((2,2))
        update(event)

    button.on_clicked(reset)
    button1.on_clicked(reset_weight)
    plt.show()