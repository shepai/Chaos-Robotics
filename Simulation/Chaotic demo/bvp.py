
import numpy as np
import matplotlib.pyplot as plt

filepath="C:/Users/dexte/OneDrive/Pictures/Saved Pictures/PhD chaos/AutoGen/"
size=2000
dt=0.005

def bvp(x,y):
    x_d=c*(x-(x**3)/3 + y)
    xs=c*(1-x**2)*x_d - x
    #ys = -1* (x+b*y -a)/c
    ys=-x*a
    return xs, ys

count=0
for c in range(1,30):
    #c*=0.1
    for a in range(1,10):
        #a*=0.1
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
        plt.savefig(filepath+"save"+str(count)+".png")
        count+=1
    plt.cla()
