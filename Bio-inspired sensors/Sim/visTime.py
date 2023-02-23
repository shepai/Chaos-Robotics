import matplotlib.pyplot as plt
import matplotlib.colors as mc
import numpy as np
import colorsys

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

folder=""
#ar=np.open(folder+"meta.npy")
ar=np.random.normal(0,5,(20,10,10))
images=ar
#v=np.open(folder+"meta.npy")
v=np.zeros((20,2))
v[:,0]=1
v[:,1]=1
v[0]=[-1,0]
v[-1]=[-1,-1]
vecs=v

fig, axes = plt.subplots(1,2, figsize=(3.5, 2.0))
axes[0].set_title("A: Tactip", loc="left")
axes[1].set_title("B: Vector", loc="left")

h=images[0].shape[0]
w=images[0].shape[1]

im=np.zeros((h*((len(images))//2),w*(len(images)//2)))
print(im.shape,h,w)
print(((len(images)//2) -1) * ((len(images)//2) -1))
c=0
i=0
j=0
while i <((len(images)//2)) and c<len(images):
    while j <((len(images)//2)) and c<len(images):
        #print(c,i*h,":",((1+i)*h),j*w,":",(j+1)*w)
        percentage=(i+1)/len(images)
        #shaded=images[i]*percentage #get shade increasing over time
        #im=(im+shaded) #add the new shade
        im[i*h:((1+i)*h),j*w:(j+1)*w]=images[c]
        a=vecs[c]
        axes[1].arrow(0, 0, a[0], a[1], head_width=0.2, head_length=0.1, length_includes_head=True, facecolor=lighten_color('b', percentage))
        c+=1
        j+=1
    j=0
    i+=1
    
        
#axes[0].figure(figsize=(5,1))

axes[0].imshow(im)
axes[0].xaxis.set_visible(False)
axes[0].yaxis.set_visible(False)
#axes[1].figure(figsize=(5,1))


axes[1].set_xlim((-1.1, 1.1))
axes[1].set_ylim((-1.1, 1.1))
axes[1].set_aspect("equal")
fig.tight_layout()
#fig.savefig("./"+"save"+".pdf")
fig.show()
