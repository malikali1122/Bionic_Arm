#!/usr/bin/env python

from ports import open_serial_port
from receiveData import wait_for_arduino, read_serial_data, open_csv_file, write_to_csv_file

try:
    #  Open serial port
    arduino = open_serial_port()

    # Wait for arduino to be ready
    wait_for_arduino(arduino)
    (f, writer) = open_csv_file()

    # Read data from arduino
    while (True):
        emg_reading = read_serial_data(arduino)
        write_to_csv_file(writer, emg_reading)

except KeyboardInterrupt:
    print("\nStopping read from Arduino...")
    arduino.close()
    print("\nClosing the CSV file...")
    f.close()
    print("\nExiting the program...")
    exit(0)
