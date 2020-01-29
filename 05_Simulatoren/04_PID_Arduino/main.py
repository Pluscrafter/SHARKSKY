from matplotlib import pyplot as plt
from matplotlib.widgets import Slider
import serial
import numpy as np
import math

PGain = 4
IGain = 0.01
DGain = 0.1
Istwert = [0]

Sollwert = [20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
            20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,
            20,20,20,20,20,20,20,20,20,20,20,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,
            90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,90,
            50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,50,
            20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,20,]

arduino = serial.Serial('COM5', 9600, timeout=0.0)
plt.ion()
fig, ax = plt.subplots()
plt.subplots_adjust(left=0.25, bottom=0.30)

axcolor = 'lightgoldenrodyellow'
axP = plt.axes([0.2, 0.1, 0.65, 0.03], facecolor=axcolor)
axI = plt.axes([0.2, 0.15, 0.65, 0.03], facecolor=axcolor)
axD = plt.axes([0.2, 0.2, 0.65, 0.03], facecolor=axcolor)

PSild = Slider(axP, 'P-Gain', 0, 8, 4, valstep=0.1)
ISild = Slider(axI, 'I-Gain', 0, 1, 0.01, valstep=0.01)
DSild = Slider(axD, 'D-Gain', 0, 1, 0.08, valstep=0.01)


fig.canvas.draw()

t = [0.0]



def update(val):
    PGain = PSild.val
    IGain = ISild.val
    DGain = DSild.val

PSild.on_changed(update)
ISild.on_changed(update)
DSild.on_changed(update)
ax.legend()

i = 0;
lastval = Sollwert[0]

try:
    Istwert.append(float((arduino.readline()[:-2]).decode('utf-8')))
except:
    pass
data = str(PGain) + " " + str(IGain) + " " + str(DGain) + " " + str(Sollwert[0]) + '\n'
arduino.write(str.encode(data))

while(True):
    try:
            Istwert.append(float((arduino.readline()[:-2]).decode('utf-8')))
    except:
        continue
    if(Sollwert[i] != lastval):
        data = str(PGain) + " " + str(IGain) + " " + str(DGain) + " " + str(Sollwert[i]) + '\n'
        arduino.write(str.encode(data))
        arduino.write(str.encode(data))
        print(data)

    lastval = Sollwert[i]


    ax.cla()
    ax.plot(Sollwert, label='Sollwert')
    ax.plot(Istwert, label='Istwert')
    i += 1

    if arduino.in_waiting > 2:
        arduino.flush()
        arduino.flushInput()
        arduino.flushOutput()

    if(i > len(Sollwert)-1):
        i = 0
        Istwert = [0]

    fig.canvas.draw()