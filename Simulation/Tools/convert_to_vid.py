import os
import cv2 as cv

folder="C:/Users/dexte/OneDrive/Pictures/Saved Pictures/PhD chaos/AutoGen/"

items=os.listdir(folder)

p=cv.imread(folder+items[0])

h, w = p.shape[:2]
print(w,h)

out = cv.VideoWriter("C:/Users/dexte/github/Chaos-Robotics/Assets/"+'output_video_pend.avi',cv.VideoWriter_fourcc(*'DIVX'), 60, (w,h))


for i in items:
    ad=cv.imread(folder+i)
    out.write(ad)
out.release()
