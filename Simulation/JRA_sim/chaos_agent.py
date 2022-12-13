
from simulation import *
import matplotlib.pyplot as plt
import numpy as np
from CPML_PC import Network

##generate enviroment
points=[]
for i in range(500):
    randPoint=[random.randint(-50,50),random.randint(-50,50)]
    points.append(copy.deepcopy(randPoint))
    #plt.scatter(randPoint[0],randPoint[1],c="y",s=20)
if [0,0] in points:
    points.remove([0,0])

#get agent that can interface with the environment
a=agent()
vision=np.array(a.get_image(points))

#create network for sensory controller
network=Network(3) #forward, left, right
network.add_layer(vision.shape[0],act=torch.sigmoid)
network.add_layer(6,act=torch.sigmoid)

weights,idx=network.get_weights()
shape_of_weights=weights.shape
print(shape_of_weights)

class controller:
    def __init__(self,sensor,chaos):
        self.sensoryNN=sensor
        self.chaoticNN=chaos


show_points(points,a)
print(a.get_image(points))
plt.show()