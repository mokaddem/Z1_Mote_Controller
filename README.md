# Z1_Mote_Controller

### Structure

* `Z1.c` is the file for the Z1
* `Remote.c` is the file for the Remote
* There are two python scripts:
  * `read_usb.py` is the simplest, it reads the input from the serial port and translates it into the directions up, left, right, down.
  * `ui.py` is a more advanced script, it opens up an UI showing useful information about the controller (mode and battery). It also allows to map other keys to the directions of the joystick and accelerometer. Moreover, this script as an independent executable called `ui`.

### To run the python scripts
In order to execute the python scripts, please install those two dependencies:
* `pip install pyserial`
* `apt-get install xdotool`

For python3, if it does not work, please try
* `apt-get install python3-serial`

### Demo
A demo available [here](https://youtu.be/fNujyreeeCI).
