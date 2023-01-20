import numpy as np
import matplotlib.pyplot as plt

trials=50
f=np.zeros((2,trials))
f=[[ 49,147,6,139,155,
48, 86, 68, 27, 32,
76, 44,124, 10, 87,
1,183,197,131,143,
51, 36,173,160, 86,
162, 97,131,  3, 17,
187, 51,150,120, 38,
121,197,197,170, 34,
9, 24,  2, 98, 28,
174, 21,111, 33, 68],
[ 72.40488751,62.14609337,71.05301651,84.68137255,91.50326797
, 90.19607843,90.8496732, 75.68215277,92.81045752,88.23529412
, 70.69525807,82.94261151,89.91041258,75.64124687,93.20261438
, 84.21024402,91.66913898,88.84873083,84.39326732,84.6310123
, 90.8496732, 96.73202614,81.10444733,83.00653595,89.71617762
, 71.41936103,80.72312796,79.25452744,85.62091503,78.62965202
, 74.39987868,93.74238584,81.45925037,82.48705788,85.62091503
, 84.96732026,70.50905812,85.8053927, 88.12419429,91.50326797
, 84.06201502,90.8496732, 84.74481991,81.69934641,93.52963992
, 90.19607843,92.81045752,74.99018816,89.54248366,76.28135267]]
f1=np.zeros((2,trials))
f1=[[ 49,147,6,139,155,
48, 86, 68, 27, 32,
76, 44,124, 10, 87,
1,183,197,131,143,
51, 36,173,160, 86,
162, 97,131,  3, 17,
187, 51,150,120, 38,
121,197,197,170, 34,
9, 24,  2, 98, 28,
174, 21,111, 33, 68],
[ 72.40488751,62.14609337,71.05301651,84.68137255,91.50326797
, 90.19607843,90.8496732, 75.68215277,92.81045752,88.23529412
, 70.69525807,82.94261151,89.91041258,75.64124687,93.20261438
, 84.21024402,91.66913898,88.84873083,84.39326732,84.6310123
, 90.8496732, 96.73202614,81.10444733,83.00653595,89.71617762
, 71.41936103,80.72312796,79.25452744,85.62091503,78.62965202
, 74.39987868,93.74238584,81.45925037,82.48705788,85.62091503
, 84.96732026,70.50905812,85.8053927, 88.12419429,91.50326797
, 84.06201502,90.8496732, 84.74481991,81.69934641,93.52963992
, 90.19607843,92.81045752,74.99018816,89.54248366,76.28135267]]

fig,ax = plt.subplots(1,2,figsize=(9, 4))
print(f)
print(f1)
bplot1 = ax[0].boxplot(list([f[0],f1[0]]),
                     vert=True,  # vertical box alignment
                     patch_artist=True,labels=["Chaotic neuron","FNN"])  # will be used to label x-ticks
bplot2 = ax[1].boxplot(list([f[1],f1[1]]),
                     vert=True,  # vertical box alignment
                     patch_artist=True,labels=["Chaotic neuron","FNN"])  # will be used to label x-ticks


ax[0].set_title('Shortest generation of convergence')

ax[0].yaxis.grid(True)
#ax[0].set_xlabel('Simulation models')
ax[0].set_ylabel('Generation')

ax[1].set_title('Simulaion results as fitnesses over trials')

ax[1].yaxis.grid(True)
#ax[1].set_xlabel('Simulation models')
ax[1].set_ylabel('Fitness of trials')

fig.show()
plt.show()