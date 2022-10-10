from servoController import *
import matplotlib.pyplot as plt
import random
import time


def graph(formula, x_range):  
    x = np.array(x_range)  
    y = eval(formula)
    plt.plot(x, y)

def get_distance(p1,p2): #measure distance between points
    y1,y2=p1[1],p2[1]
    x1,x2=p1[0],p2[0]
    dist = math.sqrt( (x2 - x1)**2 + (y2 - y1)**2 )
    return dist
    
l=leg()
plt.ylim((-50,50))
plt.xlim((-50,50))
l.update()

MAX_DIST = 21
for i in range(800): #perform
    l.reset() #set neutral position
    plt.cla()
    l.update()
    #grad=random.randint(-50,30)/100 #generate random
    tip=[l.servos[0].point.x-2,l.servos[0].point.y] #get point of leg tip
    hip=[l.servos[2].point.x,l.servos[2].point.y]
    grad=0
    c=random.randint(0,6) 
    #if grad<0: c=3
    graph(str(grad)+'*x+'+str(c), range(-10, 40)) #draw obstacle
    plt.pause(0.05)
    y=(grad*tip[0])+c
    y1=(grad*hip[0])+c
    
    if get_distance([tip[0],y],hip)>MAX_DIST:
        #too far to reach
        l.MoveTo(hip[0],y1)
        print(hip[0],y)
    else:
        l.MoveTo(tip[0],y)
        print(tip[0],y)
    
    
    print(l.servos[0].point.x,l.servos[0].point.y)
    plt.cla()
    graph(str(grad)+'*x+'+str(c), range(-10, 40)) #draw obstacle
    plt.scatter(hip[0],y1,c="y")
    plt.scatter(tip[0],y,c="y")
    l.update()
    plt.pause(0.05)
    time.sleep(0.05)
    print("")
    
