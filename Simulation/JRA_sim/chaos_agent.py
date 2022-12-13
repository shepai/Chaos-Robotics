
from simulation import *
import matplotlib.pyplot as plt
import numpy as np
from CPML_PC import Network, Brain
from copy import deepcopy
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
network=Network(8) #fdiffrent direction
network.add_layer(vision.shape[0])
network.add_layer(6)

class controller:
    def __init__(self,sensor,chaos,agent):
        self.sensoryNN=sensor
        self.chaoticNN=chaos
        self.agent=agent
        weights,idx=self.sensoryNN.get_weights()
        self.shape_of_weights=(np.concatenate((weights,chaos.getWeights()))).shape
    def forward(self,input):
        outcome=self.sensoryNN.forward(input)
        ind=np.argmax(outcome)
        vectors=[[1,1],[1,0],[0,1],[-1,0],[0,-1],[1,-1],[-1,1],[-1,-1]]
        self.agent.set_vector(vectors[ind][0],vectors[ind][1])
        #print(self.agent.x,self.agent.y)
        return getBump(self.agent,points)
  
    def mutate_gene(self,gene):
        noise=np.random.normal(0,5,self.shape_of_weights) #add gaussian noise to the weights
        gene+=noise
        return gene
    def set_gene(self,gene):
        assert gene.shape==self.shape_of_weights, "Incorrect gene size" +str(gene.shape)+","+str(self.shape_of_weights) 
        weights,idx=self.sensoryNN.get_weights()
        #set the differnet network data
        weight=gene[0:idx[-2]]
        chaos=gene[idx[-2]:]
        self.sensoryNN.reform_weights(weight,idx)
        self.chaoticNN.formWeights(chaos)
    def resetAgent(self):
        self.agent=agent()

#create chaotic neuron pair
chaos=Brain(vision.shape,8) 
c=controller(network,chaos,a)


#population of genotypes
genotype = np.zeros((c.shape_of_weights))
SIZE=100
population=np.random.normal(0,10,(SIZE,genotype.shape[0]))+np.random.normal(0,5,(SIZE,genotype.shape[0]))

#setup trial function
def trial(control,maxtime=50):
    dt=0.1
    t=0
    input=control.agent.get_image(points)
    lastPoint=[control.agent.x,control.agent.y]
    contin=True
    while contin and t<maxtime: #loop till hit or timer runs out
        input=control.agent.get_image(points)
        contin=control.forward(input)
        t+=dt #time step
    currentPoint=[control.agent.x,control.agent.y] #get current position
    distance=math.sqrt((lastPoint[0]-currentPoint[0])**2 + (lastPoint[1]-currentPoint[1])**2)
    return min(t/(maxtime+dt) *0.5 + distance/(maxtime/dt) * 0.5,1) #returns the fitness made up of time survived and distance travelled 

#genetic algorithm
Gen=100
fitness=0
for i in range(Gen):
    t1=random.randint(0,SIZE-1)
    t2 = t1+1 if (t1<SIZE-1) else t1-1 #local population tournaments
    #trial 1
    c.resetAgent()
    c.set_gene(population[t1])
    f1=trial(c)
    #trial 2
    c.resetAgent()
    c.set_gene(population[t2])
    f2=trial(c)
    #selection
    if f1>f2:
        population[t2]=deepcopy(c.mutate_gene(population[t2]))
    elif f2>f1:
        population[t1]=deepcopy(c.mutate_gene(population[t1]))
    if f1>fitness:
        pass
    elif f2>fitness:
        pass
    fitness=max([fitness,f1,f2])
print(fitness)
    
#show_points(points,a)
#print(a.get_image(points))
#plt.show()


"""for i in range(10):
    c.forward(vision)
    c.mutateNN()"""