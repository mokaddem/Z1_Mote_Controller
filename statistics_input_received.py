#!/usr/bin/env python
import serial
import subprocess
import time

# This script count the number of received input during 5 seconds
# And display the counter

def startControl():
        # Open the serial port in order to receive data from the z1/remote
	ser = serial.Serial(
	   port='/dev/ttyUSB1',\
	   baudrate=115200,\
	   parity=serial.PARITY_NONE,\
	   stopbits=serial.STOPBITS_ONE,\
	   bytesize=serial.EIGHTBITS,\
	   timeout=5)
	print("connected to: " + ser.portstr)
	num_of_input = 0
	start_time = time.time()
	while (time.time() - start_time) < 5:
		line = ser.readline();
		if line:
			if "b" in line or "s" in line or "t" in line or "n" in line or "l" in line or "o" in line or "r" in line or "e" in line:
				num_of_input += 1
				
	ser.close()
	print("--- %s inputs received ---" % num_of_input)
startControl()
