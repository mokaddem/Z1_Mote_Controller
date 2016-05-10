#!/usr/bin/env python
import serial
import subprocess
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
		if "b" in line:
			#print("bottom");
			subprocess.call(["/usr/bin/xdotool","key","Down"])
		if "t" in line:
			subprocess.call(["/usr/bin/xdotool","key","Up"])
			#print("top");
		if "l" in line:
			subprocess.call(["/usr/bin/xdotool","key","Left"])
			#print("left");
		if "r" in line:
			subprocess.call(["/usr/bin/xdotool","key","Right"])
			#print("right");
ser.close()
