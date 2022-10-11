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
    observations=[]
    totalFit=-100
    best_obvs=[]
    for _ in range(num_trials):
        action = int(genotype[_])
        observation, reward, terminated, truncated, info = env.step(action) #step through with each action
        rewards+=reward
        observations.append(fitness(observation))
        if terminated or truncated: #environment finished
            observation, info = env.reset()
            if sum(observations)>=totalFit: #save best observaations
                totalFit=sum(observation)
                best_obvs=dc(observations) #copy obs
            observations=[]
    endObs=observation
    if show:
        return rewards,endObs,env.render()
    env.close()
    endObs=observation
    return rewards,best_obvs

def visualise(genotype,num_trials,filepath=""):
    env = gym.make(SIM,render_mode="rgb_array")
    observation, info = env.reset()
    rewards=-100
    assert len(genotype)>=num_trials, "Make sure the num_trials is not greater than your genotype size"
    observations=[]
    for _ in range(num_trials):
        #action = env.action_space.sample()
        action = int(genotype[_])
        observation, reward, terminated, truncated, info = env.step(action)
        rewards+=reward
        observations.append(fitness(observation))
        
        fig, axes = plt.subplots(3,1)
        #axes[0].figure(figsize=(5,1))
        axes[0].set_title("A: Chaotic signal", loc="left")
        axes[0].plot(saved[0:GEN])
        axes[0].set_xlabel("Iteration")
        axes[0].set_ylabel("Velocity")
        #axes[1].figure(figsize=(5,1))
        axes[1].set_title("B: Signal conversion", loc="left")
        axes[1].plot(best_chaotic[0:GEN])
        axes[1].set_xlabel("Iteration")
        axes[1].set_ylabel("Action")

        axes[2].set_title("C: Current Reward: "+str(sum(observations)), loc="left")
        axes[2].imshow(env.render())
        axes[2].xaxis.set_visible(False)
        axes[2].yaxis.set_visible(False)
        fig.tight_layout(pad=0.05)
        fig.savefig(filepath+"save"+str(_)+".png")
        plt.close()
        if terminated or truncated:
            observation, info = env.reset()
            observations=[]
    endObs=observation
    return rewards,endObs,env.render()

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

GEN=1000
best=-100
best_chaotic=None
saved=None
for rho in range(50):
    for prandtl in range(1,15):
        xs,ys,zs=get_velocities(prandtl,rho,beta)
        geno=convert(xs,val=5)
        reward,obs=run_trial(geno,GEN,show=False)
        reward=sum(obs)
        #plt.cla()
        #plt.plot(geno[0:100])
        #plt.title("Gen %:"+str((rho+prandtl)/(10*28))+" Reward"+str(reward))
        plt.pause(0.05)
        if reward>=best:
            best=reward
            saved=dc(xs)
            best_chaotic=dc(geno) #deep copy the genotype
print("Reward sum:",best)
plt.close()

reward,obs,t=visualise(best_chaotic,GEN,filepath="C:/Users/dexte/OneDrive/Pictures/Saved Pictures/PhD chaos/AutoGen/")


"""fig, axes = plt.subplots(1,3, figsize=(3.5, 2.0))
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
plt.show()"""