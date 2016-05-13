from tkinter import *
from tkinter import ttk
from tkinter import font
from PIL import ImageTk, Image
from Leds import LED
from tkinter import messagebox
import threading

import serial
import subprocess

#Variables


COLOR_RED = '#ff4422'
COLOR_GREEN = '#00FF33'
COLOR_BLUE = '#3333cc'
COLOR_ORANGE = '#ffcc00'
USER_CAN_INPUT = 0
WINDOW_POPUP = ''
SERIAL = ''
CONTROL = ''
THE_LED = ''
THE_BATTERY = ''
THE_START_BUTTON = ''

JOY_UP = "Up"
JOY_DOWN = "Down"
JOY_LEFT =  "Left"
JOY_RIGHT = "Right"
ACC_UP = "Up"
ACC_DOWN = "Down"
ACC_LEFT = "Left"
ACC_RIGHT = "Right"

def startController():
	global THE_LED 
	global THE_START_BUTTON
	THE_LED.set(COLOR_ORANGE)
	t = threading.Thread(target=manageControllerInput)	
	t.daemon = True  # thread dies when main thread (only non-daemon thread) exits.	
	try:
		t.start()
		THE_START_BUTTON['state'] = 'disabled'
	except:
		print("Error: unable to start controller software")

def manageControllerInput():
	ser = serial.Serial(
	   port='/dev/ttyUSB0',\
	   baudrate=115200,\
	   parity=serial.PARITY_NONE,\
	   stopbits=serial.STOPBITS_ONE,\
	   bytesize=serial.EIGHTBITS,\
	   timeout=5)
	print("connected to: " + ser.portstr)
	while True:
		line = ser.readline();
		line = line.decode('utf-8')
		if line:
			if "battery" in line:
				batteryLevel(line)
			elif "mode" in line:
				changeMode(line)
			else:
				if "battery" in line:
					batteryLevel(line)
				if "b" in line:
					subprocess.call(["/usr/bin/xdotool","key", JOY_DOWN])
				if "t" in line:
					subprocess.call(["/usr/bin/xdotool","key", JOY_UP])
				if "l" in line:
					subprocess.call(["/usr/bin/xdotool","key", JOY_LEFT])
				if "r" in line:
					subprocess.call(["/usr/bin/xdotool","key", JOY_RIGHT])
				if "s" in line:
					subprocess.call(["/usr/bin/xdotool","key", ACC_DOWN])
				if "n" in line:
					subprocess.call(["/usr/bin/xdotool","key", ACC_UP])
				if "o" in line:
					subprocess.call(["/usr/bin/xdotool","key", ACC_LEFT])
				if "e" in line:
					subprocess.call(["/usr/bin/xdotool","key", ACC_RIGHT])

	ser.close()

def takeInput(key):
	global USER_CAN_INPUT	
	global WINDOW_POPUP
	global CONTROL
	if USER_CAN_INPUT == 1:
		#print("Pressed:" + str(key.keysym))
		setKey(key.keysym)
	else:
		print("nop")
	USER_CAN_INPUT = 0
	WINDOW_POPUP.destroy()

def setKey(c):
	global CONTROL
	global JOY_UP
	global JOY_DOWN
	global JOY_LEFT
	global JOY_RIGHT
	global ACC_UP
	global ACC_DOWN
	global ACC_LEFT
	global ACC_RIGHT
	com = c
	if CONTROL == "Jleft":
		JOY_LEFT = com
	if CONTROL == "Jright":
		JOY_RIGHT = com
	if CONTROL == "Jup":
		JOY_UP = com
	if CONTROL == "Jdown":
		JOY_DOWN = com
	if CONTROL == "Aleft":
		ACC_LEFT = com
	if CONTROL == "Aright":
		ACC_RIGHT = com
	if CONTROL == "Aup":
		ACC_UP = com
	if CONTROL == "Adown":
		ACC_DOWN = com

def showMsg(var):
	global USER_CAN_INPUT
	global WINDOW_POPUP
	global CONTROL
	CONTROL = var
	USER_CAN_INPUT = 1
	inputWindow = Toplevel(mainframe)	
	inputWindow.title('Input window')
	inputWindow.geometry('300x200+300+120')
	inputWindow.bind('<Key>', takeInput)
	ttk.Label(inputWindow, text="Press a key!", font=appHighlightFont).grid(column=0, row=0, columnspan=4, sticky=N)
	WINDOW_POPUP = inputWindow

def batteryLevel(line):
	global THE_BATTERY	
	value = line.split(":")
	batteryLevel = int(value[-1])/38
	print("batteryLevel: "+ str(batteryLevel));
	THE_BATTERY.step(int(value[-1])/38)

