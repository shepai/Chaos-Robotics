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
        axes[0].plot(chaotic[0:_])
        axes[2].xaxis.set_visible(False)
        axes[2].yaxis.set_visible(False)
        #axes[1].figure(figsize=(5,1))
        axes[1].set_title("B: Signal conversion", loc="left")
        axes[1].plot(genotype[0:_])
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


def sigmoid(x):
    with np.errstate(invalid='ignore'):
        return (1/(1+np.exp(-1*x)))


def step_e(i,t,mus,Cp,x,j,lambda_,p):
    global p_count
    #apply mathematic formula
    with np.errstate(invalid='ignore'):
        mus[t+1]=(mus[t-1]**p)+lambda_*(((x[0][t-1]-x[0][t-1-p])**2+(x[1][t-1]-x[1][t-1-p])**2)/p) 
        Cp[:,t]=(mus[t])*np.sum(weights*(x[:,t]-x[:,t-p]),axis=1) 
    a=np.dot(weights,x[:,t])
    x[i][t+1]=sigmoid(thetas[i]+a[i]+Cp[i][t])
    x[j][t+1]=sigmoid(thetas[j]+a[j]+Cp[j][t])
    return x
def run(steps,p,lambda_=0.05):
    global thetas
    global weights
    #generate all the arrays, weights and biases
    mus=np.zeros((steps,))
    mus[0]=-1
    x=np.zeros((2,steps))
    x[0][0]=random.randint(0,2)
    x[1][0]=random.randint(0,2)
    Cp=np.zeros((2,steps))
    Cp[0][0]=1
    Cp[1][0]=1
    thetas=np.random.normal(1,0.5,(2,1)) #bias terms
    weights=np.random.normal(1,0.5,(2,2))
    t=1
    for st in range(steps-2): #loop through steps
        t=st+1
        x=step_e(0,t,mus,Cp,x,1,lambda_,p) #get neuron outputs
    avg_out=((x[0]+x[1])/2)[:-1] #get overall averages
    return avg_out,mus

steps=100
genotype=np.array([random.randint(1,9) for i in range(steps)])
new_geno=np.zeros((steps**2,))
#count differences
diff=(np.diff(genotype)!=0).sum()
l=0.05
#steps*=100 #increase overall size
for i in range(len(genotype)):
    avg_out,mus=run(steps,genotype[i],lambda_=l)
    new_geno[steps*i:min(steps+steps*i -1,len(new_geno)-1)]=avg_out[0:steps-1]
print(genotype)
def convertDecision(signal):
    #make threshold with val and create a signal based off of the chaotic one
    if np.max(signal)>1:
        signal/=np.max(signal)
    new=np.zeros(signal.shape)
    new[np.argwhere(signal<=(0.3))]=0
    new[np.argwhere(signal>(0.3))]=1
    new[np.argwhere(signal>0.6)]=2 #set by threshold
    return new

decision=convertDecision(new_geno[0:500])
genotype=decision.reshape((decision.shape[0],1))
c=visualise(new_geno,genotype,len(genotype),filepath=filepath)
plt.imshow(c)
plt.show()