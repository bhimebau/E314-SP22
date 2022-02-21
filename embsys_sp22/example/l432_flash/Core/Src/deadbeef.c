/* deadbeef.c
 * Description: 
 *
 * Author: Bryce Himebaugh 
 * Contact: bhimebau@indiana.edu
 * Date: 03.12.2021
 * Copyright (C) 2021
 *
 */

#include "main.h"
#include "deadbeef.h"
#include <stdio.h>

void deadbeef_command(char *arguments) {
  HAL_StatusTypeDef status = 0;
  printf("Real beef\n\r"); 

  HAL_FLASH_Unlock();
  if ((status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,0x0803FFE0, 0x12345678DEADBEEF))) {
    HAL_FLASH_Lock();
  }
  HAL_FLASH_Lock();
  printf("OK\n\r");
}


