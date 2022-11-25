import numpy as np
import matplotlib.pyplot as plt
path="C:/Users/dexte/github/Chaos-Robotics/models/Data/"

p_vals=[]
chaos=[]
sensorA=[]
sensorB=[]

#gather data in the file
file=open(path+"loggedB.csv","r")
r=file.read()
file.close()

for line in r.split("\n")[:-1]:
    data=line.split(",")
    p_vals.append(int(data[-1]))
    chaos.append([float(data[2]),float(data[3])])
    sensorA.append(float(data[0]))
    sensorB.append(float(data[1]))

chaos=np.array(chaos)
sensorA=np.array(sensorA)
sensorB=np.array(sensorB)
sensorA=np.round_(sensorA, decimals = 3)
sensorB=np.round_(sensorB, decimals = 3)

#plot data
time=[i for i in range(len(sensorA))]
fig, axes = plt.subplots(3,1)
#axes[0].figure(figsize=(5,1))
axes[0].set_title("A: Chaotic signal", loc="left")
axes[0].plot(time,chaos[:,0],label="Neuron 1")
axes[0].plot(time,chaos[:,1],label="Neuron 2")
axes[0].set_xlabel("Time (t)")
axes[0].set_ylabel("Output")
axes[0].legend(loc="upper right")
#axes[0].set_yticks(axes[0].get_yticks()[::33]) #axes[0].get_xticks()[::100]
#axes[1].figure(figsize=(5,1))
axes[1].set_title("B: Sensors", loc="left")
axes[1].plot(time,sensorA,label="Sensor A")
axes[1].plot(time,sensorB,label="Sensor B")
axes[1].set_xlabel("Time (t)")
axes[1].set_ylabel("Light intensity %")
axes[1].legend(loc="upper right")
#axes[1].set_ylim([0,1],auto=True)
#axes[1].set_yticks(axes[1].get_yticks()[::100]) #axes[0].get_xticks()[::100]

axes[2].set_title("p: P value", loc="left")
axes[2].plot(time,p_vals)
axes[2].set_xlabel("Time (t)")
axes[2].set_ylabel("Value")

fig.tight_layout(pad=0.05)
fig.show()
plt.show()