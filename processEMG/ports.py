#!/usr/bin/env python

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
    return serial.Serial(port, 115200, timeout=10)
