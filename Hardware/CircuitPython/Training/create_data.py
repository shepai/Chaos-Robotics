import numpy as np


"""
This code is to be run on the PC side,
It makes use of the neural network training
"""
import torch
import torch.nn as nn
from CPML_PC import *
import numpy as np
import random

#make classes

data_set_size=200 #define size of data

X=np.random.random_sample((data_set_size,2,1))
y=np.zeros((data_set_size,3))

unseen_X=np.random.random_sample((data_set_size,2,1))
unseen_y=np.zeros((data_set_size,3))

def convert(X,y):
    #create the labels based on logic
    for i,arr in enumerate(X):
        if in_range(arr[0],arr[1]):
            y[i][0]=1
        elif arr[0]>arr[1]:
            y[i][1]=1
        elif arr[1]>arr[0]:
            y[i][2]=1
    return y
def in_range(num1,num2,r=0.1):
    if num1-r<=num2 and num1+r>=num2:
        return True
    return False

y=convert(X,y)
unseen_y=convert(unseen_X,unseen_y)
X_data=torch.tensor(X,dtype=torch.float32)
y_data=torch.tensor(y,dtype=torch.float32)

output_nodes=3

network=Network(output_nodes)
network.add_layer(2,act=torch.sigmoid)
network.add_layer(6,act=torch.sigmoid)
network.add_layer(6,act=torch.sigmoid)
#train network
epochs=1000
lr=0.2

criterion = nn.MSELoss()

optimizer = torch.optim.Adam(network.parameters(), lr=lr)  # Let's try a different optimizer!
 
print(np.sum(network.network[1].matrix.detach().numpy()))

for epoch in range(epochs):
    acc=0
    l=0
    for dat,lab in zip(X_data,y_data):
        #calculate loss
        # Clear gradients 
        optimizer.zero_grad()
        # Predict outputs 
        #pass through network
        output=network.forward(dat)
        #output=torch.sum(output, axis=0) #get nodes of output
        y_pred=output
   
        #get best
        #print(torch.argmax(y_pred),torch.argmax(lab))
        if torch.equal(torch.argmax(y_pred),torch.argmax(lab)):
            acc+=1
        # Calculate loss # Calculate gradients
        loss=criterion(y_pred.requires_grad_(True),lab)
        loss.backward()
        # Update weights 
        # Backward and optimize
        optimizer.step()

        if str(loss.item())=="nan":
            print(output,y_pred,"error")
            network.show()
            print(network.network[0].matrix)
            print(network.network[1].matrix)
            exit()
        l+=abs(loss.item())
        
    #reform(network,a) #try copy over incase a by-reference doesn't work
    if epoch%100==0: #sjow accuracy
        print("Epoch",epoch,"accuracy:",acc/len(X_data) *100,"loss:",l)
        print(np.sum(network.network[1].matrix.detach().numpy()),)

print("End accuracy:",acc/len(X_data) *100, "%")

#unseen data
unseen_X=torch.tensor(unseen_X,dtype=torch.float32)
unseen_y=torch.tensor(unseen_y,dtype=torch.float32)
acc=0
for dat,lab in zip(unseen_X,unseen_y):
    output=network.forward(dat)
    if torch.equal(torch.argmax(output),torch.argmax(lab)):
        acc+=1
print("Testing accuracy:",acc/len(X_data) *100,"%")

network.save("parameters")