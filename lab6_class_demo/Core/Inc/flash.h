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
  uint8_t status;                  // record type, 01=sensor data, 02=log data;
  uint16_t record_number;          // Which number is this particular record 
  uint32_t timestamp;              // Time, bit packed into 32 bits
  uint16_t battery_voltage;        // 16 bit battery voltage
  int16_t temperature;             // STM32 Temperature sensor reading 
  uint32_t sensor_period;          // Reading from the light sensor
} sensordata_t;

typedef struct log_data {
  uint8_t watermark;               // 0x01=populated, 0xFF=unpopulated 
  uint8_t status;                  // record type, 01=sensor data, 02=log data;
  uint16_t record_number;          // Which number is this particular record 
  uint32_t timestamp;              // Time, bit packed into 32 bits
  uint8_t msg[8];                  // String message to make the record seem less cryptic.
} logdata_t;

typedef struct raw {               // Raw structure used to write sensordata and log_data to the flash 
  uint64_t data0;
  uint64_t data1;
} raw_t;

typedef struct flash_status {
  raw_t * data_start;
  raw_t * data_end;
  raw_t * next_address;
} flash_status_t; 

#endif

int erase_one_page(uint32_t);
int write_raw(flash_status_t *fs, raw_t *data);
int flash_erase(void);
int flash_write_init(flash_status_t *);
