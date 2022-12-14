
from simulation import *
import matplotlib.pyplot as plt
import numpy as np
from CPML_PC import Network, Brain
from copy import deepcopy
##generate enviroment
points=[]
for i in range(600):
    randPoint=[random.randint(-50,50),random.randint(-50,50)]
    points.append(copy.deepcopy(randPoint))
    #plt.scatter(randPoint[0],randPoint[1],c="y",s=20)
if [0,0] in points:
    points.remove([0,0])

#get agent that can interface with the environment
a=agent()
vision=np.array(a.get_image(points))

#create network for sensory controller
network=Network(5) #fdiffrent direction
network.add_layer(vision.shape[0])
network.add_layer(6)

out=Network(8) #fdiffrent direction
out.add_layer(2) #chaotic oscillation
out.add_layer(6)


class controller:
    def __init__(self,sensor,chaos,output,agent):
        self.sensoryNN=sensor
        self.chaoticNN=chaos
        self.outputNN=output
        self.agent=agent
        weights,idx=self.sensoryNN.get_weights()
        weights2,idx=self.outputNN.get_weights()
        self.shape_of_weights=(np.concatenate((weights,chaos.getWeights(),weights2))).shape
        self.path=[[0,0]]
    def forward(self,input):
        #run through sensory processing
        outcome=self.sensoryNN.forward(input)
        ind=np.argmax(outcome)
        p_value=self.chaoticNN.values[ind] #select p value to control chaos
        #get chaotic generation
        self.chaoticNN.reset()
        for i in range(self.chaoticNN.steps-5):
            self.chaoticNN.runP(p_value)
        #run through post porcessing
        outcome=self.outputNN.forward(self.chaoticNN.out)
        ind=np.argmax(outcome)
        vectors=[[1,1],[1,0],[0,1],[-1,0],[0,-1],[1,-1],[-1,1],[-1,-1]]
        self.agent.set_vector(vectors[ind][0],vectors[ind][1])
        self.path.append([self.agent.y,self.agent.x])
        #print(self.agent.x,self.agent.y)
        return getBump(self.agent,points)
    def mutate_gene(self,gene):
        noise=np.random.normal(0,5,self.shape_of_weights) #add gaussian noise to the weights
        gene+=noise
        return gene
    def set_gene(self,gene):
        assert gene.shape==self.shape_of_weights, "Incorrect gene size" +str(gene.shape)+","+str(self.shape_of_weights) 
        weights,idx=self.sensoryNN.get_weights()
        weights2,idx2=self.outputNN.get_weights()
        #set the differnet network data
        weight=gene[0:idx[-2]]
        chaos=gene[idx[-2]:idx[-2]+6]
        weight2=gene[idx[-2]+6:]
        self.sensoryNN.reform_weights(weight,idx)
        self.chaoticNN.formWeights(chaos)
        self.outputNN.reform_weights(weight2,idx2)
    def resetAgent(self):
        global points
        points=[]
        for i in range(600):
            randPoint=[random.randint(-50,50),random.randint(-50,50)]
            points.append(copy.deepcopy(randPoint))
        if [0,0] in points:
            points.remove([0,0])
        self.path=[[0,0]]
        self.agent=agent()

#create chaotic neuron pair
chaos=Brain() 
c=controller(network,chaos,out,a)

#population of genotypes
genotype = np.zeros((c.shape_of_weights))
SIZE=100
population=np.random.normal(0,10,(SIZE,genotype.shape[0]))+np.random.normal(0,5,(SIZE,genotype.shape[0]))

#setup trial function
def trial(control,maxtime=5):
    dt=0.1
    t=0
    input=control.agent.get_image(points)
    lastPoint=[control.agent.x,control.agent.y]
    contin=False
    while not contin and t<maxtime: #loop till hit or timer runs out
        input=control.agent.get_image(points)
        contin=control.forward(input)
        t+=dt #time step
    currentPoint=[control.agent.x,control.agent.y] #get current position
    distance=math.sqrt((lastPoint[0]-currentPoint[0])**2 + (lastPoint[1]-currentPoint[1])**2)
    return min(t/(maxtime+dt) ,1) #returns the fitness made up of time survived and distance travelled #*0.5 + distance/(maxtime/dt) * 0.5

#genetic algorithm
Gen=500
fitness=0
bestPath=[]
points_path=None
for i in range(Gen):
    print("Generation:",i,"with fitness",fitness)
    t1=random.randint(0,SIZE-1)
    t2 = t1+1 if (t1<SIZE-1) else t1-1 #local population tournaments
    #trial 1
    f1=0
    for i in range(3):
        c.resetAgent()
        c.set_gene(population[t1])
        f1+=trial(c)
    f1/=3
    if f1>fitness:
        bestPath=c.path.copy()
    #trial 2
    f2=0
    for i in range(3):
        c.resetAgent()
        c.set_gene(population[t2])
        f2+=trial(c)
    f2/=3
    if f2>fitness:
        bestPath=c.path.copy()
        points_path=points.copy()
    #selection
    if f1>f2:
        population[t2]=deepcopy(c.mutate_gene(population[t2]))
    elif f2>f1:
        population[t1]=deepcopy(c.mutate_gene(population[t1]))
    fitness=max([fitness,f1,f2])
print(fitness)

path=np.array(bestPath)
show_points(points_path,a)
plt.plot(path[:,1],path[:,0])
plt.show()


"""for i in range(10):
    c.forward(vision)
    c.mutateNN()"""