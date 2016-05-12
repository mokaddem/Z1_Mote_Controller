#!/usr/bin/env python
import serial
import subprocess

def startControl():
	ser = serial.Serial(
	   port='/dev/ttyUSB0',\
	   baudrate=115200,\
	   parity=serial.PARITY_NONE,\
	   stopbits=serial.STOPBITS_ONE,\
	   bytesize=serial.EIGHTBITS,\
	   timeout=0)
	print("connected to: " + ser.portstr)
	ser.write("help\n");
	while True:
		line = ser.readline();
		if line:
			if "b" in line or "s" in line:
				subprocess.call(["/usr/bin/xdotool","key","Down"])
			if "t" in line or "n" in line:
				subprocess.call(["/usr/bin/xdotool","key","Up"])
			if "l" in line or "o" in line:
				subprocess.call(["/usr/bin/xdotool","key","Left"])
			if "r" in line or "e" in line:
				subprocess.call(["/usr/bin/xdotool","key","Right"])
	ser.close()
startControl()
