#!/usr/bin/env python

import time
import serial
import serial.tools.list_ports

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

while (True):
    if device.isOpen():

        try:
            input_data = device.readline()
            s1 = input_data.strip().decode("utf-8")
            print(s1)
        except UnicodeDecodeError as e:
            print("ERROR")
            print(e)
            print(input_data)
        except KeyboardInterrupt:
            print("\nStopping read from Arduino...")
            device.close()
            exit(0)
