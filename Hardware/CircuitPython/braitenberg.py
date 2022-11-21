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
    return 1-np.array([a,b]) 
#setup brain
b=Brain(2,2,size=200)

for k in range(100):
    for i in range(b.steps-2): #main loop to run
        inputs=getSensor() #grab sensor information
        b.run(inputs)
        #failsafe
        if maths.isnan(b.out[0]):
            b.out[0]=0.0
            b.reset()
        if maths.isnan(b.out[1]):
            b.out[1]=0.0
            b.reset()
        print(inputs,b.p,b.out)
        #move motors based off output pattern
        try:
            bot.motorOn(3, "f" if b.out[0]<0 else "r", 100*abs(b.out[0]))
            bot.motorOn(4, "f" if b.out[1]<0 else "r", 100*abs(b.out[1]))
            #pass
        except:
            bot.stop()
    bot.stop()
    b.clearOld()
    
bot.stop()
