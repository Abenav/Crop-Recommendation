import serial
import time
import sys
import signal
import csv

# Set up signal handler to close program
def signal_handler(signal, frame):
    print("Closing program")
    SerialPort.close()
    sys.exit(0)

# Set up serial port and communication parameters
COM = "COM4"  # COM port to which the device is connected, change as needed
BAUD = "9600"  # Baud rate for serial communication
SerialPort = serial.Serial(COM, BAUD, timeout=1)  # Open a serial connection

# Set up CSV file and header row
with open('data.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['Incoming Data'])  # Write a header row to the CSV file

# Continuously read from the serial port and store incoming data in the CSV file
while True:
    IncomingData = SerialPort.readline().decode('utf-8').rstrip()  # Read and decode incoming data
    if IncomingData:  # Check if there's valid incoming data

        with open('data.csv', mode='a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow([IncomingData])  # Write the incoming data to the CSV file
    time.sleep(0.01)  # Delay to avoid busy-waiting
