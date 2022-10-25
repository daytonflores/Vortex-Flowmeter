/**----------------------------------------------------------------------------
             \file Monitor.cpp
--                                                                           --
--              ECEN 5003 Mastering Embedded System Architecture             --
--                  Project 1 Module 4                                       --
--                Microcontroller Firmware                                   --
--                      Monitor.cpp                                            --
--                                                                           --
-------------------------------------------------------------------------------
--
--  Designed for:  University of Colorado at Boulder
--               
--                
--  Designed by:  Tim Scherr
--  Revised by:  Student's name 
-- 
-- Version: 2.0
-- Date of current revision:  2022-06-20   
-- Target Microcontroller: ST STM32F401RE 
-- Tools used:  ARM mbed compiler
--              ARM mbed SDK
--              ST Nucleo STM32F401RE Board
--               
-- 
   Functional Description: See below 
--
--      Copyright (c) 2015, 2022 Tim Scherr All rights reserved.
--
*/              

#include <stdio.h>
#include "shared.h"

#define LOWER_MEM_ADDRESS 0x00000000
#define UPPER_MEM_ADDRESS 0x00000100

uchar8_t is_hex(uchar8_t);

/*******************************************************************************
* Set Display Mode Function
* Function determines the correct display mode.  The 3 display modes operate as 
*   follows:
*
*  NORMAL MODE       Outputs only mode and state information changes   
*                     and calculated outputs
*
*  QUIET MODE        No Outputs
*
*  DEBUG MODE        Outputs mode and state information, error counts,
*                    register displays, sensor states, and calculated output
*                  (currently not all features are operation, could be enhanced)
*
* There is deliberate delay in switching between modes to allow the RS-232 cable 
* to be plugged into the header without causing problems. 
*******************************************************************************/

void set_display_mode(void)   
{
  UART_direct_msg_put("\r\nSelect Mode of Operation");
  UART_direct_msg_put("\r\n Hit NOR - Normal operation mode which continuously displays mode + state info changes + calculated outputs");
  UART_direct_msg_put("\r\n Hit QUI - Quiet operation mode which doesn't display anything");
  UART_direct_msg_put("\r\n Hit DEB - Debug operation mode which continuously displays mode + state info changes + calculated outputs" );
  UART_direct_msg_put("\r\n Hit V - Version# info displayed");
  UART_direct_msg_put("\r\n Hit REG - Registers (r0-r15) values displayed");
  UART_direct_msg_put("\r\n Hit MEM - Memory block displayed");
  UART_direct_msg_put("\r\nSelect:  ");
  
}


//*****************************************************************************/
/// \fn void chk_UART_msg(void) 
///
///  \brief - fills a message buffer until return is encountered, then calls
///           message processing
//*****************************************************************************/
/****************      ECEN 5803 add code as indicated   **********************/
  // Improve behavior of this function
void chk_UART_msg(void)    
{
   uchar8_t j;
   while( UART_input() )      // becomes true only when a byte has been received
   {                                    // skip if no characters pending
      j = UART_get();                 // get next character

      if( j == '\r' )          // on a enter (return) key press
      {                // complete message (all messages end in carriage return)
         UART_msg_put("->");
         UART_msg_process();
      }
      else 
      {
         if ((j != 0x02) )         // if not ^B
         {                             // if not command, then   
            UART_put(j);              // echo the character   
         }
         else
         {
           ;
         }
         if( j == '\b' ) 
         {                             // backspace editor
            if( msg_buf_idx != 0) 
            {                       // if not 1st character then destructive 
               UART_msg_put(" \b");// backspace
               msg_buf_idx--;
            }
         }
         else if( msg_buf_idx >= MSG_BUF_SIZE )  
         {                                // check message length too large
            UART_msg_put("\r\nToo Long!");
            msg_buf_idx = 0;
         }
         else if ((display_mode == QUIET) && (msg_buf[0] != 0x02) && 
                  (msg_buf[0] != 'D') && (msg_buf[0] != 'N') && 
                  (msg_buf[0] != 'V') &&
									(msg_buf[0] != 'R') && (msg_buf[0] != 'M') &&
                  (msg_buf_idx != 0))
         {                          // if first character is bad in Quiet mode
            msg_buf_idx = 0;        // then start over
         }
         //else if ((display_mode == MEMORY) && !is_hex(j)) {
         //    UART_msg_put("\r\nInvalid hex value!");
         //    msg_buf_idx = 0;
         //}
         else {                        // not complete message, store character
 
            msg_buf[msg_buf_idx] = j;
            msg_buf_idx++;
            if (msg_buf_idx > 2)
            {
               UART_msg_process();
            }
         }
      }
   }
}

