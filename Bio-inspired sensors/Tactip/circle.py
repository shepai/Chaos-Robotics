import numpy as np
import cv2 as cv
from skimage.measure import regionprops


# define a video capture object
cap = cv.VideoCapture(1)

def adaptive(img):
    frame=np.copy(img)
    threshold=200
    sum=0
    while sum<21000000:
        frame[frame>threshold]=255
        frame[frame<=threshold]=0
        sum=np.sum(frame)
        if sum>22000000:
            threshold+=1
        elif sum<22000000:
            threshold-=1
        frame=np.copy(img)
    frame[frame>threshold]=255
    frame[frame<=threshold]=0
    return frame

def removeBlob(im):
    # find all of the connected components (white blobs in your image).
    # im_with_separated_blobs is an image where each detected blob has a different pixel value ranging from 1 to nb_blobs - 1.
    nb_blobs, im_with_separated_blobs, stats, _ = cv.connectedComponentsWithStats(im)
    # stats (and the silenced output centroids) gives some information about the blobs. See the docs for more information. 
    # here, we're interested only in the size of the blobs, contained in the last column of stats.
    sizes = stats[:, -1]
    # the following lines result in taking out the background which is also considered a component, which I find for most applications to not be the expected output.
    # you may also keep the results as they are by commenting out the following lines. You'll have to update the ranges in the for loop below. 
    sizes = sizes[1:]
    nb_blobs -= 1

    # minimum size of particles we want to keep (number of pixels).
    # here, it's a fixed value, but you can set it as you want, eg the mean of the sizes or whatever.
    min_size = 250

    # output image with only the kept components
    im_result = np.zeros_like(im_with_separated_blobs)
    # for every component in the image, keep it only if it's above min_size
    for blob in range(nb_blobs):
        if sizes[blob] <= min_size:
            # see description of im_with_separated_blobs above
            im_result[im_with_separated_blobs == blob + 1] = 255#
    #difference = cv.subtract(im.astype(np.uint8), im_result.astype(np.uint8))
    # color the mask red
    #ret, mask = cv.threshold(difference, 0, 255,cv.THRESH_BINARY_INV |cv.THRESH_OTSU)
    #difference[mask != 255] = [255]

    return im_result

while(1):
    #vectors=[]
    ret, frame = cap.read()
    frame_gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    frame_gray=adaptive(frame_gray)
    to_Show=removeBlob(frame_gray)
    to_Show=to_Show.astype(np.uint8)
    """#img = cv.medianBlur(frame,5)
    to_Show=np.zeros_like(frame_gray)
    # calculate moments of binary image
    contours, hierarchies = cv.findContours(frame_gray, cv.RETR_LIST, cv.CHAIN_APPROX_SIMPLE)
    # put text and highlight the center
    for centroid in contours:
        centroid=np.squeeze(centroid)
        if len(centroid.shape)>1:
            for j in centroid:
                cv.circle(to_Show,(int(j[0]),int(j[1])),3, (255), thickness=1, lineType=8, shift=0)#"""
        #do stuff with that centroid ;)
    cv.imshow('detected circles',to_Show)
    cv.imshow('vision',frame)
    cv.imshow('grey',frame_gray)
    k = cv.waitKey(1) & 0xff
    if k == ord('q'):
        break

cv.destroyAllWindows()