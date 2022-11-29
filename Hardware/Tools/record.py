import matplotlib.pyplot as plt
import os
import cv2 as cv
import re
import numpy as np

folder="C:/Users/dexte/OneDrive/Pictures/Saved Pictures/PhD chaos/AutoGen/"
path="C:/Users/dexte/github/Chaos-Robotics/models/Data/"

items=os.listdir(folder)

p=cv.imread(folder+items[0])


p_vals=[]
chaos=[]
sensorA=[]
sensorB=[]
motorA=[]
motorB=[]
timer=[]
#gather data in the file
file=open(path+"LogBB.csv","r")
r=file.read()
file.close()
d={}
for i,line in enumerate(r.split("\n")[:-1]):
    data=line.split(",")
    p_vals.append(int(data[-2]))
    motorA.append(int(data[2]))
    motorB.append(int(data[3]))
    chaos.append([float(data[4]),float(data[5])])
    sensorA.append(float(data[0]))
    sensorB.append(float(data[1]))
    timer.append(int(data[-1]))
    data=d.get(data[-1],[])
    data.append(i)
chaos=np.array(chaos)
sensorA=np.array(sensorA)
sensorB=np.array(sensorB)
sensorA=np.round_(sensorA, decimals = 3)
sensorB=np.round_(sensorB, decimals = 3)
motorA=np.array(motorA)
motorB=np.array(motorB)
time=[i for i in range(len(sensorA))]

def getData(i,ad):
    fig, axes = plt.subplots(3,2)
    #axes[0].figure(figsize=(5,1))
    axes[0,0].set_title("A: Chaotic signal", loc="left")
    axes[0,0].plot(time[:i+1],chaos[:,0][:i+1],label="Neuron 1")
    axes[0,0].plot(time[:i+1],chaos[:,1][:i+1],label="Neuron 2")
    axes[0,0].set_xlabel("Time (t)")
    axes[0,0].set_ylabel("Output")
    axes[0,0].legend(loc="upper right")
    #axes[0].set_yticks(axes[0].get_yticks()[::33]) #axes[0].get_xticks()[::100]
    #axes[1].figure(figsize=(5,1))
    axes[1,0].set_title("B: Sensors", loc="left")
    axes[1,0].plot(time[:i+1],sensorA[:i+1],label="Sensor A")
    axes[1,0].plot(time[:i+1],sensorB[:i+1],label="Sensor B")
    axes[1,0].set_xlabel("Time (t)")
    axes[1,0].set_ylabel("Light intensity %")
    axes[1,0].legend(loc="upper right")
    #axes[1].set_ylim([0,1],auto=True)
    #axes[1].set_yticks(axes[1].get_yticks()[::100]) #axes[0].get_xticks()[::100]

    axes[0,1].set_title("C: P value", loc="left")
    axes[0,1].plot(time[:i+1],p_vals[:i+1])
    axes[0,1].set_xlabel("Time (t)")
    axes[0,1].set_ylabel("Value")

    axes[1,1].set_title("D: Motors", loc="left")
    axes[1,1].plot(time[:i+1],motorA[:i+1])
    axes[1,1].plot(time[:i+1],motorB[:i+1])
    axes[1,1].set_xlabel("Time (t)")
    axes[1,1].set_ylabel("Speed")
    #axes[1,1].set_ylim([-1,1],auto=True)

    #axes[4].figure(figsize=(5,5))
    axes[2,1].set_title("E: Real world", loc="left")
    axes[2,1].imshow(ad)
    axes[2,1].xaxis.set_visible(False)
    axes[2,1].yaxis.set_visible(False)

    axes[2,0].xaxis.set_visible(False)
    axes[2,0].yaxis.set_visible(False)
    fig.tight_layout(pad=0.05)
    #save
    fig.savefig("save"+".png")
    ad=cv.imread("save"+".png")
    return ad
#sort
items.sort(key=lambda f: int(re.sub('\D', '', f)))

ad=cv.imread(folder+str(items[0]))
p=getData(1,ad)
h, w = p.shape[:2]
print(w,h)

out = cv.VideoWriter("C:/Users/dexte/github/Chaos-Robotics/Assets/"+'ftl_trial.avi',cv.VideoWriter_fourcc(*'DIVX'), 15, (w,h))
#gen vid
for i,j in enumerate(items[:-1]):
    timestep=round(float(j.replace(".png","")))
    bestind=d.get(timestep,i)
    print(j,"out of",items[-1])
    ad=cv.imread(folder+str(j))
    #plot data
    ad=getData(bestind,ad)
    out.write(ad)
    plt.close()

out.release()
