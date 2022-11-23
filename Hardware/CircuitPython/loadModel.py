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
x_data=np.zeros((200,2))
with open("/sd/modelsx_data.csv", "r") as file:
    for i,numeric_string in enumerate(file.read().split("\n")[:-1]):
        for j,o in enumerate(numeric_string.split(",")):
            val=o.replace("\r","")[0:7]
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


