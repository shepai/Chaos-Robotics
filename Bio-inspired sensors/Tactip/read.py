
# import the opencv library
import cv2
  
  
# define a video capture object
vid = cv2.VideoCapture(0)
  
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

    # Display the resulting frame
    cv2.imshow('binary', gray)
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