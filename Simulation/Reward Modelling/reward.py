import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button
import math as maths
import random

# Define initial parameters
a=5
b=2
steps=650
lambda_=0.05
dt=0.01
pp=1

mus=np.zeros((steps,))
mus[0]=-1
x=np.zeros((2,steps))
x[0][0]=random.randint(0,2)
x[1][0]=random.randint(0,2)
Cp=np.zeros((2,steps))
Cp[0][0]=1
Cp[1][0]=1
thetas=np.random.random((2,1)) #bias terms
weights=np.random.random((2,2))
lower=1
outputs=np.zeros((2,steps))


def show_gen_control():
    mus=np.zeros((steps,))
    mus[0]=-1
    x=np.zeros((2,steps))
    x[0][0]=random.randint(0,2)
    x[1][0]=random.randint(0,2)
    Cp=np.zeros((2,steps))
    Cp[0][0]=1
    Cp[1][0]=1
    
    outputs=np.zeros((2,steps))

    for st in range(steps-2):
        """if i%1==0: #random inputs
            x[0][t+1]=random.randint(-3,3)
            x[1][t+1]=random.randint(-3,3)"""
        t=st+1
        step(0,t,mus,Cp,x,1)
        step(1,t,mus,Cp,x,0)
        outputs[0][t]=x[0][t+1]
        outputs[1][t]=x[1][t+1]
    
    return outputs[:,600:630],x[:,600:630]

#implementing this with random generation

def sigmoid(x):
    return (lower-1)+(1/(1+np.exp(-1*x)))

def step(i,t,mus,Cp,x,j):
    p=pp
    mus[t+1]=(mus[t-1]**p)+lambda_*(((x[0][t-1]-x[0][t-1-p])**2+(x[1][t-1]-x[1][t-1-p])**2)/p)
    Cp[i][t]=(mus[t])*((weights[i][i]*(x[i][t]-x[i][t-p]))+(weights[i][j]*(x[j][t]-x[j][t-p])))
    a=sigmoid(thetas[i]+(weights[i][i]*x[i][t])+(weights[i][j]*x[j][t])+Cp[i][t])
    x[i][t+1]=a
    




# Create the figure and the line that we will manipulate
fig, ax = plt.subplots()
#fig.subplots_adjust(left=0.05, bottom=0.15, right=0.95, top=0.98, wspace=0.05, hspace=0.05)
out,x=show_gen_control()
avg_out=((out[0]+out[1])/2)[:-1]
line, = ax.plot(avg_out, lw=2)
line2, = ax.plot(x[0],c="r",label="neuron 2", lw=2)
line3, = ax.plot(x[1],c="b",label="neuron 1", lw=2)
ax.set_xlabel('Iteration +600')
ax.set_ylabel('Neuron output')
ax.set_ylim(0,1)
ax.legend(loc="lower right")
# adjust the main plot to make room for the sliders
fig.subplots_adjust(left=0.25, bottom=0.25)

# Make a horizontal slider to control the frequency.
axfreq = fig.add_axes([0.1, 0.1, 0.65, 0.03])

freq_slider = Slider(
    ax=axfreq,
    label='lambda',
    valmin=0.01,
    valmax=1,
    valinit=lambda_,
)

# Make a vertically oriented slider to control the amplitude
axamp = fig.add_axes([0.1, 0.15, 0.65, 0.03])
amp_slider = Slider(
    ax=axamp,
    label="p",
    valmin=1,
    valmax=10,
    valinit=pp,
)


# The function to be called anytime a slider's value changes
def update(val):
    global lambda_
    global pp
    lambda_ = freq_slider.val
    pp=int(amp_slider.val)
    outputs,x=show_gen_control()
    avg_out=((outputs[0]+outputs[1])/2)[:-1]
    line.set_ydata(avg_out)
    line2.set_ydata(x[0])
    line3.set_ydata(x[1])
    fig.canvas.draw_idle()


# register the update function with each slider
freq_slider.on_changed(update)
amp_slider.on_changed(update)

# Create a `matplotlib.widgets.Button` to reset the sliders to initial values.
resetax = fig.add_axes([0.8, 0.025, 0.1, 0.04])
button = Button(resetax, 'Reset', hovercolor='0.975')
resetax1 = fig.add_axes([0.8, 0.001, 0.1, 0.04])
button1 = Button(resetax1, 'Reset weights', hovercolor='0.975')

def reset(event):
    freq_slider.reset()
    amp_slider.reset()

def reset_weight(event):
    thetas=np.random.random((2,1)) #bias terms
    weights=np.random.random((2,2))
    update(event)

button.on_clicked(reset)
button1.on_clicked(reset_weight)
plt.show()