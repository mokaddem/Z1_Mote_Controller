#include "contiki.h"
#include "net/rime/rime.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "dev/z1-phidgets.h"
#include "sys/clock.h"
#include <stdio.h>

static int pushed(int Axis);
static void send(char* direction);

#define xAxis 0
#define yAxis 1

#define noDir 0
#define left 1
#define right 2
#define top 3
#define bot 4

#define phiX PHIDGET5V_2
#define phiY PHIDGET3V_1
#define userButton 0 // button USR

int baseX = 1167; //PHIDGET5V_2
int baseY = 1906; //PHIDGET3V_1

int wired=1;

/*---------------------------------------------------------------------------*/
PROCESS(test_button_process, "Test Button & Phidgets");
AUTOSTART_PROCESSES(&test_button_process);
/*---------------------------------------------------------------------------*/
static struct broadcast_conn broadcast;
PROCESS_THREAD(test_button_process, ev, data)
{
  static struct etimer et;
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(phidgets);
  SENSORS_ACTIVATE(button_sensor);
  
  clock_init();
  
  // Open a broadcast connection
  broadcast_open(&broadcast, 129, NULL);

  while(1) {
    etimer_set(&et, CLOCK_SECOND/2);
    //printf("Please press the User Button\n");
    //PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
	printf("button sensor %d\n", button_sensor.value(userButton));
    //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

	switch(pushed(yAxis)){
    		case bot:
                        send("b\n");
			break;
		case top:
                        send("t\n");
			break;
		default:
			break;
	}
	switch(pushed(xAxis)){
    	        case left:
                        send("l\n");
			break;
		case right:
                        send("r\n");
			break;
		default:
			break;
	}
	clock_wait(20);
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
int pushed(int Axis){
	int errorInterval = 200;
	int curVal = Axis == xAxis ? phidgets.value(phiX) : phidgets.value(phiY);
	int base = Axis == xAxis ? baseX : baseY;
	int dirLow = Axis == xAxis ? left : bot;
	int dirHigh = Axis == xAxis ? right : top;
	if(curVal < base-errorInterval){
		return dirLow;
	}
	else if (curVal > base+errorInterval){
		return dirHigh;
	}
	else{
		return noDir;
	}
}
/*---------------------------------------------------------------------------*/
void send(char* direction){
	if(wired){
		printf(direction);
	} else {
		packetbuf_copyfrom(direction,2);
		broadcast_send(&broadcast);
	}
}