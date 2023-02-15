#!/usr/bin/env python

import csv
import time
from pathlib import Path


# wait for arduino to be ready
# Ignore all previous data read
def wait_for_arduino(arduino):
    start_signal = "<Arduino is ready>".encode()
    x = arduino.read_until(start_signal)
    print(x)
    print("Reading Data from Arduino...")

# # ************** PLOTTING DATA ***************** #


# open the file in the write mode
def open_csv_file():
    fileName = "test.csv"
    filePath = Path("Data/Test")/fileName
    f = open(filePath, 'w')
    # create the csv writer
    writer = csv.writer(f)
    return f, writer


# Global variable to store the row of data
row = []

# Write the emg reading to the csv file


def write_to_csv_file(writer, emg_reading):
    # emg_reading is a string.
    # If it is empty, we have reached the end of a row -> write to file
    # else, append to row
    if not emg_reading:
        global row
        writer.writerow(row)
        print(row)
        row = []
    else:
        row.append(emg_reading)


# Read the serial data from arduino
def read_serial_data(arduino):
    if arduino.isOpen():
        try:
            return arduino.readline().strip().decode("utf-8")

        except UnicodeDecodeError as e:
            print("ERROR")
            print(e)
