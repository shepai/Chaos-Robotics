import os
import cv2 as cv

folder="C:/Users/dexte/OneDrive/Pictures/Saved Pictures/PhD chaos/AutoGen/"

items=os.listdir(folder)

p=cv.imread(folder+items[0])

h, w = p.shape[:2]
print(w,h)

out = cv.VideoWriter("C:/Users/dexte/github/Chaos-Robotics/Assets/"+'bvp_new.avi',cv.VideoWriter_fourcc(*'DIVX'), 10, (w,h))


for i,j in enumerate(items):
    ad=cv.imread(folder+"save"+str(i+1)+".png")
    out.write(ad)
out.release()
