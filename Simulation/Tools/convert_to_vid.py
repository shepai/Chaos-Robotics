import os
import cv2 as cv
import re 

folder="C:/Users/dexte/OneDrive/Pictures/Saved Pictures/PhD chaos/AutoGen/"

items=os.listdir(folder)

p=cv.imread(folder+items[0])

h, w = p.shape[:2]
print(w,h)

out = cv.VideoWriter("C:/Users/dexte/github/Chaos-Robotics/Assets/"+'dynamicalSignal.avi',cv.VideoWriter_fourcc(*'DIVX'), 30, (w,h))


#sort
items.sort(key=lambda f: int(re.sub('\D', '', f)))
print(items)

#gen vid
for i,j in enumerate(items):
    ad=cv.imread(folder+str(j))
    out.write(ad)
out.release()
