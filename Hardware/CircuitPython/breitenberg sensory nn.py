from motorHatControl import *
from CPML import *
import ulab.numpy as np
from board import *
from time import *
import busio
import board
import sdcardio
import storage
import digitalio
from analogio import AnalogIn
from Brain import Brain
import adafruit_hcsr04

#create neural network
net=Network(3)
net.add_layer(2)
net.add_layer(6)
net.show()

#mount sd
spi = busio.SPI(GP10, MOSI=GP11, MISO=GP12)
cs = GP15
sd = sdcardio.SDCard(spi, cs)

vfs = storage.VfsFat(sd)
storage.mount(vfs, '/sd')

#load network data
weights=[]
with open("/sd/parameters.csv", "r") as file:
    weights = [float(numeric_string) for numeric_string in file.read().split("\n")[:-1]]
ind=[]
with open("/sd/meta_parameters.csv", "r") as file:
    ind = [int(numeric_string) for numeric_string in file.read().split("\n")[:-1]]
weights=np.array(weights)
net.reform_weights(weights,ind)

#create sensors
sensorA=AnalogIn(board.GP26)
sensorB=AnalogIn(board.GP27)


#gp2 --> echo
#gp3 --> trig
sonarA = adafruit_hcsr04.HCSR04(trigger_pin=board.GP3, echo_pin=board.GP2)
#gp4 --> echo
#gp5 --> trig
sonarB = adafruit_hcsr04.HCSR04(trigger_pin=board.GP5, echo_pin=board.GP4)

def getDist():
    a=[-1,-1]
    try:
        a=[sonarA.distance] #sonarB.distance does not work
    except RuntimeError as e:
        print("Error connecting to sensor",e)
    return a

def getSensor(): #get an input array of sensor readngs
    a=sensorA.value / 65535 * sensorA.reference_voltage /4.8
    b=sensorB.value / 65535 * sensorB.reference_voltage /4.8
    return 1-np.array([a-0.1,b])
#create motors
Enable_motor() #from motor hat control

b=Brain(2,2,size=50)
b.runP(4) #get started
p=[1,4,6]
dirA=0
dirB=0
file = open("/sd/LogBB.csv", "w")
for i in range(100):
    inputs=getSensor() #get sensor readings
    out=net.forward(inputs) #neural desicion
    b.runP(p[np.argmax(out)])
    #move based on outut
    if b.out[0]-0.15<=b.out[1] and b.out[0]+0.15>=b.out[1]: #within range
        dirA=1
        dirB=1
        Motor1_reverse()
        Motor2_reverse()
    elif b.out[0]>b.out[1]:
        dirA=1
        dirB=-1
        Motor1_forward()
        Motor2_reverse()
    elif b.out[0]<b.out[1]:
        dirA=-1
        dirB=1
        Motor1_reverse()
        Motor2_forward()
    if getDist()[0] < 10: #stop if obstacle in front to avoid damage
        dirA=0
        dirB=0
        Motor_stop()
    if i%50==0: #prevent overflow
        b.reset()
    sleep(0.5)
    Motor_stop()
    file.write(str(inputs[0])+","+str(inputs[1])+","+str(dirA)+","+str(dirB)+","+str(b.out[0])+","+str(b.out[1])+","+str(b.p)+"\n")
        
file.close()
    
    
