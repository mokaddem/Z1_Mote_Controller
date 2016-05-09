/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         An example of how to use the button and light sensor on
 *         the Z1 platform.
 * \author
 *         Joakim Eriksson <joakime@sics.se>
 */

#include "contiki.h"
#include <stdio.h>
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "dev/z1-phidgets.h"

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
PROCESS_THREAD(test_button_process, ev, data)
{
  static struct etimer et;
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(phidgets);
  SENSORS_ACTIVATE(button_sensor);

  while(1) {
    etimer_set(&et, CLOCK_SECOND/2);
    //printf("Please press the User Button\n");
    //PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);

    //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //leds_toggle(LEDS_GREEN);
    //printf("Button clicked\n");
    printf("Phidget 5V 2:%d\n", phidgets.value(PHIDGET5V_2));
    printf("Phidget 3V 1:%d\n", phidgets.value(PHIDGET3V_1));

	switch(pushed(yAxis)){
    	case bot:
			printf("Pushed Bot\n");
			break;
		case top:
			printf("Pushed Top\n");
			break;
		default:
			break;
	}
	switch(pushed(xAxis)){
    	case left:
			printf("Pushed Left\n");
			break;
		case right:
			printf("Pushed Right\n");
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

  }
  PROCESS_END();
}

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
