
import numpy as np
import matplotlib.pyplot as plt

size=100

dt=0.01
def bvp(x,y):
        x_d=y-(c*x**3)/(3) + 3*x
        xs=c*(1-x**2)*x_d - x
        ys = -1* y
        return xs, ys

for c in range(1,80):
    x_=np.zeros((size,))
    y_=np.zeros((size,))
    x_[0], y_[0] = (0., 1.)
    for i in range(size-1):
        x_dot, y_dot=bvp(x_[i],y_[i])
        x_[i + 1] = x_[i] + (x_dot * dt)
        y_[i + 1] = y_[i] + (y_dot * dt)
        
    plt.plot(x_,y_)
    plt.pause(0.5)
