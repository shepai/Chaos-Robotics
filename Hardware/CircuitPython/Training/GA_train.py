
"""
This code is to be run on the PC side,
It makes use of the neural network training
"""
import torch
import torch.nn as nn
from CPML_PC import *
import numpy as np
import random

X_data=np.load("C:/Users/dexte/github/Chaos-Robotics/x_data.npy")
y=np.load("C:/Users/dexte/github/Chaos-Robotics/y_data.npy")
unseen_X=np.load("C:/Users/dexte/github/Chaos-Robotics/x_data_test.npy")
unseen_y=np.load("C:/Users/dexte/github/Chaos-Robotics/y_data_test.npy")

X_data=torch.tensor(X_data,dtype=torch.float32)
y_data=torch.tensor(y,dtype=torch.float32)

output_nodes=3
network=Network(output_nodes)
network.add_layer(2,act=torch.sigmoid)
network.add_layer(6,act=torch.sigmoid)
network.add_layer(6,act=torch.sigmoid)
network.show()

def run_trial(network):
    acc=0
    for dat,lab in zip(X_data,y_data):
        output=network.forward(dat)
        y_pred=output

        #get best
        if torch.equal(torch.argmax(y_pred),torch.argmax(lab)):
            acc+=1
    return acc/len(X_data)
def mutate(weights):
    noise=np.random.normal(0,0.5,weights.shape)
    return weights+noise

weights=network.get_weights()
print(run_trial(network))
w=mutate(weights[0])
print(weights[0].shape,w.shape,weights[1])
network.reform_weights(w,weights[1])
print(run_trial(network))

print(weights)
