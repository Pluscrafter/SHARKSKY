from matplotlib import pyplot as plt
import numpy as np
import math


def digital_low_pass(cutoff_frequency, input):
    # https://en.wikipedia.org/wiki/Low-pass_filter#Simple_infinite_impulse_response_filter
    fc = 2 * math.pi * cutoff_frequency
    alpha = (fc * dt) / (1 + fc * dt)
    output = [0] * len(input)
    output[0] = input[0]

    print(alpha)

    for i in range(1, len(input)):
        output[i] = alpha * input[i] + (1 - alpha) * output[i - 1]
    return output


with open('LOG','r') as file:
    line = []
    for i in file:
        line.append(i.rstrip('\n'))

time = []
dtime = []
dt = 0

pitch = []
roll = []

for i in line:
    x = i.split('\t')
    pitch.append(float(x[0]))
    roll.append(float(x[1]))
    time.append(float(x[3]))

plt.plot(time, pitch)
plt.plot(time, roll)
plt.show()

for i in range(1, len(time)):
    dtime.append(time[i]-time[i-1])

for i in dtime:
    dt += i

dt = dt/len(dtime)
freq = 1.0/dt

fpitch = digital_low_pass(80, pitch)
froll = digital_low_pass(80, roll)

print(dt)
pitch = np.asarray(froll)
y = np.fft.fft(froll)
y2 = np.fft.fft(fpitch)
N = int(len(y)/2+1)
X = np.linspace(0, freq/2, N, endpoint=True)

plt.plot(X, np.abs(y[:N]/N))
plt.plot(X, np.abs(y2[:N]/N))
plt.show()