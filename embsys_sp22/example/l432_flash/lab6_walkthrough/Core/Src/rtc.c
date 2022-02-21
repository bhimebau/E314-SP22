/* rtc.c
 * Description: 
 *
 * Author: Bryce Himebaugh 
 * Contact: bhimebau@indiana.edu
 * Date: 02.25.2021
 * Copyright (C) 2021
 *
 */
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern RTC_HandleTypeDef hrtc;

void ts_command(char *arguments) {
  RTC_TimeTypeDef current_time = {0};
  char * argument;
  int argument_count = 0;
  
  if (!arguments) {
    printf("NOK\n\r");
    return;
  }
  argument = strtok(arguments,",");
  if (!argument) {
    printf("NOK\n\r");
    return;
  }
  while (argument) {
    //    printf("%d: %s\n\r",argument_count,argument);
    switch (argument_count) {
    case 0:
      current_time.Hours = (int) strtol(argument,NULL,10);
      if ((current_time.Hours<0)||(current_time.Hours>23)) {
        printf("NOK\n\r");
        return;
      }
      break;
    case 1:
      current_time.Minutes = (int) strtol(argument,NULL,10);
      if ((current_time.Minutes<0)||(current_time.Minutes>59)) {
        printf("NOK\n\r");
        return;
      }
      break;
    case 2:
      current_time.Seconds = (int) strtol(argument,NULL,10);
      if ((current_time.Seconds<0)||(current_time.Seconds>59)) {
        printf("NOK\n\r");
        return;
      }
      break;
    default:
      printf("NOK\n\r");
      break;
    }
    argument_count++;
    argument=strtok(NULL,",");
    
  }
  if (argument_count != 3) {
    printf("NOK\n\r");
    return;
  }
  current_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  current_time.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &current_time, RTC_FORMAT_BIN) != HAL_OK) {
    printf("NOK\n\r");
    return;
  }
  printf("OK\n\r");
}

void ds_command(char *arguments) {
  printf("Set date real command\n\r");
  if (arguments) {
    printf("Arguments = %s\n\r",arguments);
  }
}
