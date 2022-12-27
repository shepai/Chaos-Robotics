"""
Circuit Python machine learning tool kit for PC

This library combines and provides functionality to copy what you are doing on the device

Library by Dexter R. Shepherd
University of Sussex PhD student

"""

import numpy as np
import random
import math as maths
import pandas as pd 


"""
generate a layer to hold information on network
@param: nodes_in is the number of inputs to this layer
@param: nodes_out is the number of nodes in the next layer
@param: vals is whether the user wishes to manually set the weights
"""
class Layer:
    def __init__(self,nodes_in,nodes_out,vals=None,activ=None):
        if type(vals)==type(None):
            self.matrix=np.random.random(size=(nodes_out,nodes_in))*1.2 #generate random weights
        else:
            self.matrix=vals.reshape((nodes_out,nodes_in)) #generate set weights
        self.vals=vals
        self.bias=None
        self.matrix=self.matrix #convert to tensor
        self.activation_func=activ
        if type(activ)==type(None):
            self.activation_func=self.activation_
        self.a = 0 # defines the output of the layer after running through activation
        self.z = 0 # defines the input of layer to the activation function#
        self.i=nodes_in
        self.j=nodes_out
    def getShape(self): #return the shape of the matrix
        return self.matrix.shape
    def setBias(self,bias):
        if bias!=None:
            self.bias=bias
        else: self.bias==None
    def length(self):
        s=0
        if self.bias!=None: s+=len(self.bias.flatten())
        return s+(self.i*self.j)
    def activation_(self,inputs):
        #activation functions
        self.z=inputs
        self.a = 1/(1 + np.exp(-self.z))
        return self.a
    def activation_grad(self):
        return self.a * (1 - self.a)   
    def setWeight(self,val):
        val=val.reshape(self.getShape())
        self.matrix=val
        
"""
The network that combines all the layers together
@param: num_out is how many nodes in the output layer
"""
class Network:
    def __init__(self,num_out): 
        self.network=[]
        self.num_out=num_out
        self.net=[]
    def add_layer(self,nodes,vals=None,act=None):
        layer=Layer(nodes,self.num_out,vals=vals,activ=act) #default x by y
        if len(self.network)>0: #there are previous nodes
            layer=self.network[-1]
            bias=self.network[-1].bias
            activation=layer.activation_func
            num=layer.getShape()
            val=layer.vals
            layer=Layer(num[1],nodes,vals=val,activ=activation)
            layer.setBias(bias)
            self.network[-1]=layer #correct output of matrices before
            layer=Layer(nodes,self.num_out,vals=vals,activ=act) #generate layer with correct matrices
        self.network.append(layer) #add the layer to the network
    def add_bias(self,vals=None):
        assert len(self.network)>0, "Network is empty. Add layers"
        size=self.network[-1].getShape() #get the end sizing to add on
        if type(vals)==type(None):
            vals=np.random.random((size,1))
        self.network[-1].setBias(vals) #set the bias in the current end layer
    def forward(self,inp):
        x=inp
        #self.network[0].a=x.copy()
        for i in range(len(self.network)):
            #print(self.network[i].matrix.shape,x.shape)
            x=np.dot(self.network[i].matrix,x)
            if type(self.network[i].bias)!=type(None):
                x+=self.network[i].bias
            x=self.network[i].activation_func(x)
            self.network[i].a=x.copy()
        return x
    def show(self):
        #show all the network layers and biases
        for i in range(len(self.network)):
            print("Layer",i+1,", nodes:",self.network[i].getShape(),", biases:",self.network[i].bias)
    def get_weights(self):
        indicies=[0] #store encoded indices
        ind=[0] #store active indicies
        s=0
        si=0
        for i in range(len(self.network)):
            si+=self.network[i].length()
        wb=np.zeros((si))
        for i,layer in enumerate(self.network): #perform calculations
            indicies.append(s+len(layer.matrix.flatten())) #add each index
            ind.append(s+len(layer.matrix.flatten())) #add each index
            #print(ind[i],ind[i+1],len(wb[ind[i]:ind[i+1]]))
            wb[ind[i]:ind[i+1]]=layer.matrix.flatten()
            if type(layer.bias)!=type(None):
                indicies.append(s+len(layer.bias.flatten())) #add each index
                ind.append(s+len(layer.bias.flatten())) #add each index
                wb[ind[i+1]:ind[i+2]]=layer.flatten()
                s+=len(layer.bias.flatten())
            else: indicies.append(-1) #none found
            s+=len(layer.matrix.flatten())
        #indicies.append(s)
        return wb, indicies
    def reform_weights(self,wb,ind):
        back=ind[0]
        biases=[2+(2*i) for i in range(((len(ind)-1)//2))]
        layer=0
        for i in range(1,len(ind)):
            front=ind[i]
            if i in biases: #bias term
                if ind[i]!=-1:
                    self.network[layer].bias=wb[back:front]
                    back=ind[i]
                layer+=1
            else: #weight term
                self.network[layer].setWeight(wb[back:front])
                back=ind[i]
        
    def parameters(self):
        n=[]
        for i,layer in enumerate(self.network): #perform calculations
            n.append(self.network[i].matrix)
            if self.network[i].bias!=None:
                n.append(self.network[i].bias)
        return n
    def save(self,name,path=""):
        name=name.replace(".csv","")
        wb,ind=self.get_weights() #gather
        pd.DataFrame(wb).to_csv(path+name+".csv", header=None, index=None)
        pd.DataFrame(np.array(ind)).to_csv(path+"meta_"+name+".csv", header=None, index=None)


class Brain:
    def __init__(self,lamda_=0.05,size=1000):
        #set up the inputs, cpg and output
        self.steps=size
        self.reset()
        self.lambda_=lamda_
        self.values=[1,4,6,8,9]
        self.p=1 #initial value
        self.thetas= np.array([[3.4],[3.8]])#np.random.normal(2,1.5,(2,1)) #bias terms
        self.weights= np.array([[-12,5.6],[0,-6.6]]) #np.random.normal(2,1.5,(2,2))
    def reset(self):
        self.mus=np.zeros((self.steps,))
        self.mus[0:9]=-1
        self.Cp=np.zeros((2,self.steps))
        self.Cp[0][0:9]=1
        self.Cp[1][0:9]=1
        self.x=np.zeros((2,self.steps))
        self.x[0][0:9]=1#random.randint(0,2)
        self.x[1][0:9]=1#random.randint(0,2)
        self.t=8
    def formWeights(self,nparray):
        nparray=nparray.flatten() #size 6
        weights=nparray[0:4]
        thetas=nparray[4:]
        self.thetas=thetas.reshape((2,1))
        self.weights=weights.reshape((2,2))
    def getWeights(self):
        #print(self.weights.flatten(),self.thetas.flatten())
        return np.concatenate((self.weights.flatten(),self.thetas.flatten()))
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
    def runP(self,p):
        self.p=p
        self.t+=1
        if self.p>self.t: self.p=1
        self.step(self.t)
        out=self.x[:,self.t+1] 
        #print(">>>",self.x[:,self.t+1])
        self.out=out

