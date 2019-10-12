import matplotlib.pyplot as plt
import math
import numpy as np

# deltatime for plot
dt = 0.0001
# samplefreq
fs = 1.0/dt


# sine function with a dt of 0.000001 -> 1MHz "sample rate"
def sine(enlongation, frequency, phase):
    # Y * sin(w*t), w = 2*pi*f, t = 0.000001
    return enlongation * math.sin(2 * math.pi * frequency * i * dt + phase)


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


# number of sine periods
period = 10
period = period * 125
# x is the x axis value with steps of 0.000001 -> 1MHz "sample rate"
x = []
# sine80 is a list with numerical values for x sine period with the frequency of 80Hz
sine80 = []
# sinemixed is a list with numerical values for x sine period with the frequency of 80Hz and harmonics with
# 1.234kHz, 5.01kHz and 10khz
sinemixed = []

# this loop calculate the values for the sine80 list
for i in range(0, period):
    sine80.append(sine(1, 80, 0))
    x.append(dt * i)

for i in range(0, period):
    sinemixed.append(sine(1, 80, 0) + sine(1, 133, math.pi) + sine(1, 800, 0.1) + sine(0.3, 1234, 50) +
                     + sine(0.5, 1000, 2.3) + sine(0.5, 3000, 2.3))

cutoffreq = 80
filtered_dlpf = digital_low_pass(cutoffreq, sinemixed)

plt.plot(x, sinemixed, color='blue', linewidth=1, label='80Hz sine with noise')
plt.plot(x, filtered_dlpf, color='red', linewidth=1, label='80Hz sine filtered with dlpf')
plt.plot(x, sine80, color='green', linewidth=3, label='80Hz sine')
plt.legend()
plt.show()

# https://www.cbcity.de/die-fft-mit-python-einfach-erklaert 12.10.19 18:23
y1 = np.fft.fft(sinemixed)
y2 = np.fft.fft(filtered_dlpf)
y3 = np.fft.fft(sine80)

N = int(len(y1)/2+1)

X = np.linspace(0, fs/2, N, endpoint=True)


plt.plot(X, np.abs(y1[:N]/N), color='blue', linewidth=1, label='80Hz sine with noise')
plt.plot(X, np.abs(y3[:N]/N), color='green', linewidth=3, label='80Hz sine')
plt.plot(X, np.abs(y2[:N]/N), color='red', linewidth=1, label='80Hz sine filtered with dlpf')
plt.legend()
plt.show()