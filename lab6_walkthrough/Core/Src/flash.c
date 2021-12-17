/* flash.c
 * Description: 
 *
 * Author: Bryce Himebaugh 
 * Contact: bhimebau@indiana.edu
 * Date: 03.16.2021
 * Copyright (C) 2021
 *
 */

#include "main.h"
#include "flash.h"
#include <stdio.h>
#include <stdint.h>

extern int assert_failed_flag;

void report_test_status(int test_num) {
  if (assert_failed_flag) {
    printf("Test %d failed\n\r",test_num);
    assert_failed_flag = 0;
  }
  else {    
    printf("Test %d passed\n\r",test_num);
  }
}

void write_raw_test_command(char *arguments) {
  char tdata[16] = {0,1,2,3,4,5,6,7,
                    8,9,10,11,12,13,14,15};
  uint32_t * addr = (uint32_t *) 0x0803FFF0;
  erase_page((uint32_t) addr);

  // Test 1, pointer to data is 0
  assert_param(write_raw(0, addr)==-1);
  report_test_status(1);

  // Test 2, pointer to address is 0
  assert_param(write_raw((raw_t *) tdata, 0)==-1);
  report_test_status(2);

  // Test 3, pointer to address is misaligned
  assert_param(write_raw((raw_t *) tdata, (uint32_t *) 0x0803FFF8)==-2);
  report_test_status(3);

  // Test 4, attempt to write data to an empty cell 
  assert_param(write_raw((raw_t *) tdata, addr)==0);
  report_test_status(4);

  // Test 5, attempt to write data to a non-erased space
  assert_param(write_raw((raw_t *) tdata, addr)==-3);
  report_test_status(5);

  
}

/* write_raw
inputs: 
data: pointer to data structure 
address: location to write
Returns
0: success
-1: failed because one or both of the pointers is 0
-2: failed because address not on an 16 byte boundary 
-3: failed because location not blank
-4: HAL failed to program first 8 bytes
-5: HAL failed to program the second 8 bytes
*/
int write_raw(raw_t *data, uint32_t *address) {
  HAL_StatusTypeDef status = 0;
  
  // Check to make sure that both pointers are not 0
  if ((!data) || (!address)) {
    return(-1); 
  }
   
  // Check to make sure that the address is aligned on 16 byte boundary
  if ((((unsigned long) address)&0x0F) != 0) {
    return(-2);
  }
  // Check to see if location is all Fs indicating it is erased
  if ((*address!=0xFFFFFFFF) ||
      (*(address+1)!=0xFFFFFFFF)) {
    return(-3);
  }
  HAL_FLASH_Unlock();
  if ((status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,(uint32_t) address, data->data0))) {
    HAL_FLASH_Lock();
    return(-4);
  }
  address+=2;
  if ((status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,(uint32_t) address, data->data1))) {
    HAL_FLASH_Lock();
    return(-5);
  }
  HAL_FLASH_Lock();
   
  return(0);
}

int erase_page(uint32_t Addr) {
  static FLASH_EraseInitTypeDef EraseInitStruct;
  uint32_t page_error;
   
  HAL_FLASH_Unlock();
  /* Clear OPTVERR bit set on virgin samples */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_OPTVERR); 
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Banks       = GetBank(Addr);
  EraseInitStruct.Page        = GetPage(Addr);
  EraseInitStruct.NbPages     = 1;
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &page_error) != HAL_OK) {
    HAL_FLASH_Lock();
    return (-1);
  }
  HAL_FLASH_Lock();
  return(0);
}

uint32_t GetPage(uint32_t Addr) {
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

uint32_t GetBank(uint32_t Addr) {
  return FLASH_BANK_1;
}
