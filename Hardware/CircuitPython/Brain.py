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
        self.values=[1,4,6,8,9]
        self.p=1 #initial value
        self.sensors=sensor_num
        self.outputs=motor_num
        self.post_process_bias=np.array([0.1,0.1]) #biases to control basic forward motion
        self.in_nuerons_weights=np.array([[-5.23159,-4.19076],[-5.23159,-5.19295]]) #cp.normal(0,2,size=(sensor_num,2))    
        self.in_nuerons_biases=np.array([[0.233793,-0.529649],[0.323159,0.218076]]) #cp.normal(-2,2,size=(sensor_num,2))/10
        self.out_nuerons_weights=np.array([[5.06038, -3.29649],[-5.00933, 5.00933]])#cp.normal(0,2,size=(sensor_num,2))    
        self.out_nuerons_biases=np.array([[0.122669,0.196101],[0.129649, 0.282701]])#cp.normal(-2,2,size=(sensor_num,2))/10
        self.thetas= np.array([[3.4],[3.8]])#np.random.normal(2,1.5,(2,1)) #bias terms
        self.weights= np.array([[-12,5.6],[0,-6.6]]) #np.random.normal(2,1.5,(2,2))
    def reset(self):
        self.mus=np.zeros((self.steps,))
        self.mus[0]=-1
        self.Cp=np.zeros((2,self.steps))
        self.Cp[0][0]=1
        self.Cp[1][0]=1
        self.x=np.zeros((2,self.steps))
        self.x[0][0]=1#random.randint(0,2)
        self.x[1][0]=1#random.randint(0,2)
        self.t=0
    def clearOld(self):
        #reset arrays but keep last item
        self.mus=np.zeros((self.steps,))+self.mus[-1]
        self.Cp=np.zeros((2,self.steps))+self.Cp[-1]
        self.x=np.zeros((2,self.steps))+self.x[-1]
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
        out=(self.in_nuerons_weights.transpose()*inputs )- self.in_nuerons_biases.transpose()
        #out[out<0]=0 #make sure non 0 is dealt with
        #assert (self.in_nuerons_weights.transpose()*inputs).shape==(self.in_nuerons_weights.transpose*inputs - self.in_nuerons_biases.transpose()).shape,"Assertion incorrect"
        return self.values[round(np.sum(self.sigmoid(out))/4 * len(self.values))] #get selected p value from range 9
    def B(self,outputs):
        #run neural post processing to get movement
        out=(self.out_nuerons_weights.transpose()*outputs )- self.out_nuerons_biases.transpose()
        return self.sigmoid(np.sum(out,axis=1))
    def run(self,inputs): #take in inputs and produce outputs
        assert len(inputs)==self.sensors,"incorrect sizing of inputs"
        #print(">>>",inputs,self.A(inputs))
        self.p=self.A(inputs) #run preprocessing

        self.t+=1
        self.step(self.t)
        #neural post processing
        out=self.x[:,self.t+1] 
        #print(">>>",self.x[:,self.t+1])
        self.out=self.B(out) #+ self.post_process_bias #get summed outputs of step
        #place through neural post processing
        #send to motors



