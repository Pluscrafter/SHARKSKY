from matplotlib import pyplot as plt
from matplotlib.widgets import Slider
import numpy as np
import math

PGain = 0.4
IGain = 0.002
DGain = 0.2

fig, ax = plt.subplots()
plt.subplots_adjust(left=0.25, bottom=0.25)

axcolor = 'lightgoldenrodyellow'
axP = plt.axes([0.2, 0.1, 0.65, 0.03], facecolor=axcolor)
axI = plt.axes([0.2, 0.15, 0.65, 0.03], facecolor=axcolor)
axD = plt.axes([0.2, 0.2, 0.65, 0.03], facecolor=axcolor)

PSild = Slider(axP, 'P-Gain', 0, 1, 0.4, valstep=0.01)
ISild = Slider(axI, 'I-Gain', 0, 1, 0.1, valstep=0.01)
DSild = Slider(axD, 'D-Gain', 0, 1, 0.2, valstep=0.01)



t = [0.0]

def update(val):
    PGain = PSild.val
    IGain = ISild.val / 100
    DGain = DSild.val
    Sollwert = [0.0, 0.0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
                10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10,10, 10, 10, 10, 10, 10, 10, 30, 30, 30, 30,
                30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
                10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
                10, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
                30, 30, 30, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]

    Istwert = [0.0, 0.0]

    Fehler = [0.0, 0.0]

    IFehler = 0

    for i in range(1, len(Sollwert)):
        t.append(i*0.1)
        IFehler += Sollwert[i]-Istwert[i]
        Fehler.append(Sollwert[i]-Istwert[i])
        Istwert.append(Istwert[i] + PGain * Fehler[i]+DGain*(Fehler[i]-Fehler[i-1])+IGain*IFehler)
    fig.canvas.draw_idle()

    ax.cla()
    ax.plot(Sollwert)
    ax.plot(Istwert)


PSild.on_changed(update)
ISild.on_changed(update)
DSild.on_changed(update)
plt.show()