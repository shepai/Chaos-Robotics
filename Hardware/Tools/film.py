import cv2
import time

path="C:/Users/dexte/OneDrive/Pictures/Saved Pictures/PhD chaos/AutoGen/"
vid = cv2.VideoCapture(0)

#variables must match the circuitpython code
time_delay=0.5
iterations=100
startTime=time.time()
print("Recording")
for i in range(iterations):
    ret, frame = vid.read()
    time.sleep(time_delay)
    cv2.imwrite(path+str(round(time.time()-startTime))+".png", frame)

