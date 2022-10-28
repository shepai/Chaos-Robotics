import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button
import math as maths


# Define initial parameters
prandtl = 10 
rho = 28
beta = 8/3

def gen(prandtl,rho,beta):
    def lorenz_attr(x, y, z):
        x_dot = prandtl*(y - x)
        y_dot = rho*x - y - x*z
        z_dot = x*y - beta*z
        return x_dot, y_dot, z_dot

    dt = 0.01
    num_steps = 10000

    xs = np.empty(num_steps + 1)
    ys = np.empty(num_steps + 1)
    zs = np.empty(num_steps + 1)
    xs[0], ys[0], zs[0] = (0., 1., 1.05)

    for i in range(num_steps):
        x_dot, y_dot, z_dot = lorenz_attr(xs[i], ys[i], zs[i])
        xs[i + 1] = xs[i] + (x_dot * dt)
        ys[i + 1] = ys[i] + (y_dot * dt)
        zs[i + 1] = zs[i] + (z_dot * dt)
    return xs,ys,zs
# Create the figure and the line that we will manipulate
fig, ax = plt.subplots()
#fig.subplots_adjust(left=0.05, bottom=0.15, right=0.95, top=0.98, wspace=0.05, hspace=0.05)
xs,ys,zs=gen(prandtl,rho,beta)
line, = ax.plot(xs,ys, lw=2)
ax.set_xlabel('Iteration')

# adjust the main plot to make room for the sliders
fig.subplots_adjust(left=0.25, bottom=0.25)

# Make a horizontal slider to control the frequency.
axfreq = fig.add_axes([0.1, 0.1, 0.65, 0.03])

prand = Slider(
    ax=axfreq,
    label='prandtl',
    valmin=0.1,
    valmax=100,
    valinit=prandtl,
)

# Make a vertically oriented slider to control the amplitude
axamp = fig.add_axes([0.1, 0.15, 0.65, 0.03])
rh = Slider(
    ax=axamp,
    label="rho",
    valmin=0,
    valmax=100,
    valinit=rho,
)

# Make a vertically oriented slider to control the amplitude
axA = fig.add_axes([0.15, 0.25, 0.0225, 0.63])

bet = Slider(
    ax=axA ,
    label="beta",
    valmin=0,
    valmax=10,
    valinit=beta,
    orientation="vertical"
)


# The function to be called anytime a slider's value changes
def update(val):
    xs,ys,zs=gen(prand.val,rh.val,bet.val)
    line.set_ydata(ys)
    line.set_xdata(xs)
    fig.canvas.draw_idle()


# register the update function with each slider
prand.on_changed(update)
rh.on_changed(update)
bet.on_changed(update)

# Create a `matplotlib.widgets.Button` to reset the sliders to initial values.
resetax = fig.add_axes([0.8, 0.025, 0.1, 0.04])
button = Button(resetax, 'Reset', hovercolor='0.975')


def reset(event):
    prand.reset()
    rh.reset()
    bet.reset()
button.on_clicked(reset)

plt.show()