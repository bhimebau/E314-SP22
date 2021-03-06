/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Tue Feb 28 08:55:30 2006
 * Version: 
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */

/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/* Code: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ueac.h"
#include "interpreter.h"
#include "lla.h"

char QUIT[]="Q\n";
char command[200];
ueac_t ueac_state;
char *strptr = NULL;

int main(void) {
  while(1) {
    strptr = command;
    while ((*strptr++=getchar())!='\n');                    // grab the chars until a newline is found    
    *strptr='\0';                                           // stuff a null  
    if (!strncasecmp(command,QUIT,sizeof(QUIT))) break;
    interpreter(command,&ueac_state);
  }
  lla_print_active(&ueac_state);
  printf("done\n");
  return 0; 
}

/* main.c ends here */
