import numpy as np
import matplotlib.pyplot as plt
path="C:/Users/dexte/github/Chaos-Robotics/models/Data/"

p_vals=[]
chaos=[]
sensorA=[]
sensorB=[]
motorA=[]
motorB=[]
#gather data in the file
file=open(path+"LogBB.csv","r")
r=file.read()
file.close()

for line in r.split("\n")[:-1]:
    data=line.split(",")
    p_vals.append(int(data[-1]))
    motorA.append(int(data[2]))
    motorB.append(int(data[3]))
    chaos.append([float(data[4]),float(data[5])])
    sensorA.append(float(data[0]))
    sensorB.append(float(data[1]))

chaos=np.array(chaos)
sensorA=np.array(sensorA)
sensorB=np.array(sensorB)
sensorA=np.round_(sensorA, decimals = 3)
sensorB=np.round_(sensorB, decimals = 3)

#plot data
time=[i for i in range(len(sensorA))]
fig, axes = plt.subplots(4,1)
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

axes[2].set_title("C: P value", loc="left")
axes[2].plot(time,p_vals)
axes[2].set_xlabel("Time (t)")
axes[2].set_ylabel("Value")

axes[3].set_title("D: Motors", loc="left")
axes[3].plot(time,motorA)
axes[3].plot(time,motorB)
axes[3].set_xlabel("Time (t)")
axes[3].set_ylabel("Speed")
axes[1].set_ylim([-1,1],auto=True)

fig.tight_layout(pad=0.05)
fig.show()
plt.show()