import sys
import gym
import gym
from gym import logger as gymlogger
import matplotlib.pyplot as plt
from IPython import display as ipythondisplay
from pyvirtualdisplay import Display
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D 
import random
from copy import deepcopy as dc

SIM='MountainCar-v0'

def run_trial(genotype,num_trials,show=True):
    env = gym.make(SIM,render_mode="rgb_array")
    observation, info = env.reset()
    rewards=0
    assert len(genotype)>=num_trials, "Make sure the num_trials is not greater than your genotype size"
    for _ in range(num_trials):
        action = int(genotype[_])
        observation, reward, terminated, truncated, info = env.step(action)
        rewards+=reward
        
        if terminated or truncated:
            observation, info = env.reset()
    if show:
        endObs=observation
        return rewards,endObs,env.render()
    env.close()
    endObs=observation
    return rewards,endObs

def visualise(genotype,num_trials):
    env = gym.make(SIM,render_mode="rgb_array")
    observation, info = env.reset()
    rewards=0
    assert len(genotype)>=num_trials, "Make sure the num_trials is not greater than your genotype size"
    for _ in range(num_trials):
        #action = env.action_space.sample()
        action = int(genotype[_])
        observation, reward, terminated, truncated, info = env.step(action)
        rewards+=reward
        plt.imshow(env.render())
        plt.pause(0.05)
        if terminated or truncated:
            observation, info = env.reset()
    env.close()
    return rewards

def lorenz_attr(x, y, z):
    x_dot = prandtl*(y - x)
    y_dot = rho*x - y - x*z
    z_dot = x*y - beta*z
    return x_dot, y_dot, z_dot
def get_velocities(prandtl,rho,beta):
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

def convert(signal,val=5):
    #make threshold with val and create a signal based off of the chaotic one
    new=np.zeros(signal.shape)
    new[np.argwhere(signal<=(val*-1))]=0
    new[np.argwhere(signal>(val*-1))]=1
    new[np.argwhere(signal>val)]=2
    return new
def fitness(observation):
    best=[0.6, 0.07]
    worst=[-1.2, -0.07]
    return ((observation[0]))/(best[0]+abs(worst[0]))#*0.5 + (abs(observaion[])/(best[1]+abs(worst[1])))*0.5


prandtl = 10
rho = 28
beta = 8/3 

GEN=100
best=-100
best_chaotic=None
saved=None
for rho in range(28):
    for prandtl in range(1,10):
        xs,ys,zs=get_velocities(prandtl,rho,beta)
        geno=convert(xs,val=5)
        reward,obs=run_trial(geno,GEN,show=False)
        reward=fitness(obs)
        #plt.cla()
        #plt.plot(geno[0:100])
        #plt.title("Gen %:"+str((rho+prandtl)/(10*28))+" Reward"+str(reward))
        plt.pause(0.05)
        if reward>=best:
            best=reward
            saved=dc(xs)
            best_chaotic=dc(geno) #deep copy the genotype
print("Reward sum:",best)
plt.cla()
reward,obs,t=run_trial(best_chaotic,GEN,show=True)

fig, axes = plt.subplots(1,3, figsize=(3.5, 2.0))
axes[0].set_title("Chaotic signal", loc="left")
axes[0].plot(saved[0:GEN])
axes[0].set_xlabel("Iteration")
axes[0].set_ylabel("Velocity")
axes[1].set_title("Chaotic signal conversion", loc="left")
axes[1].plot(best_chaotic[0:GEN])
axes[1].set_xlabel("Iteration")
axes[1].set_ylabel("Action")
axes[2].set_title("End step", loc="left")
axes[2].imshow(t)
axes[2].xaxis.set_visible(False)
axes[2].yaxis.set_visible(False)

plt.show()