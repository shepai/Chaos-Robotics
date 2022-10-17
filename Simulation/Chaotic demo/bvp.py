
import numpy as np
import matplotlib.pyplot as plt
import math as maths
filepath="C:/Users/dexte/OneDrive/Pictures/Saved Pictures/PhD chaos/AutoGen/"
size=2000
dt=0.005

def wave_generator(size,theta,a,c,dt,start=(0.,1.)):
    x_=np.zeros((size,))
    y_=np.zeros((size,))

    x_[0], y_[0] = start

    def bvp(x,y):
        x_d=c*(x-(x**3)/3 + y)
        xs=c*(1-x**2)*x_d - x
        #ys = -1* (x+b*y -a)/c
        ys=-x*a
        return xs, ys
    def rotate(x_,y_,theta):
        #rotate points
        for i in range(size):
            x_dot, y_dot=(x_[i],y_[i])
            d=maths.sqrt(x_dot**2 + y_dot**2)
            x_[i] = x_dot*maths.cos(theta) - y_dot*maths.sin(theta)
            y_[i] = y_dot*maths.cos(theta) + x_dot*maths.sin(theta)
        return x_,y_
    
    for i in range(size-1):
        x_dot, y_dot=bvp(x_[i],y_[i])
        x_[i + 1] = x_[i] + (x_dot * dt)
        y_[i + 1] = y_[i] + (y_dot * dt)
    
    #rotate
    x_,y_=rotate(x_,y_,theta)
    return x_,y_

def make(x,y,s,_):
    fig, axes = plt.subplots(2,1)
    #axes[0].figure(figsize=(5,1))
    axes[0].set_title("A: Chaotic generation", loc="left")
    axes[0].plot(x,y)
    #axes[0].set_xlabel("Iteration")
    #axes[0].set_ylabel("Velocity")
    #axes[1].figure(figsize=(5,1))
    axes[1].set_title("B: Signal conversion", loc="left")
    axes[1].plot(s)
    axes[1].set_xlabel("Iteration")
    axes[1].set_ylabel("Action")

    fig.tight_layout(pad=0.05)
    fig.savefig(filepath+"save"+str(_)+".png")
    plt.close()

count=0
theta=0
for c in range(1,30):
    #c*=0.1
    for a in range(1,10):
        #a*=0.1
        count+=1
        x_,y_=wave_generator(size,theta,a,c,dt,start=(0.,1.))
        make(x_,y_,x_,count)
    print((count/(30*10))*100,"%")
    plt.cla()
