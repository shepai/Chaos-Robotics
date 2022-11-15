#main code
import CPML as cp
import ulab.numpy as np
import random
import math as maths

class Brain:
    def __init__(self,sensor_num,motor_num,lamda_=0.05,size=1000):
        #set up the inputs, cpg and output
        self.steps=size
        self.reset()
        self.lambda_=lamda_
        self.p=1 #initial value
        self.sensors=sensor_num
        self.outputs=motor_num
        self.post_process_bias=np.array([-0.5,-0.6])
        self.in_nuerons_weights=cp.normal(0,1,size=(sensor_num,2))    
        self.in_nuerons_biases=cp.normal(0,1,size=(sensor_num,2))/10
    def reset(self):
        self.mus=np.zeros((self.steps,))
        self.mus[0]=-1
        self.Cp=np.zeros((2,self.steps))
        self.Cp[0][0]=1
        self.Cp[1][0]=1
        self.x=np.zeros((2,self.steps))
        self.x[0][0]=1#random.randint(0,2)
        self.x[1][0]=1#random.randint(0,2)
        self.thetas= np.array([[-3.4],[3.8]])#np.random.normal(2,1.5,(2,1)) #bias terms
        self.weights= np.array([[-22,5.6],[0,-6.6]]) #np.random.normal(2,1.5,(2,2))
        self.t=0
    def sigmoid(self,x):
        return (1/(1+np.exp(-1*x)))
    def step(self,t):
        #apply mathematic formula to make a step
        self.mus[t+1]=(self.mus[t-1]**self.p)+self.lambda_*(((self.x[0][t-1]-self.x[0][t-1-self.p])**2+(self.x[1][t-1]-self.x[1][t-1-self.p])**2)/self.p) 
        self.Cp[:,t]=(self.mus[t])*np.sum(self.weights*(self.x[:,t]-self.x[:,t-self.p]),axis=1) 
        a=np.dot(self.weights,self.x[:,0])
        self.x[0][t+1]=self.sigmoid(self.thetas[0]+a[0]+self.Cp[0][t])
        self.x[1][t+1]=self.sigmoid(self.thetas[1]+a[1]+self.Cp[1][t])
    def A(self,inputs):
        #run neural preprocessing on inputs and generate p value
        out=(self.in_nuerons_weights.transpose()*inputs - self.in_nuerons_biases.transpose()) 
        #out[out<0]=0 #make sure non 0 is dealt with
        #assert (self.in_nuerons_weights.transpose()*inputs).shape==(self.in_nuerons_weights.transpose*inputs - self.in_nuerons_biases.transpose()).shape,"Assertion incorrect"
        return round(np.sum(out))
    def run(self,inputs): #take in inputs and produce outputs
        assert len(inputs)==self.sensors,"incorrect sizing of inputs"
        self.p=self.A(inputs) #run preprocessing
        if self.p>self.t: self.p=self.t #not exceed
        if self.p<=0: self.p=1 #non zero
        self.t+=1
        self.step(self.t)
        #neural post processing
        out=self.x[:,self.t+1] + self.post_process_bias #get summed outputs of step
        self.out=out
        #place through neural post processing
        #send to motors



