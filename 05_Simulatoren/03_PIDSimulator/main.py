from matplotlib import pyplot as plt
import numpy as np
import math

Sollwert = [0.0,0.0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]


[float(i) for i in Sollwert

 ]
PGain = 0.2
DGain = 0.1

Istwert = [0.0,0.0]

preverror = 0

lerror = []

for i in range(1, len(Sollwert)):
    error = Sollwert[i]-Istwert[i]
    lerror.append(error)
    Istwert.append(error*PGain)
    Istwert[i] = Istwert[i]*6

    preverror = error

plt.plot(Sollwert)
plt.plot(Istwert)
plt.show()