
# import the opencv library
import cv2
  
  
# define a video capture object
vid = cv2.VideoCapture(0)
  
ret, frame = vid.read()
h, w = frame.shape[:2]
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('./movement.avi', fourcc, 20.0, (w,h))

while(True):
      
    # Capture the video frame
    # by frame
    ret, frame = vid.read()
  
    # Display the resulting frame
    cv2.imshow('frame', frame)
    out.write(frame)
    # the 'q' button is set as the
    # quitting button you may use any
    # desired button of your choice
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
  
# After the loop release the cap object
vid.release()
out.release()
# Destroy all the windows
cv2.destroyAllWindows()
