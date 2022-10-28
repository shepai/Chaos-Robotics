import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button
import math as maths


# Define initial parameters
size=1000

c=10
a=50
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
fig, ax = plt.subplots()
#fig.subplots_adjust(left=0.05, bottom=0.15, right=0.95, top=0.98, wspace=0.05, hspace=0.05)
wave_x,wave_y=wave_generator(size,theta,a,c,dt)
line, = ax.plot(wave_x,wave_y, lw=2)
ax.set_xlabel('Iteration')

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
    line.set_ydata(wave_y)
    line.set_xdata(wave_x)
    ax.set_title(str(c_.val*dt_.val*a_.val))
    fig.canvas.draw_idle()


# register the update function with each slider
a_.on_changed(update)
c_.on_changed(update)
dt_.on_changed(update)

# Create a `matplotlib.widgets.Button` to reset the sliders to initial values.
resetax = fig.add_axes([0.8, 0.025, 0.1, 0.04])
button = Button(resetax, 'Reset', hovercolor='0.975')


def reset(event):
    a_.reset()
    c_.reset()
    dt_.reset()
button.on_clicked(reset)

plt.show()