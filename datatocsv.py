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
COM = "COM4"
BAUD = "9600"
SerialPort = serial.Serial(COM, BAUD, timeout=1)

# Set up CSV file and header row
with open('data.csv', mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['Incoming Data'])

# Continuously read from serial port and store incoming data in CSV file
while True:
    IncomingData = SerialPort.readline().decode('utf-8').rstrip()
    if IncomingData:

        with open('data.csv', mode='a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow([IncomingData])
    time.sleep(0.01)
