import matplotlib.pyplot as plt

# Read data from the file
with open("output_data.txt", "r") as file:
    lines = file.readlines()

# Extract time, original signal, and filtered signal
time = []
original_signal = []
filtered_signal = []
for line in lines[2:]:  # Skip the first two lines (header)
    t, orig, filt = map(float, line.split())
    time.append(t)
    original_signal.append(orig)
    filtered_signal.append(filt)

# Plot the waveform
plt.figure(figsize=(10, 6))
plt.plot(time, original_signal, 'r', label='Original Signal')
plt.plot(time, filtered_signal, 'k', label='Gaussian-filtered Signal')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude (a.u.)')
plt.title('Gaussian Smoothing Filter')
plt.legend()
plt.grid(True)
plt.show()

