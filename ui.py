from tkinter import *
from tkinter import ttk
from tkinter import font
from PIL import ImageTk, Image
from Leds import LED
from tkinter import messagebox

#Variables
COLOR_RED = '#ff4422'
COLOR_GREEN = '#00FF33'
COLOR_BLUE = '#3333cc'
COLOR_ORANGE = '#ffcc00'
USER_CAN_INPUT = 0
WINDOW_POPUP = ''

#Functions
def takeInput(key):
	global USER_CAN_INPUT	
	global WINDOW_POPUP
	if USER_CAN_INPUT == 1:
		print("yoo:" + str(key.char))
	else:
		print("nop")
	USER_CAN_INPUT = 0
	WINDOW_POPUP.destroy()

def showMsg(var):
	global USER_CAN_INPUT
	global WINDOW_POPUP	
	USER_CAN_INPUT = 1
	inputWindow = Toplevel(mainframe)	
	inputWindow.title('Input window')
	inputWindow.geometry('300x200+300+120')
	inputWindow.bind('<Key>', takeInput)
	ttk.Label(inputWindow, text="Press a key!", font=appHighlightFont).grid(column=0, row=0, columnspan=4, sticky=N)
	WINDOW_POPUP = inputWindow

def batteryLevel():
	return 25
    
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
ttk.Button(mainframe, text="Left", command=lambda: showMsg("left")).grid(column=0, row=2, sticky=E)
ttk.Button(mainframe, text="Up", command=lambda: showMsg("up")).grid(column=1, row=1, sticky=N)
ttk.Button(mainframe, text="Right", command=lambda: showMsg("right")).grid(column=2, row=2, sticky=W)
ttk.Button(mainframe, text="Down", command=lambda: showMsg("down")).grid(column=1, row=3, sticky=N)

imageJ = Image.open('pictures/joystick.png')
imageJ = imageJ.resize((150, 150), Image.ANTIALIAS) #The (h, w) is (height, width)
imageJoy = ImageTk.PhotoImage(imageJ)
PicJoy = ttk.Label(mainframe, text="",image=imageJoy).grid(column=1, row=2, sticky=N)


#Accelerometer
ttk.Button(mainframe, text="Left", command=lambda: showMsg("left")).grid(column=5, row=2, sticky=E)
ttk.Button(mainframe, text="Up", command=lambda: showMsg("up")).grid(column=6, row=1, sticky=N)
ttk.Button(mainframe, text="Right", command=lambda: showMsg("right")).grid(column=7, row=2, sticky=W)
ttk.Button(mainframe, text="Down", command=lambda: showMsg("down")).grid(column=6, row=3, sticky=N)

imageA = Image.open('pictures/accel.png')
imageA = imageA.resize((150, 150), Image.ANTIALIAS) #The (h, w) is (height, width)
imageAcc = ImageTk.PhotoImage(imageA)
PicJoy = ttk.Label(mainframe, text="",image=imageAcc).grid(column=6, row=2, sticky=W)

#Battery level
ttk.Label(mainframe, text="").grid(column=0, row=4, sticky=W)
ttk.Label(mainframe, text="Battery level:").grid(column=0, row=5, sticky=E)
battery = ttk.Progressbar(mainframe, orient=HORIZONTAL, length=200, mode='determinate')
battery.grid(column=1, row=5, columnspan=4, sticky=W)
battery.step(batteryLevel())

#LEDS - Show Connection State
leds = [(2, 15, 15, RAISED, 1, None, "")]
states = [(1, 0)]
for shape, w, h, app, bd, orient, outline in leds:
	for state, blink in states:
		theLed = LED(mainframe, shape=shape, status=state,
		    width=w, height=h, appearance=app,
		    orient=orient, blink=blink, bd=bd, 
		    outline=outline)
theLed.set(COLOR_GREEN)

#Configuration + Mainloop
for child in mainframe.winfo_children(): child.grid_configure(padx=5, pady=5)
root.mainloop()
