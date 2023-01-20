import cv2
import matplotlib.pyplot as plt
import numpy as np
cap = cv2.VideoCapture('C:/Users/dexte/github/Chaos-Robotics/Bio-inspired sensors/Tactip/Vid/Movement.mp4')

# Check if camera opened successfully
if (cap.isOpened()== False): 
  print("Error opening video stream or file")
out = cv2.VideoWriter("C:/Users/dexte/github/Chaos-Robotics/Assets/"+'TactipOpticlong.avi',cv2.VideoWriter_fourcc(*'DIVX'), 30, (768,864))
last=None
# Read until video is completed
hsv = np.zeros((1080,1920,3),dtype=np.float32)
hsv[..., 1] = 255
try:
    while(cap.isOpened()):
        # Capture frame-by-frame
        ret, frame = cap.read()
        next = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        #make binary image
        next[next>100]=255
        next[next<100]=0
        #place initialization of hsv here to restart events based on frames
        
        if type(last)!=type(None):
            flow = cv2.calcOpticalFlowFarneback(last, next, None, 0.5, 3, 15, 3, 5, 1.2, 0)
            mag, ang = cv2.cartToPolar(flow[..., 0], flow[..., 1])
            hsv[..., 0] = ang*180/np.pi/2
            hsv[..., 2] = cv2.normalize(mag, None, 0, 255, cv2.NORM_MINMAX)
            
            bgr = cv2.cvtColor(hsv, cv2.COLOR_HSV2BGR)
            # resize image
            scale_percent = 40 # percent of original size
            width = int(bgr.shape[1] * scale_percent / 100)
            height = int(bgr.shape[0] * scale_percent / 100)
            dim = (width, height)
            resized = cv2.resize(bgr, dim, interpolation = cv2.INTER_AREA)
            resized_orig = cv2.resize(frame, dim, interpolation = cv2.INTER_AREA)
            vis = np.concatenate((resized, resized_orig), axis=0)
            cv2.imshow("frame",vis)
            out.write(vis)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                cap.release()
                break
        last=next
except cv2.error as e:
    print(e)
    pass
except KeyboardInterrupt:
    pass

out.release()
