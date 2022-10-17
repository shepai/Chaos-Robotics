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
import math as maths

filepath="C:/Users/dexte/OneDrive/Pictures/Saved Pictures/PhD chaos/AutoGen/"

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

def visualise(chaotic,genotype,num_trials,filepath="",external_count=None):
    env = gym.make(SIM,render_mode="rgb_array")
    observation, info = env.reset()
    rewards=0
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
        axes[0].plot(chaotic[0],chaotic[1])
        axes[2].xaxis.set_visible(False)
        axes[2].yaxis.set_visible(False)
        #axes[1].figure(figsize=(5,1))
        axes[1].set_title("B: Signal conversion", loc="left")
        axes[1].plot(genotype[0:GEN])
        axes[1].set_xlabel("Iteration")
        axes[1].set_ylabel("Action")

        axes[2].set_title("C: Current Reward: "+str(sum(observations)), loc="left")
        axes[2].imshow(env.render())
        axes[2].xaxis.set_visible(False)
        axes[2].yaxis.set_visible(False)
        fig.tight_layout(pad=0.05)
        if external_count!=None: #allow big visulatisation
            _=external_count+_
        fig.savefig(filepath+"save"+str(_)+".png")
        plt.close()
        if terminated or truncated:
            observation, info = env.reset()
            observations=[]
    endObs=observation
    return env.render()

def wave_generator(size,theta,a,c,dt,start=(0.,1.)):
    x_=np.zeros((size,))
    y_=np.zeros((size,))

    x_[0], y_[0] = start

    def bvp(x,y):
        x_d=c*(x-(x**3)/3 + y)
        xs=c*(1-x**2)*x_d - x
        #ys = -1* (x+b*y -a)/c
        ys=-x*a
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
def convert(signal,val=5):
    #make threshold with val and create a signal based off of the chaotic one
    new=np.zeros(signal.shape)
    new[np.argwhere(signal<=(val*-1))]=0
    new[np.argwhere(signal>(val*-1))]=1
    new[np.argwhere(signal>val)]=2 #set by threshold
    return new

def fitness(observation): #get the fitness based on how far along the plane it is
    best=[0.6, 0.07]
    worst=[-1.2, -0.07]
    return ((observation[0]))/(best[0]+abs(worst[0]))#*0.5 + (abs(observaion[])/(best[1]+abs(worst[1])))*0.5


c=0.5
a=10
#b=100
theta=maths.pi/4
dt=0.005

GEN=500
best=-100
best_chaotic=None
count=0
#loop through diffrent parameters
for c in range(1,10):
    #c*=0.1
    for a in range(1,30):
        for theta in range(1,10):
            theta=maths.pi/theta
            x_,y_=wave_generator(GEN,theta,a,c,dt,start=(0.,1.))
            geno=convert(x_,val=0.5)
            #print(geno)
            reward,obs=run_trial(geno,GEN,show=False) #run a simulated trial
            reward=sum(obs)
            if reward>=best:
                best=reward
                best_chaotic=dc(geno) #deep copy the genotype
            count+=1
    print((count/(10*30*10))*100,"%")
print("Reward sum:",best)
#reward=run_trial(best_chaotic,100)
im=visualise((x_,y_),best_chaotic,GEN,filepath=filepath)
plt.imshow(im)
plt.show()