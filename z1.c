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
 *         Testing the broadcast layer in Rime
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "net/rime/rime.h"

#include "dev/button-sensor.h"
#include "dev/battery-sensor.h"
#include "dev/z1-phidgets.h"
#include "dev/adxl345.h"
#include "dev/battery-sensor.h"

#include "dev/leds.h"

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

#define LOW_BATTERY_LEVEL = 500

int baseX = 1167; //PHIDGET5V_2
int baseY = 1906; //PHIDGET3V_1

// This is changed in order to be in wired or wireless mode.
int wired=1;
// Battery level 
int batteryLow=0;

/*---------------------------------------------------------------------------*/
PROCESS(z1_game_controller, "Z1 game controller");
AUTOSTART_PROCESSES(&z1_game_controller, &test_battery_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const linkaddr_t *from)
{
  /*printf("broadcast message received from %d.%d: '%s'\n",
         from->u8[0], from->u8[1], (char *)packetbuf_dataptr());*/
}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(z1_game_controller, ev, data)
{
  static struct etimer et;
  int16_t x, y;

  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();
  // Activate sensors
  SENSORS_ACTIVATE(phidgets);
  SENSORS_ACTIVATE(button_sensor);
  SENSORS_ACTIVATE(adxl345);
  SENSORS_ACTIVATE(battery_sensor);
 
  // initiate broadcast
  broadcast_open(&broadcast, 129, &broadcast_call);

  while(1) {

    /* Delay */
    etimer_set(&et, 20);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
   /* Joystick */
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

    /* Accelerometer */
   
    x = adxl345.value(X_AXIS);
    y = adxl345.value(Y_AXIS);

    if(x > 130){
      send("n\n");
    } else if(x < -130){
      send("s\n");
    } 
    if(y > 130){
      send("o\n");
    } else if(y < -130){
      send("e\n");
    }

    /* Switch mode button sensor */
    if(data == &button_sensor){
       printf("Change wired");
       wired=(wired+1)%2;
    }
  }

  PROCESS_END();
}
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
/* Function sending the char either on the USB/serial or with RIME */
void send(char* direction){
	if(wired){
		printf(direction);
	} else {
		packetbuf_copyfrom(direction,2);
		broadcast_send(&broadcast);
	}
}
/*---------------------------------------------------------------------------*/
/* Process reading the battery status every minute. 
 * Turn on the RED led if the battery is getting under the LOW_BATTERY_LEVEL. */
PROCESS(test_battery_process, "Battery Sensor Test");
PROCESS_THREAD(test_battery_process, ev, data)
{
  static struct etimer et;
  PROCESS_BEGIN();

  SENSORS_ACTIVATE(battery_sensor);

  leds_off(LEDS_RED); // first, turn the leds off
  
  etimer_set(&et, CLOCK_SECOND * 60); //define a 60 seconds timer

  while(1) {
    uint16_t bateria = battery_sensor.value(0);
    if (batteria < LOW_BATTERY_LEVEL){
		leds_on(LEDS_RED);
	} else {
		leds_off(LEDS_RED);
	}
	/* Delay */
    etimer_reset(&et);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }

  SENSORS_DEACTIVATE(battery_sensor);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
