import numpy as np


"""
This code is to be run on the PC side,
It makes use of the neural network training
"""
import torch
import torch.nn as nn
from CPML_PC import *
import numpy as np
import random

#make classes

data_set_size=200 #define size of data

X=np.random.random_sample((data_set_size,2))
y=np.zeros((data_set_size,3))

unseen_X=np.random.random_sample((data_set_size,2,1))
unseen_y=np.zeros((data_set_size,3))

def convert(X,y):
    #create the labels based on logic
    for i,arr in enumerate(X):
        if in_range(arr[0],arr[1]):
            y[i][0]=1
        elif arr[0]>arr[1]:
            y[i][1]=1
        elif arr[1]>arr[0]:
            y[i][2]=1
    return y
def in_range(num1,num2,r=0.1):
    if num1-r<=num2 and num1+r>=num2:
        return True
    return False

y=convert(X,y)
unseen_y=convert(unseen_X,unseen_y)

path="C:/Users/dexte/github/Chaos-Robotics/models"
np.save(path+"x_data",X)
np.save(path+"y_data",y)
np.save(path+"x_data_test",unseen_X)
np.save(path+"y_data_test",unseen_y)

import pandas as pd 
pd.DataFrame(X).to_csv(path+"x_data.csv", header=None, index=None)
pd.DataFrame(y).to_csv(path+"y_data.csv", header=None, index=None)


