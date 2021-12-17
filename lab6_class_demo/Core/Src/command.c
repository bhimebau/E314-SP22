/* command.c
 * Description: 
 *
 * Author: Bryce Himebaugh 
 * Contact: bhimebau@indiana.edu
 * Date: 04.25.2019
 * Copyright (C) 2019
 *
 */

#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "command.h"
#include "queue.h"
#include "interrupt.h"

/* extern UART_HandleTypeDef huart1; */
/* extern RTC_HandleTypeDef hrtc; */
extern queue_t rx_queue;
extern RTC_HandleTypeDef hrtc;

void prompt(void) {
  RTC_TimeTypeDef current_time;
  RTC_DateTypeDef current_date;
  HAL_RTC_GetTime(&hrtc,&current_time,RTC_FORMAT_BIN);
  HAL_RTC_GetDate(&hrtc,&current_date,RTC_FORMAT_BIN);
  printf("%02d/%02d/20%02d ",current_date.Month,current_date.Date,current_date.Year);
  printf("%02d:%02d:%02d ",current_time.Hours,current_time.Minutes, current_time.Seconds);
  printf("IULS> ");
}
void help_command(char *);
void lof_command(char *);
void lon_command(char *);
void uid_command(char *);
void test_command(char *);
void ts_command(char *);
void ds_command(char *);

void temp_command(char *);
void batt_command(char *);
void tsl237_command(char *);
void ef_command(char *);
void sample_command(char *);
void data_command(char *);
void time_command(char *);
void flash_info_command(char *arguments);


command_t commands[] = {
  {"help",help_command},
  {"lof",lof_command},
  {"lon",lon_command},
  {"uid",uid_command},
  {"test",test_command},
  {"ts",ts_command},
  {"ds",ds_command},
  {"ls",help_command},
  {"temp",temp_command},
  {"battery",batt_command},
  {"tsl237",tsl237_command},
  {"ef",ef_command},
  {"sample",sample_command},
  {"data",data_command},
  {"time",time_command},
  {"flash_info",flash_info_command},
  {0,0}
};

void __attribute__((weak)) help_command(char *arguments) {
  command_t * p = commands;
  printf("Available Commands:\n\r");
  while (p->cmd_string) {
    printf("%s\n\r",p->cmd_string);
    p++;
  }
  printf("OK\n\r");
}

void __attribute__((weak)) flash_info_command(char *arguments) {
  printf("Flash Information command\n\r");
  printf("OK\n\r");
}


void __attribute__((weak)) time_command(char *arguments) {
  printf("Time Weak command\n\r");
  printf("OK\n\r");
}

void __attribute__((weak)) data_command(char *arguments) {
  printf("Data Weak command\n\r");
  printf("OK\n\r");
}

void __attribute__((weak)) sample_command(char *arguments) {
  printf("Sample Weak command\n\r");
  printf("OK\n\r");
}

void __attribute__((weak)) ef_command(char *arguments) {
  printf("Erase Flash Weak command\n\r");
  printf("OK\n\r");
}

void __attribute__((weak)) temp_command(char *arguments) {
  printf("In weak temp command\n\r");
  printf("OK\n\r");
}

void __attribute__((weak)) batt_command(char *arguments) {
  printf("In weak battery command\n\r");
  printf("OK\n\r");
}

void __attribute__((weak)) tsl237_command(char *arguments) {
  printf("In weak tsl237 command\n\r");
  printf("OK\n\r");
}

void __attribute__((weak)) test_command(char *arguments) {
  printf("In weak test command\n\r");
  printf("OK\n\r");
}


void __attribute__((weak)) lon_command(char *arguments) {
  printf("Led On Default Command\n\r");
  if (arguments) {
    printf("Arguments = %s\n\r",arguments);
  }
}

void __attribute__((weak)) lof_command(char *arguments) {
  printf("Led Off Default Command\n\r");
  if (arguments) {
    printf("Arguments = %s\n\r",arguments);
  }
}

