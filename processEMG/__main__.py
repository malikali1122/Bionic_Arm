#!/usr/bin/env python

import ports
from receiveData import read_serial_data, open_csv_file, write_to_csv_file
import stopwatch

try:
    #  Open serial port
    arduino = ports.open_serial_port()

    # Wait for arduino to be ready
    ports.wait_for_arduino(arduino)
    (f, writer) = open_csv_file()

    # Read data from arduino
    stopwatch.start()
    while (True):
        emg_reading = read_serial_data(arduino)
        print(stopwatch.get_elapsed_time())
        write_to_csv_file(writer, emg_reading)

except KeyboardInterrupt:
    print("\nStopping read from Arduino...")
    arduino.close()
    print("Closing the CSV file...")
    f.close()
    print("Exiting the program...")
    exit(0)
