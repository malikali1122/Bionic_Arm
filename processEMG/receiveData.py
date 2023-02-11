#!/usr/bin/env python

import time
import serial
import serial.tools.list_ports
import csv
from pathlib import Path

#
# Find the USB port we are on
#
def get_port():
    commports = serial.tools.list_ports.comports()  # get possible ports
    numPorts = len(commports)
    if (numPorts == 0):
        print("No serial ports available\n\n")
        exit()
    if (numPorts > 1):
        # Have user pick one
        portNum = 0
        for port in commports:
            print("port number ", portNum)
            print(port)
            portNum = portNum+1
        usePort = int(input('enter port number to use 0-'+str(numPorts-1)+':'))
    else:
        usePort = 0

    thePort = commports[usePort][0]
    print('using ', thePort, '\n')
    
    return thePort

# open serial port
def open_serial_port():
    port = get_port()
    return serial.Serial(port, 115200, timeout=1)

# wait for arduino to be ready
# Ignore all previous data read
def wait_for_arduino(device):
    start_signal = "<Arduino is ready>".encode()
    device.read_until(start_signal)
    print("Reading Data from Arduino...")


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

        except UnicodeDecodeError as e:
            print("ERROR")
            print(e)
        except KeyboardInterrupt:
            print("\nStopping read from Arduino...")
            device.close()
            f.close()
            exit(0)
