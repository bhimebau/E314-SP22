/* tsl237.c
 * Description: 
 *
 * Author: Bryce Himebaugh 
 * Contact: bhimebau@indiana.edu
 * Date: 03. 5.2021
 * Copyright (C) 2021
 *
 */

#include "main.h"
#include <string.h>
#include <stdio.h>
#include "tsl237.h"

extern uint32_t period;

void tsl237_command(char *arguments) {
  float clock_period;
  float sensor_period;
  float sensor_frequency;
  
  if (arguments) {
    printf("NOK\n\r");
  }
  else {
    clock_period = SystemCoreClock;
    clock_period = 1/clock_period;
    sensor_period = clock_period * (float) period;
    sensor_frequency = 1/sensor_period;
    printf("%0.3f hz\n\r", sensor_frequency);
    printf("OK\n\r");
  }
}
