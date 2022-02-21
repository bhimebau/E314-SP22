/* time.c
 * Description: 
 *
 * Author: Bryce Himebaugh 
 * Contact: bhimebau@indiana.edu
 * Date: 03.19.2021
 * Copyright (C) 2021
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"

extern RTC_HandleTypeDef hrtc;


void time_command (char *args) {
  struct tm t;
  time_t t_of_day;
  t.tm_year = 2021-1900;  // Year - 1900
  t.tm_mon = 2;           // Month, where 0 = jan
  t.tm_mday = 19;         // Day of the month
  t.tm_hour = 10;
  t.tm_min = 12;
  t.tm_sec = 00;
  t.tm_isdst = -1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown
  t_of_day = mktime(&t);
  printf("When should we panic: 0x%08x seconds\n\r", 0x7FFFFFFF - (int) t_of_day);
  printf("OK\n\r");
}

int sample_time_test(void) {
  /* Function that reads the RTC time, converts it to epoch time using
     mktime, and then converts it back using localtime
   */
  RTC_TimeTypeDef current_time;
  RTC_DateTypeDef current_date;
  struct tm t = {0};
  struct tm * recover;
  time_t t_of_day;
  
  HAL_RTC_GetTime(&hrtc,&current_time,RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc,&current_date,RTC_FORMAT_BIN);
  printf("RTC Reports the following Date: %02d/%02d/20%02d\n\r",
         current_date.Month,
         current_date.Date,
         current_date.Year);

  printf("RTC Reports the following Time: %02d:%02d:%02d\n\r",
         current_time.Hours,
         current_time.Minutes,
         current_time.Seconds);

  t.tm_year = current_date.Year + 100;  
  t.tm_mon = current_date.Month - 1; 
  t.tm_mday = current_date.Date; 
  t.tm_hour = current_time.Hours;
  t.tm_min = current_time.Minutes;
  t.tm_sec = current_time.Seconds;
  t.tm_isdst = -1;    
  t_of_day = mktime(&t);
  printf("Current Epoch time Converted from RTC: %d\n\r", (int) t_of_day);
  recover = localtime(&t_of_day);
  printf("Epoch Time Recovered Date: %02d/%02d/20%02d\n\r",
         recover->tm_mday,
         recover->tm_mon + 1,
         recover->tm_year - 100);

  printf("Epoch Time Recovered Time: %02d:%02d:%02d\n\r",
         recover->tm_hour,
         recover->tm_min,
         recover->tm_sec);
  free(recover);
  return (0);
}
