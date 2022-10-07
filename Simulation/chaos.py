import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D 

prandtl = 10 
rho = 28
beta = 8/3

def lorenz_attr(x, y, z):
    x_dot = prandtl*(y - x)
    y_dot = rho*x - y - x*z
    z_dot = x*y - beta*z
    return x_dot, y_dot, z_dot


for rho in range(28):
    for prandtl in range(0,100,5):
        #loop through search space
        dt = 0.01
        num_steps = 1000

        xs = np.empty(num_steps + 1)
        ys = np.empty(num_steps + 1)
        zs = np.empty(num_steps + 1)
        xs[0], ys[0], zs[0] = (0., 1., 1.05)

        for i in range(num_steps):
            x_dot, y_dot, z_dot = lorenz_attr(xs[i], ys[i], zs[i])
            xs[i + 1] = xs[i] + (x_dot * dt)
            ys[i + 1] = ys[i] + (y_dot * dt)
            zs[i + 1] = zs[i] + (z_dot * dt)
        #show in real time
        plt.cla()
        
        
        plt.plot(ys-2,c="r",label="y")
        plt.plot(zs-2,c="b",label="z")
        plt.plot(xs-2,c="g",label="x")
        plt.legend(loc="upper right")
        plt.xlabel("Time step")
        plt.ylabel("Velocity")
        plt.pause(0.05)
plt.show()