/* flash.c
 * Description: 
 *
 * Author: Bryce Himebaugh 
 * Contact: bhimebau@indiana.edu
 * Date: 03.16.2021
 * Copyright (C) 2021
 *
 */

#include <string.h>
#include <stdio.h>
#include "main.h"
#include "flash.h"

extern int _edata;
extern int _sdata;
extern int __fini_array_end;
extern flash_status_t flash_status;

static uint32_t GetPage(uint32_t);
static uint32_t GetBank(uint32_t);

void ef_command(char *args) {
  if (!args) {
    printf("NOK\n\r");
    return;
  }
  else {
    if (!strcmp("all",args)) {
      flash_erase();
      printf("Erase Here!\n\r");
    }
    else {
      printf("NOK\n\r");
      return;
    }
  }
  printf("OK\n\r");
}

void flash_info_command(char * args) {
  printf("Start of Flash Data: %p\n\r",flash_status.data_start);
  printf("End of Flash Data: %p\n\r",flash_status.data_end);
  printf("Next Address: %p\n\r",flash_status.next_address);
  printf("Total Records Possible: %d\n\r",((int)flash_status.data_start-(int)flash_status.data_end)/16);
  printf("Total Empty Records: %d\n\r",((int)flash_status.next_address-(int)flash_status.data_end)/16);
  printf("OK\n\r");
}

int flash_write_init(flash_status_t * fs) {
  sensordata_t * p;
  uint32_t program_end = ((uint32_t )&__fini_array_end
                  + (uint32_t)&_edata
                  - (uint32_t)&_sdata);
  uint32_t bottom  = (((uint32_t)program_end & ~0x7FF) + 0x800);
  fs->data_end = (raw_t *) bottom;
  fs->data_start = (raw_t *) 0x0803FFF0;
  p = (sensordata_t *) fs->data_start;
  while (p->watermark!=0xff) {
    p--;
    if (p<=((sensordata_t *)bottom)) return (-1);
  }
  fs->next_address = (raw_t *) p;
  return(0);
}

int write_raw(flash_status_t *fs, raw_t *data) {
  HAL_StatusTypeDef status = 0;
  uint32_t addr;

  if ((!data) || (!fs)) {
    return(-1); 
  }
  HAL_FLASH_Unlock();
  addr = (uint32_t) fs->next_address;
  if ((status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,addr, data->data0))) {
    HAL_FLASH_Lock();
    return(-4);
  }
  addr += 8;
  if ((status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,addr, data->data1))) {
    HAL_FLASH_Lock();
    return(-4);
  }
  HAL_FLASH_Lock();
  fs->next_address--;
  return(0);
}

int erase_one_page(uint32_t addr) {
  uint32_t PAGEError = 0;
  static FLASH_EraseInitTypeDef EraseInitStruct;
  
  HAL_FLASH_Unlock();
  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = GetBank(addr);
  EraseInitStruct.Page        = GetPage(addr);
  EraseInitStruct.NbPages     = 1;
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
    HAL_FLASH_Lock();
    return (-1);
  }
  HAL_FLASH_Lock();
  return(0);
}

static uint32_t GetPage(uint32_t Addr) {
  uint32_t page = 0;
  
  if (Addr < (FLASH_BASE + FLASH_BANK_SIZE))
  {
    /* Bank 1 */
    page = (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;
  }
  else
  {
    /* Bank 2 */
    page = (Addr - (FLASH_BASE + FLASH_BANK_SIZE)) / FLASH_PAGE_SIZE;
  }
  
  return page;
}

static uint32_t GetBank(uint32_t Addr) {
  return FLASH_BANK_1;
}

int flash_erase(void) {
  uint32_t FirstPage = 0, NbOfPages = 0, BankNumber = 0;
  uint32_t PAGEError = 0;
  /* __IO uint32_t data32 = 0 , MemoryProgramStatus = 0; */

  static FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t program_end = ((uint32_t )&__fini_array_end 
                  + (uint32_t)&_edata
                  - (uint32_t)&_sdata); 
  uint32_t bottom  = (((uint32_t)program_end & ~0x7FF) + 0x800);
  uint32_t top = FLASH_END;
  
  HAL_FLASH_Unlock();
  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 
  /* Get the 1st page to erase */
  FirstPage = GetPage(bottom);
  /* Get the number of pages to erase from 1st page */
  NbOfPages = GetPage(top) - FirstPage + 1;
  /* Get the bank */
  BankNumber = GetBank(bottom);
  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = BankNumber;
  EraseInitStruct.Page        = FirstPage;
  EraseInitStruct.NbPages     = NbOfPages;

  /* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {
    HAL_FLASH_Lock();
    return (-1);
  }
  HAL_FLASH_Lock();
  flash_write_init(&flash_status);
  return(0);
} 


