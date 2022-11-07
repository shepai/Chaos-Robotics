import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button
import math as maths


# Define initial parameters
a=5
b=2
size=100
noise=0.5
noises=0.5
sensory_sig=np.random.normal(5,noises,(size,))
sensory_want=np.zeros((size,))+noise#np.random.normal(5,0.5,(size,))

#t = np.linspace(0, 1, size)
sig=np.zeros((size,))
def show_gen_control(sensory_sig,sensory_want,a,b,sig):
    for i in range(size):
        I=(sensory_sig[i]-np.average(sensory_sig[i]))*maths.log(1+(maths.e**(a)))+(np.average(sensory_sig[i])+b)
        sig[i]=I
    return sig
# Create the figure and the line that we will manipulate
fig, ax = plt.subplots()
#fig.subplots_adjust(left=0.05, bottom=0.15, right=0.95, top=0.98, wspace=0.05, hspace=0.05)
line, = ax.plot(show_gen_control(sensory_sig,sensory_want,a,b,sig), lw=2)
ax.set_xlabel('Iteration')

# adjust the main plot to make room for the sliders
fig.subplots_adjust(left=0.25, bottom=0.25)

# Make a horizontal slider to control the frequency.
axfreq = fig.add_axes([0.1, 0.1, 0.65, 0.03])

freq_slider = Slider(
    ax=axfreq,
    label='alpha',
    valmin=0.1,
    valmax=100,
    valinit=a,
)

# Make a vertically oriented slider to control the amplitude
axamp = fig.add_axes([0.1, 0.15, 0.65, 0.03])
amp_slider = Slider(
    ax=axamp,
    label="beta",
    valmin=0,
    valmax=100,
    valinit=b,
)

# Make a vertically oriented slider to control the amplitude
axA = fig.add_axes([0.15, 0.25, 0.0225, 0.63])

sens_slider = Slider(
    ax=axA ,
    label="s_hat",
    valmin=0,
    valmax=10,
    valinit=noise,
    orientation="vertical"
)

axB = fig.add_axes([0.2, 0.25, 0.0225, 0.63])

sens_sliderB = Slider(
    ax=axB ,
    label="s",
    valmin=0,
    valmax=10,
    valinit=noises,
    orientation="vertical"
)
# The function to be called anytime a slider's value changes
def update(val):
    line.set_ydata(show_gen_control(sensory_sig,sensory_want, freq_slider.val, amp_slider.val,sig))
    fig.canvas.draw_idle()

def updateNoise(val):
    global sensory_want
    global sensory_sig
    sensory_want=np.random.normal(5,sens_slider.val,(size,))#np.zeros((size,))+sens_slider.val#np.random.normal(5,0.5,(size,))
    sensory_sig=np.random.normal(5,sens_sliderB.val,(size,))
    update(val)
# register the update function with each slider
freq_slider.on_changed(update)
amp_slider.on_changed(update)
sens_slider.on_changed(updateNoise)
sens_sliderB.on_changed(updateNoise)
# Create a `matplotlib.widgets.Button` to reset the sliders to initial values.
resetax = fig.add_axes([0.8, 0.025, 0.1, 0.04])
button = Button(resetax, 'Reset', hovercolor='0.975')


def reset(event):
    freq_slider.reset()
    amp_slider.reset()
button.on_clicked(reset)

plt.show()