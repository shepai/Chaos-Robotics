from Brain import Brain
from EduBot_CP import *
import ulab.numpy as np
import random
import math as maths
import board
from analogio import AnalogIn
import time
#setup robot
bot=wheelBot(board_type="pico")
bot.stop()

#get sensor set up
sensorA=AnalogIn(board.GP26)
sensorB=AnalogIn(board.GP27)

def getSensor(): #get an input array of sensor readngs
    a=sensorA.value / 65535 * sensorA.reference_voltage /4.8
    b=sensorB.value / 65535 * sensorB.reference_voltage /4.8
    return np.array([a,b]) *100
#setup brain
b=Brain(2,2,size=100)
d=[]
ps=[]

for i in range(b.steps-2): #main loop to run
    inputs=getSensor() #grab sensor information
    b.run(inputs)
    print(b.out)
    d.append(b.out)
    ps.append(b.p)
    #move motors based off output pattern
    bot.motorOn(3, "r" if b.out[0]<0 else "f", 100*abs(b.out[0]))
    bot.motorOn(4, "r" if b.out[1]<0 else "f", 100*abs(b.out[1]))
d=np.array(d)

bot.stop()
