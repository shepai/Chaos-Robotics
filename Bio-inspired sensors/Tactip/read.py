
# import the opencv library
import cv2
import numpy as np
from scipy import ndimage
import matplotlib.pyplot as plt

def getDots(im):
    labels, nlabels = ndimage.label(gray)
    t = ndimage.center_of_mass(gray, labels, np.arange(nlabels) + 1 )
    t=np.array(t)
    t=np.rint(t).astype(int)
    return t
# define a video capture object
vid = cv2.VideoCapture(1)
ret, frame = vid.read()
gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
#binary
gray[gray>245]=255
gray[gray<=245]=0
old=gray.copy()
old_T=getDots(gray)

new=np.zeros_like(frame)

while(True):
      
    # Capture the video frame
    # by frame
    ret, frame = vid.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    #binary
    gray[gray>245]=255
    gray[gray<=245]=0
    #glare
    m_img = cv2.medianBlur(frame,5)
    ret,th1 = cv2.threshold(m_img,180,255,cv2.THRESH_BINARY)
    #new=np.abs(gray-old)
    t=getDots(gray)
    
    new[t[:,0],t[:,1]]=(255,0,0)
    new[old_T[:,0],old_T[:,1]]=(0,0,255)
    old_T=t.copy()
    
    old=gray.copy()
    # Display the resulting frame
    cv2.imshow('binary', new)
    
    cv2.imshow('normal', m_img)
    # the 'q' button is set as the
    # quitting button you may use any
    # desired button of your choice
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
  
# After the loop release the cap object
vid.release()
# Destroy all the windows
cv2.destroyAllWindows()