//*****************************************************************************/
///  \fn void UART_msg_process(void) 
///UART Input Message Processing
//*****************************************************************************/
void UART_msg_process(void)
{
   uchar8_t chr,err=0;
//   unsigned char  data;


   if( (chr = msg_buf[0]) <= 0x60 ) 
   {      // Upper Case
      switch( chr ) 
      {
         case 'D':
            if((msg_buf[1] == 'E') && (msg_buf[2] == 'B') && (msg_buf_idx == 3)) 
            {
               display_mode = DEBUG;
               UART_msg_put("\r\n\r\nMode=DEBUG\n");
               display_timer = 0;
            }
            else
               err = 1;
            break;

         case 'N':
            if((msg_buf[1] == 'O') && (msg_buf[2] == 'R') && (msg_buf_idx == 3)) 
            {
               display_mode = NORMAL;
               UART_msg_put("\r\n\r\nMode=NORMAL\n");
               //display_timer = 0;
            }
            else
               err = 1;
            break;

         case 'Q':
            if((msg_buf[1] == 'U') && (msg_buf[2] == 'I') && (msg_buf_idx == 3)) 
            {
               display_mode = QUIET;
               UART_msg_put("\r\n\r\nMode=QUIET\n");
               display_timer = 0;
            }
            else
               err = 1;
            break;

         case 'V':
            display_mode = VERSION;
            UART_msg_put("\r\n");
            UART_msg_put( CODE_VERSION ); 
						UART_msg_put("\r\nSelect:  ");
            display_timer = 0;
            break;
/****************      ECEN 5803 add code as indicated   **********************/
//  Add other message types here
         case 'R':
					  char j[3];
            if((msg_buf[1] == 'E') && (msg_buf[2] == 'G') && (msg_buf_idx == 3)) 
            {
               display_mode = REGISTER;
               UART_msg_put("\r\n\r\nREGISTERS\n");
							 display_timer = 0;
            }
            else
               err = 1;
            break;
         case 'M':
            if((msg_buf[1] == 'E') && (msg_buf[2] == 'M') && (msg_buf_idx == 3)) 
            {
               display_mode = MEMORY;
               UART_msg_put("\r\n\r\nMEMORY\n");
               display_timer = 0;
						}
            
            else
               err = 1;
            break;
               
         default:
            err = 1;
      }
   }

   else 
   {                                 // Lower Case
      switch( chr ) 
      {
        default:
         err = 1;
      }
   }

   if( err == 1 )
   {
      UART_msg_put("\n\rEntry Error! Try again.");
			UART_msg_put("\r\nSelect:  ");
   }   
   else if( err == 2 )
   {
      UART_msg_put("\n\rNot in DEBUG Mode!");
   }   
   else
   {
    msg_buf_idx = 0;          // put index to start of buffer for next message
      ;
   }
   msg_buf_idx = 0;          // put index to start of buffer for next message


}


