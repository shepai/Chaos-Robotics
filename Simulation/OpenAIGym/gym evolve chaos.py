import matplotlib.pyplot as plt
import numpy as np
from CPML_PC import Network, Brain
from copy import deepcopy
import gym
import random
import math as maths
import cv2

SIM='MountainCar-v0'

def run(agent,num_trials):
    env = gym.make(SIM,render_mode="rgb_array")
    observation, info = env.reset()
    victory=False
    _ =0
    route=[]
    while _<num_trials and not victory:
        action=agent.forward(observation)
        observation, reward, terminated, truncated, info = env.step(action) #step through with each action
        if terminated or truncated: #environment finished
            print(">>",observation)
            victory=True
        _+=1
        route.append(env.render())
    f=fitness(observation)
    observation, info = env.reset()
    return f*0.5 + ((num_trials-_)/num_trials)*0.5, route#take into account how quickly it did it

#create network for sensory controller
network=Network(2) #fdiffrent direction
network.add_layer(2)
network.add_layer(6)

out=Network(3) #fdiffrent direction
out.add_layer(2) #chaotic oscillation
out.add_layer(6)

Sennetwork=Network(3) #fdiffrent direction
Sennetwork.add_layer(2)
Sennetwork.add_layer(10)
Sennetwork.add_layer(6)

class controller:
    def __init__(self,sensor,chaos,output,chaotic=True):
        self.sensoryNN=sensor
        self.chaoticNN=chaos
        self.outputNN=output
        weights,idx=self.sensoryNN.get_weights()
        weights2,idx=self.outputNN.get_weights()
        if chaotic:
            self.shape_of_weights=(np.concatenate((weights,chaos.getWeights(),weights2))).shape
        else:
            self.shape_of_weights=((weights)).shape

        self.chaotic=chaotic
    def forward(self,input):
        #run through sensory processing
        outcome=self.sensoryNN.forward(input)
        ind=np.argmax(outcome)
        vectors=[[1,1],[1,0],[0,1],[-1,0],[0,-1],[1,-1],[-1,1],[-1,-1]]
        if self.chaotic:
            p_value=self.chaoticNN.values[ind] #select p value to control chaos
            #get chaotic generation
            self.chaoticNN.reset()
            for i in range(self.chaoticNN.steps//2):
                self.chaoticNN.runP(p_value)
            end_out=np.argmax(self.chaoticNN.out)
            #run through post porcessing
            inp=np.zeros(self.chaoticNN.out.shape)
            inp[end_out]=1
            #print(p_value,self.chaoticNN.out,inp)
            outcome=self.outputNN.forward(inp)
            ind=np.argmax(outcome)
        return ind
    def mutate_gene(self,gene):
        noise=np.random.normal(0,5,self.shape_of_weights) #add gaussian noise to the weights
        gene+=noise
        return gene
    def set_gene(self,gene):
        assert gene.shape==self.shape_of_weights, "Incorrect gene size" +str(gene.shape)+","+str(self.shape_of_weights) 
        weight,idx=self.sensoryNN.get_weights()
        weight2,idx2=self.outputNN.get_weights()
        chaos=self.chaoticNN.getWeights()
        #set the differnet network data
        #areas=random.choice([1,2,3,4,5,6,7]) #only change parts of the network
        if self.chaotic:
            #if areas in [1,4,6,7]:
            weight=gene[0:idx[-2]]
            #if areas in [2,4,5,7]:
            chaos=gene[idx[-2]:idx[-2]+6]
            #if areas in [3,5,6,7]:
            weight2=gene[idx[-2]+6:]
            self.sensoryNN.reform_weights(weight,idx)
            #self.chaoticNN.formWeights(chaos) #keep as normal
            self.outputNN.reform_weights(weight2,idx2)
        else:
            self.sensoryNN.reform_weights(gene,idx)

def fitness(obs):
    best=[0.6, 0.07]
    worst=[-1.2, -0.07]
    total=maths.sqrt((worst[0]-best[0])**2 + (worst[1]-best[1])**2)
    dist=maths.sqrt((obs[0]-best[0])**2 + (obs[1]-best[1])**2)
    return dist/total
def run_controller(chaos_):
    #create chaotic neuron pair
    chaos=Brain() 
    c=None
    if chaos_:
        c=controller(network,chaos,out,chaotic=chaos_)
    else: 
        c=controller(Sennetwork,chaos,out,chaotic=chaos_)
    
    #population of genotypes
    genotype = np.zeros((c.shape_of_weights))
    SIZE=100
    population=np.random.normal(0,20,(SIZE,genotype.shape[0]))+np.random.normal(0,2,(SIZE,genotype.shape[0]))
    #genetic algorithm
    Gen=200
    fitness=[0]
    bestPath=[]

    for i in range(Gen):
        #print("Generation:",i,"with fitness",fitness[-1])
        t1=random.randint(0,SIZE-1)
        t2 = t1+1 if (t1<SIZE-1) else t1-1 #local population tournaments
        #trial 1
        c.set_gene(population[t1])
        f1,p1=run(c,150)
        #trial 2
        c.set_gene(population[t2])
        f2,p2=run(c,150)
        #selection
        if f1>f2:
            population[t2]=deepcopy(c.mutate_gene(population[t2]))
        elif f2>f1:
            population[t1]=deepcopy(c.mutate_gene(population[t1]))
        else: #mutate both
            population[t2]=deepcopy(c.mutate_gene(population[t2]))
            population[t1]=deepcopy(c.mutate_gene(population[t1]))
        if f1>fitness[-1]:
            bestPath=deepcopy(p1)
        elif f1>fitness[-1]:
            bestPath=deepcopy(p2)
        fitness.append(max(fitness+[f1,f2]))
        print("Top:",fitness[-1])
    for i,rend in enumerate(bestPath):
        cv2.imwrite("C:/Users/dexte/OneDrive/Pictures/Saved Pictures/PhD chaos/AutoGen/",rend)
        """plt.imshow(rend)
        plt.title("Best")
        plt.pause(0.01)
        if i==0:
            plt.show() #wait for user to watch"""
    return fitness.index(fitness[-1]),fitness[-1] #get first occurence of best fitness

run_controller(False)