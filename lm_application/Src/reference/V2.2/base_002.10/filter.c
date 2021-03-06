/* filter.c --- 
 * 
 * Filename: filter.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Fri Mar 24 09:38:45 2006
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
#include <stdlib.h>
#include <stdio.h>
#include "filter.h"

unsigned short update_a2d_data (channel_t *channel_struct, unsigned short data) {
  long sum = 0;
  int i;
  channel_struct->raw_result=data;
  channel_struct->channel_data[channel_struct->insertion_point++]=data;
  if (channel_struct->insertion_point>=FILTER_DEPTH) {
    channel_struct->insertion_point=0;
  }
  for (i=0;i<FILTER_DEPTH;i++) {
    sum += channel_struct->channel_data[i];
  }
  sum /= FILTER_DEPTH;
  channel_struct->filtered_result = (unsigned short) sum;
  return (channel_struct->filtered_result);
}

int report_channel_structure(channel_t *channel_struct) {
  int i,j;
  printf("insertion_point=%d\n",channel_struct->insertion_point);
  printf("raw_result=%d\n",channel_struct->raw_result);
  printf("filtered_results=%d\n",channel_struct->filtered_result);
  for (i=0;i<4;i++) {
    for (j=0;j<4;j++) {
      printf("%5d ",channel_struct->channel_data[4*i+j]);
    }
    printf("\n");
  }
  printf("\n");
  return (0);
}

int initialize_channel_structure(channel_t *channel_struct) {
  int i;
  channel_struct->insertion_point=0;
  for (i=0;i<FILTER_DEPTH;i++) {
    channel_struct->channel_data[i]=0;
  }
  channel_struct->raw_result=0;
  channel_struct->filtered_result=0;
  return (0);
}

/* filter.c ends here */
















