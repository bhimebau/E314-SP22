/* external_flash.h --- 
 * 
 * Filename: external_flash.h
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Fri Feb 10 11:19:00 2006
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

int flash_integrity_test(void);
unsigned char send_spi_byte(unsigned char);                
void init_spi(void);                                       
void start_main_array_read(unsigned short,unsigned short); 
void end_main_array_read(void);                            
void page_to_buffer1_compare(unsigned short);              
void buffer1_to_page_e(unsigned short);                    
void buffer2_to_page_e(unsigned short);                    
unsigned char poll_status_register_blocking(void);         
void buffer1_write(unsigned short, unsigned char);         
void buffer2_write(unsigned short, unsigned char);         
unsigned char buffer1_read(unsigned short);                
unsigned char buffer2_read(unsigned short);                

#define nCS_LOW {P4OUT &= ~BIT3;}
#define nCS_HIGH {P4OUT |= BIT3;}
#define PAGE_SIZE 264
#define NUMBER_PAGES 1024

/* external_flash.h ends here */
