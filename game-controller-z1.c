#include "contiki.h"
#include <stdio.h>
#include "dev/button-sensor.h"
#include "dev/leds.h"
//#include "net/rime/rime.h"
#include "dev/z1-phidgets.h"
//#include "dev/slip.h"

static int pushed(int Axis);

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

/*---------------------------------------------------------------------------*/
PROCESS(test_button_process, "Test Button & Phidgets");
AUTOSTART_PROCESSES(&test_button_process);
/*---------------------------------------------------------------------------*/
/*static void
recv_uc(struct unicast_conn *c, const linkaddr_t *from)
{
  printf("unicast message received from %d.%d\n",
	 from->u8[0], from->u8[1]);
}
static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;*/
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_button_process, ev, data)
{
  static struct etimer et;
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(phidgets);
  SENSORS_ACTIVATE(button_sensor);
  //linkaddr_t addr;
  //unicast_open(&uc, 146, &unicast_callbacks);
  // SLIP
  //slip_arch_init(BAUD2UBR(115200));

  while(1) {
    etimer_set(&et, CLOCK_SECOND/2);
    //printf("Please press the User Button\n");
    //PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);

    //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //leds_toggle(LEDS_GREEN);
    //printf("Button clicked\n");
    //printf("Phidget 5V 2:%d\n", phidgets.value(PHIDGET5V_2));
    //printf("Phidget 3V 1:%d\n", phidgets.value(PHIDGET3V_1));

	switch(pushed(yAxis)){
    		case bot:
			//putchar('b');
printf("b\n");
			break;
		case top:
			break;
		default:
			break;
	}
	switch(pushed(xAxis)){
    	case left:
			break;
		case right:
			break;
		default:
			break;
	}

/*
	if (pushed(yAxis)) {
      leds_on(LEDS_RED);
	  leds_off(LEDS_BLUE);
	  printf("Pushed Bot\n");
    } else {
      leds_off(LEDS_RED);
	  leds_on(LEDS_BLUE);
	  printf("Pushed Top\n");
    }
    if (pushed(xAxis)) {
      leds_on(LEDS_GREEN);
      leds_off(LEDS_YELLOW);
	  printf("Pushed Left\n");
    } else {
      leds_off(LEDS_GREEN);
      leds_on(LEDS_YELLOW);
	  printf("Pushed Right\n");
    }
*/
    /*packetbuf_copyfrom("Hello", 5);
    addr.u8[0] = 1;
    addr.u8[1] = 0;
    if(!linkaddr_cmp(&addr, &linkaddr_node_addr)) {
      unicast_send(&uc, &addr);
    }*/

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
