import matplotlib.pyplot as plt
import math

# deltatime for plot
dt = 0.000001


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
period = 4
period = period * 12500
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
    x.append(0.000001 * i)

for i in range(0, period):
    sinemixed.append(sine(1, 80, 0) + sine(2, 8000, math.pi) + sine(1, 800, 0.1) + sine(0.3, 1234, 50) +
                     sine(0.5, 5010, 2.3) + sine(2, 80000, 3) + sine(0.5, 1000, 2.3) + sine(0.5, 1000000, 2.3))

cutoffreq = 200
filtered_dlpf = digital_low_pass(cutoffreq, sinemixed)

plt.plot(x, sinemixed, color='blue', linewidth=1, label='80Hz sine with noise')
plt.plot(x, filtered_dlpf, color='red', linewidth=1, label='80Hz sine filtered with dlpf')
plt.plot(x, sine80, color='green', linewidth=3, label='80Hz sine')
plt.legend()
plt.show()