void __attribute__((weak)) uid_command(char *arguments) {
  printf("Unique Processor ID Default Command\n\r");
  if (arguments) {
    printf("Arguments = %s\n\r",arguments);
  }
}

void __attribute__((weak)) ts_command(char *arguments) {
  printf("Set time default function\n\r");
  if (arguments) {
    printf("Arguments = %s\n\r",arguments);
  }
}

void __attribute__((weak)) ds_command(char *arguments) {
  printf("Set date default command\n\r");
  if (arguments) {
    printf("Arguments = %s\n\r",arguments);
  }
}

int execute_command(uint8_t * line) {
  uint8_t *cmd;
  uint8_t *arg;
  command_t *p = commands;
  int success = 0;

  if (!line) {
    return (-1); // Passed a bad pointer 
  }
  if (parse_command(line,&cmd,&arg) == -1) {
    printf("Error with parse command\n\r");
    return (-1);
  }
  while (p->cmd_string) {
    if (!strcmp(p->cmd_string,(char *) cmd)) {
      if (!p->cmd_function) {
        return (-1);
      }
      (*p->cmd_function)((char *)arg);            // Run the command with the passed arguments
      success = 1;
      break;
    }
    p++;
  }
  if (success) {
    return (0);
  }
  else {
    return (-1);
  }
}

int parse_command (uint8_t *line, uint8_t **command, uint8_t **args) {
  // looks for the first comma, places a NULL and captures the remainder as the arguments
  uint8_t *p; 
  
  if((!line) ||
     (!command) ||
     (!args)) {
    return (-1); // Passed a bad pointer 
  }    
  *command = line;
  p = line;
  while (*p!=','){
    if (!*p) {
      *args = '\0';
      return(0);
    }
    p++;
  }
  *p++ = '\0'; // Replace first comma with a null
  *args = p;   // The arguments are right after the comma
  return (0);
}

enum {COLLECT_CHARS, COMPLETE};
  
int get_command(uint8_t *command_buf) {
  static uint32_t counter=0;
  static uint32_t mode = COLLECT_CHARS;
    
  uint8_t ch = 0;;
  uint32_t mask;
  
  ch=dequeue(&rx_queue);
  while (ch!=0) {
    if ((ch!='\n')&&(ch!='\r')) {
      if (ch==0x7f) {               // backspace functionality
        if (counter > 0) { 
            printf("\b \b");
            counter--;
        }
      }
      else {
        putchar(ch); // send the character
        /* while (!LL_LPUART_IsActiveFlag_TXE(LPUART1)); // wait until the character has been sent.       */
        command_buf[counter++]=ch;
        if (counter>=(QUEUE_SIZE-2)) {
          mode=COMPLETE;
          break;
        }
      }
    }
    else {
      mode = COMPLETE;
      break;
    }
    mask = disable();
    ch=dequeue(&rx_queue);
    restore(mask);  
  }
  if (mode == COMPLETE) {
    command_buf[counter] = 0;
    printf("\n\r");
    counter = 0;
    mode = COLLECT_CHARS;
    return(1);
  }
  else {
    return(0);
  }
}
    

int delspace(uint8_t *instr) {
  int length;
  uint8_t temp[MAX_COMMAND_LEN];
  uint8_t *src;
  uint8_t *dest;
  int count = 0;
  
  if (!instr) {
    return (-1); // if passed a null pointer, bail out 
  } 
  if (!(length=strnlen((char *)instr,MAX_COMMAND_LEN))) {
    return (-1); // if the passed string length is 0
  }
  if (length == MAX_COMMAND_LEN) {
    return (-1); // no null was found in the passed string. 
  }
  strcpy((char *)temp, (char *)instr);
  src = temp;                 // point at the beginning of the temp string (copy of instr)
  dest = instr;
  while(isspace(*src)) src++; // Skip leading whitespace
  while (*src!='\0') {        // traverse the source string
    if (!isspace(*src)) {     // if the character is not whitespace, copy it to the dest. 
      *dest = *src;
      dest++;
      count++;
    }
    src++;
  }
  *dest = '\0';
  return (count);
}


