/* led.c
 * Description: 
 *
 * Author: Bryce Himebaugh 
 * Contact: bhimebau@indiana.edu
 * Date: 02.19.2021
 * Copyright (C) 2021
 *
 */
#include "main.h" 
#include <stdio.h>


void lon(void) {
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
}

void lof(void) {
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}




void lon_command(char *arguments) {
  lon();
  printf("OK\n\r");
}

void lof_command(char *arguments) {
  lof();
  printf("OK\n\r");
}
