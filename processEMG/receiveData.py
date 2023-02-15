#!/usr/bin/env python

import csv
import stopwatch
from pathlib import Path

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
# emg_reading is a string.
# If it is empty, we have reached the end of a row -> write to file
# else, append to row
def write_to_csv_file(writer, emg_reading):
    global row
    if not emg_reading:
        row.insert(0, stopwatch.get_elapsed_time())
        writer.writerow(row)
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
