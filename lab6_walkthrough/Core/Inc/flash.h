/* flash.h
 * Description: 
 *
 * Author: Bryce Himebaugh 
 * Contact: bhimebau@indiana.edu
 * Date: 03.16.2021
 * Copyright (C) 2021
 *
 */

#ifndef FLASH_H
#define FLASH_H

typedef struct sensordata {
  uint8_t watermark;               // 0x01=populated, 0xFF=unpopulated 
  uint8_t status;                  // record type, 01=sensor data, 02=error data;
  uint16_t record_number;          // Which number is this particular record  
  uint32_t timestamp;              // Time, bit packed into 32 bits
  uint16_t battery_voltage;        // 16 bit battery voltage
  int16_t temperature;             // STM32 Temperature sensor reading 
  int light_data;                  // Reading from the light sensor
} sensordata_t;

typedef struct log_data {
  uint8_t watermark;               // 0x01=populated, 0xFF=unpopulated 
  uint8_t status;                  // record type, 01=sensor data, 02=error data;
  uint16_t record_number;          // Which number is this particular record  
  uint32_t timestamp;              // Time, bit packed into 32 bits
  uint8_t length;                  // message length
  uint8_t msg[7];                  // String message to make the record seem less cryptic.
} logdata_t;

typedef struct raw {
  uint64_t data0;
  uint64_t data1;
} raw_t;

#endif

int write_raw(raw_t *, uint32_t *);
int erase_page(uint32_t);
static uint32_t GetPage(uint32_t);
static uint32_t GetBank(uint32_t); 
