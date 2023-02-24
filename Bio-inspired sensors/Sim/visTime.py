import matplotlib.pyplot as plt
import matplotlib.colors as mc
import numpy as np
import colorsys
import cv2

def lighten_color(color, amount=0.5):
    """
    Lightens the given color by multiplying (1-luminosity) by the given amount.
    Input can be matplotlib color string, hex string, or RGB tuple.

    Examples:
    >> lighten_color('g', 0.3)
    >> lighten_color('#F034A3', 0.6)
    >> lighten_color((.3,.55,.1), 0.5)
    """
    try:
        c = mc.cnames[color]
    except:
        c = color
    c = colorsys.rgb_to_hls(*mc.to_rgb(c))
    return colorsys.hls_to_rgb(c[0], 1 - amount * (1 - c[1]), c[2])

LIM=-1
folder="./"
ar=np.load(folder+"images.npy")
#ar=np.random.normal(0,5,(20,10,10))
images=ar[:LIM]
v=np.load(folder+"arrows.npy")
#v=np.zeros((20,2))
vecs=v[:LIM]

fig, axes = plt.subplots(1,2, figsize=(3.5, 2.0))
axes[0].set_title("A: Tactip", loc="left")
axes[1].set_title("B: Vector", loc="left")
DIM=50
h=DIM#images[0].shape[0]
w=DIM#images[0].shape[1]

im=np.zeros((h*((len(images))//8),w*(len(images)//6)))
print(im.shape,h,w)
print(((len(images)//2) -1) * ((len(images)//2) -1))
c=0
i=0
j=0
while i <((len(images)//4)) and c<len(images):
    while j <((len(images)//6)) and c<len(images):
        #print(c,i*h,":",((1+i)*h),j*w,":",(j+1)*w)
        percentage=(c+1)/len(images)
        #shaded=images[i]*percentage #get shade increasing over time
        #im=(im+shaded) #add the new shade
        _im=cv2.resize(images[c].reshape((128,128))[44:84,44:83], dsize=(h,w), interpolation=cv2.INTER_AREA) 
        im[i*h:((1+i)*h),j*w:(j+1)*w]=np.copy(_im)
        a=vecs[c]
        axes[1].arrow(0, 0, a[0], a[1], head_width=0.2, head_length=0.1, length_includes_head=True, color=lighten_color('b', percentage))
        #axes[1].plot([0],[0],c=lighten_color('r', c/100))
        c+=2
        j+=1
    j=0
    i+=1
    
#plt.imshow(im)
#plt.show()
#axes[0].figure(figsize=(5,1))

axes[0].imshow(im)
axes[0].xaxis.set_visible(False)
axes[0].yaxis.set_visible(False)
#axes[1].figure(figsize=(5,1))


axes[1].set_xlim((-1.1, 1.1))
axes[1].set_ylim((-1.1, 1.1))
axes[1].set_aspect("equal")
fig.tight_layout()
fig.savefig("./"+"save"+".pdf")
#fig.show()
#"""
