import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button
import math as maths
import sys
import gym
from gym import logger as gymlogger
from IPython import display as ipythondisplay
from pyvirtualdisplay import Display
from copy import deepcopy as dc

SIM='MountainCar-v0'
env = gym.make(SIM,render_mode="rgb_array")
env.reset()
# Define initial parameters
size=1000
index=1
c=2
a=26
#b=100
theta=maths.pi/8
dt=0.005
def wave_generator(size,theta,a,c,dt,start=(0.,1.)):
    x_=np.zeros((size,))
    y_=np.zeros((size,))
    
    x_[0], y_[0] = start

    def bvp(x,y):
        try:
            x_d=c*(x-(x**3)/3 + y)
            xs=c*(1-x**2)*x_d - x
            #ys = -1* (x+b*y -a)/c
            ys=-x*a
        except RuntimeWarning:
            print("error")
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



# Create the figure and the line that we will manipulate
fig, ax = plt.subplots(1,3)
#fig.subplots_adjust(left=0.05, bottom=0.15, right=0.95, top=0.98, wspace=0.05, hspace=0.05)
wave_x,wave_y=wave_generator(size,theta,a,c,dt)
line, = ax[0].plot(wave_x,wave_y, lw=2)
ax[0].set_xlabel('Iteration')
ax[1].imshow(env.render())
ax[2].plot(wave_x[0:100])
# adjust the main plot to make room for the sliders
fig.subplots_adjust(left=0.25, bottom=0.25)

# Make a horizontal slider to control the frequency.
axfreq = fig.add_axes([0.1, 0.1, 0.65, 0.03])

a_ = Slider(
    ax=axfreq,
    label='a',
    valmin=0.1,
    valmax=50,
    valinit=a,
)

# Make a vertically oriented slider to control the amplitude
axamp = fig.add_axes([0.1, 0.15, 0.65, 0.03])
c_ = Slider(
    ax=axamp,
    label="c",
    valmin=0,
    valmax=15,
    valinit=c,
)

# Make a vertically oriented slider to control the amplitude
axA = fig.add_axes([0.15, 0.25, 0.0225, 0.63])

dt_ = Slider(
    ax=axA ,
    label="dt",
    valmin=0,
    valmax=1,
    valinit=dt,
    orientation="vertical"
)


# The function to be called anytime a slider's value changes
def update(val):
    wave_x,wave_y=wave_generator(size,theta,a_.val,c_.val,dt_.val)
    ax[2].cla()
    ax[2].plot(wave_x[0:100])
    line.set_ydata(wave_y)
    line.set_xdata(wave_x)
    ax[0].set_title(str(c_.val*dt_.val*a_.val))
    fig.canvas.draw_idle()


# register the update function with each slider
a_.on_changed(update)
c_.on_changed(update)
dt_.on_changed(update)

# Create a `matplotlib.widgets.Button` to reset the sliders to initial values.
resetax = fig.add_axes([0.8, 0.035, 0.1, 0.04])
button = Button(resetax, 'Reset', hovercolor='0.975')
resetax2 = fig.add_axes([0.8, 0.005, 0.1, 0.04])
button2 = Button(resetax2, 'Step', hovercolor='0.975')
def convert(signal,val=5):
    #make threshold with val and create a signal based off of the chaotic one
    new=np.zeros(signal.shape)
    new[np.argwhere(signal<=(val*-1))]=0
    new[np.argwhere(signal>(val*-1))]=1
    new[np.argwhere(signal>val)]=2 #set by threshold
    return new
def reset(event):
    global index
    env.reset()
    a_.reset()
    c_.reset()
    dt_.reset()
    index=0
def step(event):
    global index
    for i in range(5):  #take 10 steps
        wave_x,wave_y=wave_generator(size,theta,a_.val,c_.val,dt_.val)
        action = int(convert(wave_x,val=2)[index])
        observation, reward, terminated, truncated, info = env.step(action) #step through with each action
        if terminated or truncated: #environment finished
            observation, info = env.reset()
        index+=1
        if index>=size: #reset if too large
            index=0
    ax[1].imshow(env.render())
button.on_clicked(reset)
button2.on_clicked(step)
plt.show()

def fitness(observation):
    best=[0.6, 0.07]
    worst=[-1.2, -0.07]
    return ((observation[0]))/(best[0]+abs(worst[0]))#*0.5 + (abs(observaion[])/(best[1]+abs(worst[1])))*0.5


