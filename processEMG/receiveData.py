#!/usr/bin/env python

import time
import serial
import serial.tools.list_ports
import csv
from pathlib import Path
import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from ports import open_serial_port

emg_values = [0, 0]

# wait for arduino to be ready
# Ignore all previous data read
def wait_for_arduino(device):
    start_signal = "<Arduino is ready>".encode()
    device.read_until(start_signal)
    print("Reading Data from Arduino...")

# ************** PLOTTING DATA ***************** #

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []

# This function is called periodically from FuncAnimation
def animate(i, xs, ys):
    # Add x and y to lists
    xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))
    ys.append(emg_values[0])

    # Limit x and y lists to 20 items
    xs = xs[-20:]
    ys = ys[-20:]

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, ys)

    # Format plot
    plt.xticks(rotation=45, ha='right')
    plt.subplots_adjust(bottom=0.30)
    plt.title('EMG Readings')
    plt.ylabel('EMG Value')

# Set up plot to call animate() function periodically
# ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=200)
# plt.show()


# ************** RECEIVING DATA ***************** #


device = open_serial_port()
wait_for_arduino(device)

# open the file in the write mode
fileName = "test.csv"
filePath = Path("../Data/Test")/fileName
f = open(filePath, 'w')
# create the csv writer
writer = csv.writer(f)

row=[]

while (True):
    if device.isOpen():

        try:
            sensor_val = device.readline().strip().decode("utf-8")

            # sensor_val is a string. 
            # If it is empty, we have reached the end of a row -> write to file
            # else, append to row
            if not sensor_val:
                writer.writerow(row)
                row = []
            else:
                row.append(sensor_val)
                emg_values = [int(emg_val) for emg_val in sensor_val.split(',')]
                print(sensor_val)

        except UnicodeDecodeError as e:
            print("ERROR")
            print(e)
        except KeyboardInterrupt:
            print("\nStopping read from Arduino...")
            device.close()
            f.close()
            exit(0)
