
import numpy as np
import matplotlib.pyplot as plt

filepath="C:/Users/dexte/OneDrive/Pictures/Saved Pictures/PhD chaos/AutoGen/"
size=1000
c=10
a=0.5
b=1

dt=0.005

dt=0.005

def bvp(x,y):
    x_d=c*(x-x**3 + y)
    xs=c*(1-x**2)*x_d - x
    #ys = -1* (x+b*y -a)/c
    ys=-x*a
    return xs, ys

def bvp2(x,y):
    x_d=c*(x-x**3 + y)
    xs=c*(1-x**2)*x_d - x
    ys = -1* (x+b*y -a)/c
    return xs, ys

for c in range(1,80):
    for a in range(1,10):
        x_=np.zeros((size,))
        y_=np.zeros((size,))
        x_[0], y_[0] = (0., 1.)
        for i in range(size-1):
            x_dot, y_dot=bvp(x_[i],y_[i])
            x_[i + 1] = x_[i] + (x_dot * dt)
            y_[i + 1] = y_[i] + (y_dot * dt)
        #plt.cla()
        #plt.title(str(c))
        plt.plot(x_,y_)
        plt.pause(0.05)
        plt.savefig(filepath+"save"+str(c)+".png")
