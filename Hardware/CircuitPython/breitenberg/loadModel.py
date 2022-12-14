"""
Circuit Python code for reading a file

"""

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

#create neural network
net=Network(3)
net.add_layer(2)
net.add_layer(6)
net.show()

#load sd card
sleep(1)

spi = busio.SPI(GP10, MOSI=GP11, MISO=GP12)
cs = GP15
sd = sdcardio.SDCard(spi, cs)

vfs = storage.VfsFat(sd)
storage.mount(vfs, '/sd')

#load training set
x_data=np.zeros((200,2))
with open("/sd/modelsx_data.csv", "r") as file:
    for i,numeric_string in enumerate(file.read().split("\n")[:-1]):
        for j,o in enumerate(numeric_string.split(",")):
            val=o.replace("\r","")[0:8]
            x_data[i][j]=float(val)

y_data=[]
with open("/sd/modelsy_data.csv", "r") as file:
    y_data = [[float(o) for o in numeric_string.split(",")] for numeric_string in file.read().split("\n")[:-1]]

#load network data
weights=[]
with open("/sd/parameters.csv", "r") as file:
    weights = [float(numeric_string) for numeric_string in file.read().split("\n")[:-1]]
ind=[]
with open("/sd/meta_parameters.csv", "r") as file:
    ind = [int(numeric_string) for numeric_string in file.read().split("\n")[:-1]]
weights=np.array(weights)
#testing loop
def getAccuracy(net):
    accuracy=0
    for dat,lab in zip(x_data,y_data):
        y_pred=net.forward(dat)
        #get best
        if np.argmax(y_pred)==np.argmax(lab):
            accuracy+=1
    return accuracy/len(x_data) *100

def show_weights(net):
    for i in range(len(net.network)):
        print(np.sum(net.network[i].matrix))
    
#network built on previous trained weights
print(np.sum(net.network[-1].matrix))
print("Accuracy:",getAccuracy(net),"%")
net.reform_weights(weights,ind)

print("Accuracy:",getAccuracy(net) ,"%")
print("Accuracy:",getAccuracy(net) ,"%")

#test real sensors
sensorA=AnalogIn(board.GP26)
sensorB=AnalogIn(board.GP27)

def getSensor(): #get an input array of sensor readngs
    a=sensorA.value / 65535 * sensorA.reference_voltage /4.8
    b=sensorB.value / 65535 * sensorB.reference_voltage /4.8
    return 1-np.array([a,b])


b=Brain(2,2,size=200)
b.runP(4) #get started
p=[1,4,5]
actions=["forward","left","right"]
"""
for i in range(100):
    inputs=getSensor() #get sensor readings
    out=net.forward(inputs) #neural desicion
    b.runP(p[np.argmax(out)])
    print(inputs,"-->",b.out) #actions[np.argmax(out)]
    sleep(0.5)
"""