//*****************************************************************************
///   \fn   is_hex
/// Function takes 
///  @param a single ASCII character and returns 
///  @return 1 if hex digit, 0 otherwise.
///    
//*****************************************************************************
uchar8_t is_hex(uchar8_t c)
{
   if( (((c |= 0x20) >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f'))  )
      return 1;
   return 0;
}

//*****************************************************************************
///   \fn   get_mem_data
/// Function takes 
///  @param a single 32-bit address and returns
///  @return the 32 bits of data stored at that address
///    
//*****************************************************************************
__asm int get_mem_data(int address)
{
	LDR		r0, [r0]
	BX		lr
}

/*******************************************************************************
*   \fn  DEBUG and DIAGNOSTIC Mode UART Operation
*******************************************************************************/
void monitor(void)
{

/**********************************/
/*     Spew outputs               */
/**********************************/

   switch(display_mode)
   {
      case(QUIET):
         {
             UART_msg_put("\r\n ");
             display_flag = 0;
         }  
         break;
      case(VERSION):
         {
             display_flag = 0;
         }  
         break;         
      case(NORMAL):
         {
            if (display_flag == 1)
            {
               UART_msg_put("\r\nNORMAL ");
               UART_msg_put(" Flow: ");
               // ECEN 5803 add code as indicated
               //  add flow data output here, use UART_hex_put or similar for 
               // numbers
               UART_msg_put(" Temp: ");
               //  add flow data output here, use UART_hex_put or similar for 
               // numbers
               UART_msg_put(" Freq: ");
               //  add flow data output here, use UART_hex_put or similar for 
               // numbers
               display_flag = 0;
            }
         }  
         break;
      case(DEBUG):
         {
            if (display_flag == 1)
            {
               UART_msg_put("\r\nDEBUG ");
               UART_msg_put(" Flow: ");
               // ECEN 5803 add code as indicated               
               //  add flow data output here, use UART_hex_put or similar for 
               // numbers
               UART_msg_put(" Temp: ");
               //  add flow data output here, use UART_hex_put or similar for 
               // numbers
               UART_msg_put(" Freq: ");
               //  add flow data output here, use UART_hex_put or similar for 
               // numbers
						}
					}
          break;     
/****************      ECEN 5803 add code as indicated   **********************/          
               //  Create a display of  error counts, sensor states, and
               //  ARM Registers R0-R15
							 // ECEN 5803 add code as indicated
      case(REGISTER):
         {
            if (display_flag == 1)
            {
							 register int r0 __asm("r0");
							 register int r1 __asm("r1");
							 register int r2 __asm("r2");
							 register int r3 __asm("r3");
							 register int r4 __asm("r4");
							 register int r5 __asm("r5");
							 register int r6 __asm("r6");
							 register int r7 __asm("r7");
							 register int r8 __asm("r8");
							 register int r9 __asm("r9");
							 register int r10 __asm("r10");
							 register int r11 __asm("r11");
							 register int r12 __asm("r12");
							 register int r13 __asm("r13");
							 register int r14 __asm("r14");
							 register int r15 __asm("r15");

							 UART_direct_msg_put("\r\n\r\n r0:  0x");
							 UART_direct_hex_put(int_to_byte(r0, 3));
							 UART_direct_hex_put(int_to_byte(r0, 2));
							 UART_direct_hex_put(int_to_byte(r0, 1));
							 UART_direct_hex_put(int_to_byte(r0, 0));
							 UART_direct_msg_put("\r\n r1:  0x");
							 UART_direct_hex_put(int_to_byte(r1, 3));
							 UART_direct_hex_put(int_to_byte(r1, 2));
							 UART_direct_hex_put(int_to_byte(r1, 1));
							 UART_direct_hex_put(int_to_byte(r1, 0));
							 UART_direct_msg_put("\r\n r2:  0x");
							 UART_direct_hex_put(int_to_byte(r2, 3));
							 UART_direct_hex_put(int_to_byte(r2, 2));
							 UART_direct_hex_put(int_to_byte(r2, 1));
							 UART_direct_hex_put(int_to_byte(r2, 0));
							 UART_direct_msg_put("\r\n r3:  0x");
							 UART_direct_hex_put(int_to_byte(r3, 3));
							 UART_direct_hex_put(int_to_byte(r3, 2));
							 UART_direct_hex_put(int_to_byte(r3, 1));
							 UART_direct_hex_put(int_to_byte(r3, 0));
							 UART_direct_msg_put("\r\n r4:  0x");
							 UART_direct_hex_put(int_to_byte(r4, 3));
							 UART_direct_hex_put(int_to_byte(r4, 2));
							 UART_direct_hex_put(int_to_byte(r4, 1));
							 UART_direct_hex_put(int_to_byte(r4, 0));
							 UART_direct_msg_put("\r\n r5:  0x");
							 UART_direct_hex_put(int_to_byte(r5, 3));
							 UART_direct_hex_put(int_to_byte(r5, 2));
							 UART_direct_hex_put(int_to_byte(r5, 1));
							 UART_direct_hex_put(int_to_byte(r5, 0));
							 UART_direct_msg_put("\r\n r6:  0x");
							 UART_direct_hex_put(int_to_byte(r6, 3));
							 UART_direct_hex_put(int_to_byte(r6, 2));
							 UART_direct_hex_put(int_to_byte(r6, 1));
							 UART_direct_hex_put(int_to_byte(r6, 0));
							 UART_direct_msg_put("\r\n r7:  0x");
							 UART_direct_hex_put(int_to_byte(r7, 3));
							 UART_direct_hex_put(int_to_byte(r7, 2));
							 UART_direct_hex_put(int_to_byte(r7, 1));
							 UART_direct_hex_put(int_to_byte(r7, 0));
							 UART_direct_msg_put("\r\n r8:  0x");
							 UART_direct_hex_put(int_to_byte(r8, 3));
							 UART_direct_hex_put(int_to_byte(r8, 2));
							 UART_direct_hex_put(int_to_byte(r8, 1));
							 UART_direct_hex_put(int_to_byte(r8, 0));
							 UART_direct_msg_put("\r\n r9:  0x");
							 UART_direct_hex_put(int_to_byte(r9, 3));
							 UART_direct_hex_put(int_to_byte(r9, 2));
							 UART_direct_hex_put(int_to_byte(r9, 1));
							 UART_direct_hex_put(int_to_byte(r9, 0));
							 UART_direct_msg_put("\r\n r10: 0x");
							 UART_direct_hex_put(int_to_byte(r10, 3));
							 UART_direct_hex_put(int_to_byte(r10, 2));
							 UART_direct_hex_put(int_to_byte(r10, 1));
							 UART_direct_hex_put(int_to_byte(r10, 0));
							 UART_direct_msg_put("\r\n r11: 0x");
							 UART_direct_hex_put(int_to_byte(r11, 3));
							 UART_direct_hex_put(int_to_byte(r11, 2));
							 UART_direct_hex_put(int_to_byte(r11, 1));
							 UART_direct_hex_put(int_to_byte(r11, 0));
							 UART_direct_msg_put("\r\n r12: 0x");
							 UART_direct_hex_put(int_to_byte(r12, 3));
							 UART_direct_hex_put(int_to_byte(r12, 2));
							 UART_direct_hex_put(int_to_byte(r12, 1));
							 UART_direct_hex_put(int_to_byte(r12, 0));
							 UART_direct_msg_put("\r\n r13: 0x");
							 UART_direct_hex_put(int_to_byte(r13, 3));
							 UART_direct_hex_put(int_to_byte(r13, 2));
							 UART_direct_hex_put(int_to_byte(r13, 1));
							 UART_direct_hex_put(int_to_byte(r13, 0));
							 UART_direct_msg_put("\r\n r14: 0x");
							 UART_direct_hex_put(int_to_byte(r14, 3));
							 UART_direct_hex_put(int_to_byte(r14, 2));
							 UART_direct_hex_put(int_to_byte(r14, 1));
							 UART_direct_hex_put(int_to_byte(r14, 0));
							 UART_direct_msg_put("\r\n r15: 0x");
							 UART_direct_hex_put(int_to_byte(r15, 3));
							 UART_direct_hex_put(int_to_byte(r15, 2));
							 UART_direct_hex_put(int_to_byte(r15, 1));
							 UART_direct_hex_put(int_to_byte(r15, 0));

							 UART_direct_msg_put("\r\n\r\nSelect:  ");
							 display_flag = 0;
						}
         }  
         break;
               //  Create a command to read a section of Memory and display it
      case(MEMORY):
         {
            if (display_flag == 1)
            {
							 volatile unsigned int data;
							 UART_direct_msg_put("\r\n");
							 for(unsigned int i = LOWER_MEM_ADDRESS; i <= UPPER_MEM_ADDRESS; i = i + 4){
								 UART_direct_msg_put("\r\n Address 0x");
								 UART_direct_hex_put(int_to_byte(i, 3));
							   UART_direct_hex_put(int_to_byte(i, 2));
							   UART_direct_hex_put(int_to_byte(i, 1));
							   UART_direct_hex_put(int_to_byte(i, 0));
								 UART_direct_msg_put(" -> Data 0x");
								 data = get_mem_data(i);
								 UART_direct_hex_put(int_to_byte(data, 0));
							   UART_direct_hex_put(int_to_byte(data, 1));
							   UART_direct_hex_put(int_to_byte(data, 2));
							   UART_direct_hex_put(int_to_byte(data, 3));
							 }

							 UART_direct_msg_put("\r\n\r\nSelect:  ");
							 display_flag = 0;
						}
         }  
         break;

      default:
      {
         UART_msg_put("Mode Error");
      }  
   }
}  