"""
Simulation code for Nanosaur JRA adapted for PhD

Code By Dexter Shepherd

Evolving navigation via motor control and simulated radar.

uncomment anything to do with plt to make it run faster, as well as remove time.sleep
plt left in for visualisation tool.

"""

import matplotlib.pyplot as plt
import math
import time
import random
import copy

import cv2
import numpy as np
import copy
import torch


plt.ylim((-50,50))
plt.xlim((-50,50))

def getBump(a,points): #get whether it has met an obstacle
    if [a.x,a.y] in points:
        return True
    return False
def fitness(numBumps,timeGiven): #gather the fitness based on time and bumps
    #print((timeGiven-numBumps)/20)
    return (timeGiven-numBumps)/20

def mutation(gene, mean=0, std=0.1):
    gene = gene + np.random.normal(mean, std, size=gene.shape) #mutate the gene via normal 
    # constraint
    gene[gene > 4] = 4
    gene[gene < -4] = -4
    return gene

def crossover(loser, winner, p_crossover=0.5): #provide a crossover function
    for i,gene in enumerate(winner):
      if random.random() <= p_crossover:
        loser[i] = winner[i]
    return loser
    
class agent: #agent control in the simulation
    def __init__(self):
        self.x=0
        self.y=0
        self.vector=[0,0]
    def set_vector(self,x,y):
        self.vector=[x,y]
        self.x+=x
        self.y+=y
    def get_image(self,points,r=8): #radar simulation
        #get array facing the vector
        #(x-self.x)^2+ (y-self.y)^2 = 25
        arr=[]
        for j in range(r):
            toAdd=0
            for i in range(r):
                if [self.x+j-i,self.y+i] in points: toAdd=1
            arr.append(toAdd)
        for j in range(r):
            toAdd=0
            for i in range(r):
                if [self.x-i,self.y+j-i] in points: toAdd=1
            arr.append(toAdd)
        for j in range(r):
            toAdd=0
            for i in range(r):
                if [self.x-j-i,self.y-i] in points: toAdd=1
            arr.append(toAdd)
                
        for j in range(r):
            toAdd=0
            for i in range(r):
                if [self.x-j+i,self.y-j+i] in points: toAdd=1
            arr.append(toAdd)
        return np.array(arr)



def show_points(points,agent): #show points if you wish to visualize
    plt.cla()
    for p in points:
        plt.scatter(p[0],p[1],c="y")
    plt.scatter(agent.x,agent.y,c="r")
    plt.pause(0.1)



