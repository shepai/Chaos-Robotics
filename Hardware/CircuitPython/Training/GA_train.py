
"""
This code is to be run on the PC side,
It makes use of the neural network training
"""
import torch
import torch.nn as nn
from CPML_PC import *
import numpy as np
import random
from copy import deepcopy

X_data=np.load("C:/Users/dexte/github/Chaos-Robotics/models/Data/modelsx_data.npy")
y=np.load("C:/Users/dexte/github/Chaos-Robotics/models/Data/modelsy_data.npy")
unseen_X=np.load("C:/Users/dexte/github/Chaos-Robotics/models/Data/modelsx_data_test.npy")
unseen_y=np.load("C:/Users/dexte/github/Chaos-Robotics/models/Data/modelsy_data_test.npy")

X_data=torch.tensor(X_data,dtype=torch.float32)
y_data=torch.tensor(y,dtype=torch.float32)

output_nodes=3


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
    noise=np.random.normal(0,2,weights.shape)
    return weights+noise



pop_size=20
population=[]
#generate population
for gen in range(pop_size):
    network=Network(output_nodes)
    network.add_layer(2,act=torch.sigmoid)
    network.add_layer(6,act=torch.sigmoid)
    population.append(deepcopy(network))

#genetic algorithm
gen=1000
fitness=0
bestInd=-1
for g in range(gen):
    #get genotypes
    n1=random.randint(0,pop_size-1)
    n2 = n1-1 if n1+1>=pop_size else n1+1
    net1=population[n1]
    net2=population[n2]
    #trial 1
    fit1=run_trial(net1)
    #trial 2
    fit2=run_trial(net2)
    if fit1>fit2: #wselection
        weights=deepcopy(net1.get_weights())
        w=mutate(weights[0])
        net2.reform_weights(w,weights[1])
    elif fit1<fit2: #selection
        weights=deepcopy(net1.get_weights())
        w=mutate(weights[0])
        net1.reform_weights(w,weights[1])
    if fit1>fitness:
        fitness=fit1
        bestInd=n1
    elif fit2>fitness:
        fitness=fit2
        bestInd=n2
    population[n1]=net1
    population[n2]=net2
    print("Gen",g,"Fitness",fitness,bestInd)

fit=0
for i in population:
    network = i
    temp=run_trial(network)
    if temp>fit:
        fit=temp
        network.save("parameters",path="C:/Users/dexte/github/Chaos-Robotics/models/")
print(fit)