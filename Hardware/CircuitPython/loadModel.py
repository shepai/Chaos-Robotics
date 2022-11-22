"""
Circuit Python code for reading a file

"""

from CPML import *
import ulab.numpy as np
from board import *
from time import *
import busio
import sdcardio
import storage

#create neural network
net=Network(3)
net.add_layer(2)
net.add_layer(6)
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
x_data=[]
with open("/sd/x_data.csv", "r") as file:
    x_data = [[float(o) for o in numeric_string.split(",")] for numeric_string in file.read().split("\n")]

y_data=[]
with open("/sd/y_data.csv", "r") as file:
    y_data = [[float(o) for o in numeric_string.split(",")] for numeric_string in file.read().split("\n")]

#load network data
weights=[]
with open("/sd/parameters.csv", "r") as file:
    weights = [float(numeric_string) for numeric_string in file.read().split("\n")]
ind=[]
with open("/sd/meta_parameters.csv", "r") as file:
    ind = [float(numeric_string) for numeric_string in file.read().split("\n")]

#network built on previous trained weights
net.reform_weights(weights,ind)

#testing loop
accuracy=0
for dat,lab in zip(x_data,y_data):
    y_pred=net.forward(dat)
    #get best
    if np.argmax(y_pred)==np.argmax(lab):
        accuracy+=1
print("Accuracy:",accuracy/len(x_data) *100,"%")
