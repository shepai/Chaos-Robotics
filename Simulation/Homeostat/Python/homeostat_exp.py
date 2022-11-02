import sys
import copy as cp
import matplotlib.pyplot as plt

# relative path to folder which contains the Sandbox module
sys.path.insert(1, '../..')
#from Sandbox import *

from Homeostat import *

# Homeostat parameters
n_units = 1
upper_limit = 20
lower_limit = -20
upper_viability = 1
lower_viability = -1
adapt_fun = random_val
weights_set = None
test_interval = 10

# # uncomment to use a discrete weight set
# adapt_fun = random_selector
# weights_set = np.linspace(-1, 1, 26)

# Simulation parameters
t = 0
ts = [t]
dt = 0.05
duration = 100
# construct Homeostat
homeostat = Homeostat(n_units=n_units, upper_limit=upper_limit, lower_limit=lower_limit, upper_viability=upper_viability, lower_viability=lower_viability, adapt_fun=adapt_fun, weights_set=weights_set, test_interval=test_interval)

# randomise parameters for system equations
# homeostat.randomise_params()
count=0
# main Homeostat simulation loop
while t < duration:
    homeostat.step(dt)
    t += dt
    ts.append(t)
    count+=1
print(count+1)
# plot Homeostat essential variables and weights over time
fig, ax = plt.subplots(2, 1)

# plot all homeostat unit variables over time
for i, unit in enumerate(homeostat.units):
    ax[0].plot(ts, unit.thetas, label='Unit ' + str(i) + ': essential variable')
    print(len(unit.thetas))
ax[0].plot([ts[0], ts[-1]], [upper_viability, upper_viability], 'r--', label='upper viable boundary')
ax[0].plot([ts[0], ts[-1]], [lower_viability, lower_viability], 'g--', label='lower viable boundary')
ax[0].set_title('Essential variables')
ax[0].set_xlabel('t')
ax[0].set_ylabel('Essential variable')
ax[0].legend()

#print(len(ts),unit.weights_hist)

# plot all homeostat unit weights over time
for i, unit in enumerate(homeostat.units):
    ax[1].plot(ts, unit.weights_hist, label='Unit ' + str(i) + ': weight')
ax[1].set_title('Connection weights')
ax[1].set_xlabel('t')
ax[1].set_ylabel('Weight')
ax[1].legend()

plt.show()