def changeMode(line):	
	global THE_LED	
	value = line.split(":")
	mode = int(value[-1])
	if mode == 1: #wired
		THE_LED.set(COLOR_BLUE)
	else: #Wireless
		THE_LED.set(COLOR_GREEN)

    
#UI Init
root = Tk()
root.title("Key binder for Z1 Controler")

appHighlightFont = font.Font(family='Helvetica', size=15, weight='bold')
font.families()

mainframe = ttk.Frame(root, padding="3 3 12 12")
mainframe.grid(column=0, row=0, sticky=(N, W, E, S))
mainframe.columnconfigure(0, weight=1)
mainframe.rowconfigure(0, weight=1)

#Elements
ttk.Label(mainframe, text="Click on a button to bind the key!", font=appHighlightFont).grid(column=2, row=0, columnspan=4, sticky=N)

#Joystick
ttk.Button(mainframe, text="Left", command=lambda: showMsg("Jleft")).grid(column=0, row=2, sticky=E)
ttk.Button(mainframe, text="Up", command=lambda: showMsg("Jup")).grid(column=1, row=1, sticky=N)
ttk.Button(mainframe, text="Right", command=lambda: showMsg("Jright")).grid(column=2, row=2, sticky=W)
ttk.Button(mainframe, text="Down", command=lambda: showMsg("Jdown")).grid(column=1, row=3, sticky=N)

imageJ = Image.open('pictures/joystick.png')
imageJ = imageJ.resize((150, 150), Image.ANTIALIAS) #The (h, w) is (height, width)
imageJoy = ImageTk.PhotoImage(imageJ)
PicJoy = ttk.Label(mainframe, text="",image=imageJoy).grid(column=1, row=2, sticky=N)


#Accelerometer
ttk.Button(mainframe, text="Left", command=lambda: showMsg("Aleft")).grid(column=5, row=2, sticky=E)
ttk.Button(mainframe, text="Up", command=lambda: showMsg("Aup")).grid(column=6, row=1, sticky=N)
ttk.Button(mainframe, text="Right", command=lambda: showMsg("Aright")).grid(column=7, row=2, sticky=W)
ttk.Button(mainframe, text="Down", command=lambda: showMsg("Adown")).grid(column=6, row=3, sticky=N)

imageA = Image.open('pictures/accel.png')
imageA = imageA.resize((150, 150), Image.ANTIALIAS) #The (h, w) is (height, width)
imageAcc = ImageTk.PhotoImage(imageA)
PicJoy = ttk.Label(mainframe, text="",image=imageAcc).grid(column=6, row=2, sticky=W)

#Battery level
ttk.Label(mainframe, text="").grid(column=0, row=4, sticky=W)
ttk.Label(mainframe, text="Battery level:").grid(column=0, row=5, sticky=E)
battery = ttk.Progressbar(mainframe, orient=HORIZONTAL, length=200, mode='determinate')
battery.grid(column=1, row=5, columnspan=4, sticky=W)
battery.step(0)
THE_BATTERY = battery

#Start button
THE_START_BUTTON = ttk.Button(mainframe, text="Start the controller!", command=startController)
THE_START_BUTTON.grid(column=4, row=6, sticky=N)

#Texts
Intro = ttk.Label(mainframe, text="Led indicators:", font=appHighlightFont)
Intro.grid(column=0, row=7, sticky=W)
Color1 = ttk.Label(mainframe, text="Controler not started", foreground=COLOR_RED, background="black")
Color1.grid(column=0, row=8, sticky=W)
Color2 = ttk.Label(mainframe, text="Running in wirless mode", foreground=COLOR_GREEN, background="black")
Color2.grid(column=0, row=9, sticky=W)
Color3 = ttk.Label(mainframe, text="Running in wired mode", foreground=COLOR_BLUE, background="black")
Color3.grid(column=0, row=10, sticky=W)
Color4 = ttk.Label(mainframe, text="Running mode unknown for the moment", foreground=COLOR_ORANGE, background="black")
Color4.grid(column=0, row=11, sticky=W)

#LEDS - Show Connection State
leds = [(2, 15, 15, RAISED, 1, None, "")]
states = [(1, 0)]
for shape, w, h, app, bd, orient, outline in leds:
	for state, blink in states:
		theLed = LED(mainframe, shape=shape, status=state,
		    width=w, height=h, appearance=app,
		    orient=orient, blink=blink, bd=bd, 
		    outline=outline)
theLed.set(COLOR_RED)
THE_LED = theLed



#Configuration + Mainloop
for child in mainframe.winfo_children(): child.grid_configure(padx=5, pady=5)

root.mainloop()
