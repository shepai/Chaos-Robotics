"""
BVP generation code for python
This variant uses the compiled library written in C++

Code by Dexter R Shepherd 26/10/2022

"""

import numpy as np
import bvp

import matplotlib.pyplot as plt

b=bvp.BVP()
fig, axes = plt.subplots(2,1)
SIZE=600
signal=np.zeros((SIZE,))
i=0
for c in range(1,10):
    
    for a in range(1,20):
        x=np.asarray(b.gen(10000,c,a,0.005)).view(np.float32)

        x=x.reshape(x.shape[0]//2,2)
        xs=x[:,0]
        ys=x[:,1]
        signal[i:100+i]=xs[0:100]
        i+=100
        if i+100>SIZE:
            i=0
        #axes[0].figure(figsize=(5,1))
        axes[0].clear()
        axes[1].clear()
        
        axes[0].set_title("A: Chaotic generation", loc="left")
        axes[0].plot(xs,ys)
        axes[1].set_title("B: Signal conversion n=100", loc="left")
        axes[1].plot(signal)
        axes[1].set_xlabel("Iteration")
        axes[1].set_ylabel("Action")
        plt.pause(0.05)
    

#print(xs,ys)
plt.show()

