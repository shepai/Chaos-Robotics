
from simulation import *
import matplotlib.pyplot as plt
import numpy as np
from CPML_PC import Network, Brain
from copy import deepcopy
from timer_pred import predictor
import time
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
network=Network(2) #fdiffrent direction
network.add_layer(vision.shape[0])
network.add_layer(10)
network.add_layer(6)

out=Network(8) #fdiffrent direction
out.add_layer(2) #chaotic oscillation
out.add_layer(6)
out.add_layer(6)

Sennetwork=Network(8) #fdiffrent direction
Sennetwork.add_layer(vision.shape[0])
Sennetwork.add_layer(10)
Sennetwork.add_layer(6)
Sennetwork.add_layer(6)
Sennetwork.add_layer(6)

class controller:
    def __init__(self,sensor,chaos,output,agent,chaotic=True):
        self.sensoryNN=sensor
        self.chaoticNN=chaos
        self.outputNN=output
        self.agent=agent
        weights,idx=self.sensoryNN.get_weights()
        weights2,idx=self.outputNN.get_weights()
        if chaotic:
            self.shape_of_weights=(np.concatenate((weights,chaos.getWeights(),weights2))).shape
        else:
            self.shape_of_weights=((weights)).shape
        self.path=[[0,0]]
        self.vectors=[]
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
            if np.sum(self.chaoticNN.out)==0:
                #forward
                ind=2
            else:
                #turn
                ind=0
            #print(p_value,self.chaoticNN.out,inp)
            #outcome=self.outputNN.forward(inp)
            #ind=np.argmax(outcome)

        self.vectors.append(vectors[ind]) #store path
        self.agent.set_vector(vectors[ind][0],vectors[ind][1])
        self.path.append([self.agent.y,self.agent.x]) 
        return getBump(self.agent,self.points)
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
    def resetAgent(self):
        self.vectors=[]
        self.points=[]
        for i in range(600):
            randPoint=[random.randint(-50,50),random.randint(-50,50)]
            self.points.append(copy.deepcopy(randPoint))
        if [0,0] in self.points:
            self.points.remove([0,0])
        self.path=[[0,0]]
        self.agent=agent()



#setup trial function
def trial(control,maxtime=5):
    dt=0.1
    t=0
    input=control.agent.get_image(control.points)
    lastPoint=[control.agent.x,control.agent.y]
    contin=False
    while not contin and t<maxtime: #loop till hit or timer runs out
        input=control.agent.get_image(control.points)
        contin=control.forward(input)
        t+=dt #time step
    currentPoint=[control.agent.x,control.agent.y] #get current position
    distance=math.sqrt((lastPoint[0]-currentPoint[0])**2 + (lastPoint[1]-currentPoint[1])**2)
    #count how many turns
    count=0
    for v in control.vectors:
        n=len(control.vectors)-control.vectors.count(v)
        if n>count: count=n
    return min(t/(maxtime+dt) *0.5 + distance/(maxtime/dt) * 0.5,1) * (count/len(control.vectors)) #returns the fitness made up of time survived and distance travelled #

def run(chaos_):
    #create chaotic neuron pair
    chaos=Brain() 
    c=None
    if chaos_:
        c=controller(network,chaos,out,a,chaotic=chaos_)
    else: 
        c=controller(Sennetwork,chaos,out,a,chaotic=chaos_)
    #population of genotypes
    genotype = np.zeros((c.shape_of_weights))
    SIZE=100
    population=np.random.normal(0,20,(SIZE,genotype.shape[0]))+np.random.normal(0,2,(SIZE,genotype.shape[0]))
    #genetic algorithm
    Gen=200
    fitness=[0]
    bestPath=[]
    points_path=None
    for i in range(Gen):
        #print("Generation:",i,"with fitness",fitness[-1])
        t1=random.randint(0,SIZE-1)
        t2 = t1+1 if (t1<SIZE-1) else t1-1 #local population tournaments
        #trial 1
        f1=0
        for i in range(3):
            c.resetAgent()
            c.set_gene(population[t1])
            f=trial(c)
            f1+=f
            if f>fitness[-1]:
                bestPath=deepcopy(c.path)
                points_path=deepcopy(points)
        f1/=3
        #trial 2
        f2=0
        for i in range(3):
            c.resetAgent()
            c.set_gene(population[t2])
            f=trial(c)
            f2+=f
            if f>fitness[-1]:
                bestPath=deepcopy(c.path)
                points_path=deepcopy(points)
        f2/=3
        #selection
        if f1>f2:
            population[t2]=deepcopy(c.mutate_gene(population[t2]))
        elif f2>f1:
            population[t1]=deepcopy(c.mutate_gene(population[t1]))
        else: #mutate both
            population[t2]=deepcopy(c.mutate_gene(population[t2]))
            population[t1]=deepcopy(c.mutate_gene(population[t1]))
        fitness.append(max(fitness+[f1,f2]))
    return fitness.index(fitness[-1]),fitness[-1] #get first occurence of best fitness
    """
    path=np.array(bestPath)
    c.resetAgent()
    show_points(points_path,c.agent)
    plt.plot(path[:,1],path[:,0])
    plt.show()"""
trials=50
f=np.zeros((2,trials))
f1=np.zeros((2,trials))
print("RUN...")
p_=predictor(time.time())
for i in range(trials):
    ind,fit=run(True)
    f[0][i]=ind
    f[1][i]=fit*100
    p_.calc_iteration(time.time())
    print("Time left:",p_.time_left((trials*2) -i)/60,"minutes")
    print(">>",f[1][i],f[0][i])

for i in range(trials):
    ind,fit=run(False)
    f1[0][i]=ind
    f1[1][i]=fit*100
    p_.calc_iteration(time.time())
    print("Time left:",p_.time_left((trials) - i)/60,"minutes")
    print(">>",f1[1][i],f1[0][i])

fig,ax = plt.subplots(1,2,figsize=(9, 4))
print(f)
print(f1)
bplot1 = ax[0].boxplot(list([f[0],f1[0]]),
                     vert=True,  # vertical box alignment
                     patch_artist=True,labels=["Chaotic neuron","FNN"])  # will be used to label x-ticks
bplot2 = ax[1].boxplot(list([f[1],f1[1]]),
                     vert=True,  # vertical box alignment
                     patch_artist=True,labels=["Chaotic neuron","FNN"])  # will be used to label x-ticks


ax[0].set_title('Shortest generation of convergence')

ax[0].yaxis.grid(True)
#ax[0].set_xlabel('Simulation models')
ax[0].set_ylabel('Generation')

ax[1].set_title('Simulation results as fitnesses over trials')

ax[1].yaxis.grid(True)
#ax[1].set_xlabel('Simulation models')
ax[1].set_ylabel('Fitness of trials')

fig.show()
plt.show()
"""for i in range(10):
    c.forward(vision)
    c.mutateNN()"""