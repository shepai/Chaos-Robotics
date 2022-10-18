"""

Code to plot the mvoement of an individual genotype 
This genotype will be generated and placed in a numpy file of shape (length,)

"""


import os
import cv2 as cv
import re 
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


file_path="C:/Users/dexte/github/Chaos-Robotics/models/bvp_signal.npy" #preset signal file
out_file="C:/Users/dexte/OneDrive/Pictures/Saved Pictures/PhD chaos/AutoGen/" #where to save outfiles


SIM='MountainCar-v0'

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
        axes[0].plot(chaotic)
        axes[2].xaxis.set_visible(False)
        axes[2].yaxis.set_visible(False)
        #axes[1].figure(figsize=(5,1))
        axes[1].set_title("B: Signal conversion", loc="left")
        axes[1].plot(genotype)
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


# The code for loading and converting
# depending on the size you will want to edit the val to be 'reasonable'

signal=np.load(file_path)

genotype=convert(signal,val=0.25)

# visualise and save the images
visualise(signal,genotype,len(genotype),filepath=out_file,external_count=None)
print("Done")