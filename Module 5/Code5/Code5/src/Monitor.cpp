/**----------------------------------------------------------------------------
             \file Monitor.cpp
--                                                                           --
--              ECEN 5803 Mastering Embedded System Architecture             --
--                  Project 1 Module 5                                      --
--                Microcontroller Firmware                                   --
--                      Monitor.cpp                                            --
--                                                                           --
-------------------------------------------------------------------------------
--
--  Designed for:  University of Colorado at Boulder
--               
--                
--  Designed by:  Tim Scherr
--  Revised by:  Mark Sherman 
-- 
-- Version: 2.0
-- Date of current revision:  2022-07-09  
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

#include "pindef.h"
#include "DS1631.h"

#define TEMP_ADDR		0x90
DS1631 temp_sensor(I2C_SDA, I2C_SCL, TEMP_ADDR);
Serial pc(UART_TX, UART_RX);

const int NUM_OF_REG 	= 16;

double temp;
double freq;
double velocity	= 1;
double flow;
int vel_cnt	= 0;

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
	pc.baud(9600);
  pc.printf("\r\n\nDebug Monitor\n\r");
	pc.printf("\rAuthor: Mark Sherman\n\r");
  pc.printf("\r\nSelect Mode");
  pc.printf("\r\n\tNOR\t- Normal");
  pc.printf("\r\n\tQUI\t- Quiet");
  pc.printf("\r\n\tDEB\t- Debug" );
	pc.printf("\r\n\tREG\t- Register View (R0-R15)");
	pc.printf("\r\n\tMEM\t- Memory Viewer");
	pc.printf("\r\n\tVER\t- Version Info\r\n");
  pc.printf("\r\n->");
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
                  (msg_buf[0] != 'V') && (msg_buf[0] != 'R') &&
									(msg_buf[0] != 'M') &&
                  (msg_buf_idx != 0))
         {                          // if first character is bad in Quiet mode
            msg_buf_idx = 0;        // then start over
         }
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
	get_temp();

   if( (chr = msg_buf[0]) <= 0x60 ) 
   {      // Upper Case
      switch( chr ) 
      {
         case 'D':
            if((msg_buf[1] == 'E') && (msg_buf[2] == 'B') && (msg_buf_idx == 3)) 
            {
               display_mode = DEBUG;
               UART_msg_put("\r\nMode=DEBUG\n");
               display_timer = 0;
            }
            else
               err = 1;
            break;

         case 'N':
            if((msg_buf[1] == 'O') && (msg_buf[2] == 'R') && (msg_buf_idx == 3)) 
            {
               display_mode = NORMAL;
               UART_msg_put("\r\nMode=NORMAL\n");
               display_timer = 0;
            }
            else
               err = 1;
            break;

         case 'Q':
            if((msg_buf[1] == 'U') && (msg_buf[2] == 'I') && (msg_buf_idx == 3)) 
            {
               display_mode = QUIET;
               UART_msg_put("\r\nMode=QUIET\n");
               display_timer = 0;
            }
            else
               err = 1;
            break;

         case 'V':
					 if((msg_buf[1] == 'E') && (msg_buf[2] == 'R') && (msg_buf_idx == 3)) 
           {
							display_mode = VERSION;
							pc.printf("\r\n\n-------------------------");
							pc.printf("\r\n|Version Info:\t\t|\r\n|\t");
							pc.printf( CODE_VERSION ); 
							pc.printf("\t|");
							pc.printf("\r\n-------------------------\n");
						 	pc.printf("\r\n->");
							display_timer = 0;
					 }
            break;
/****************      ECEN 5803 add code as indicated   **********************/
//  Add other message types here
         case 'R':
            if((msg_buf[1] == 'E') && (msg_buf[2] == 'G') && (msg_buf_idx == 3)) 
            {
               display_mode = REG_VIEW;
							pc.printf("\r\nMode=REG_VIEW\n");
               //UART_msg_put("\r\nMode=REG_VIEW\n");
               display_timer = 0;
            }
            else
               err = 1;
            break;
						
				 case 'M':
            if((msg_buf[1] == 'E') && (msg_buf[2] == 'M') && (msg_buf_idx == 3)) 
            {
               display_mode = MEM_VIEW;
							pc.printf("\r\nMode=MEM_VIEW\n");
               //UART_msg_put("\r\nMode=MEM_VIEW\n");
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
      UART_msg_put("\n\rEntry Error!");
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
							pc.printf("\r\n\n-------------------------");
							pc.printf("\r\n| MODE = NORMAL\t\t|");
							pc.printf("\r\n-------------------------");
              pc.printf("\r\nFlow: %f", flow);
              pc.printf("\r\nTemp: %f", temp);
              pc.printf("\r\nFreq: %f", freq);
							pc.printf("\r\n->");
              display_flag = 0;
            }
         }
         break;
      case(DEBUG):
         {
            if (display_flag == 1)
            {
							pc.printf("\r\n\n-------------------------");
							pc.printf("\r\n| MODE = DEBUG\t\t|");
							pc.printf("\r\n-------------------------");
              pc.printf("\r\nFlow: %f", flow);
              pc.printf("\r\nTemp: %f", temp);
              pc.printf("\r\nFreq: %f", freq);
							pc.printf("\r\n->");
               // clear flag to ISR      
              display_flag = 0;
             }   
         }
				 break;
/**
///     ARM Register View
///			Reads value from Registers 0-15 and outputs the 2 LSB via pc.printf;
*/
			case(REG_VIEW):
			{
				if (display_flag == 1)
				{
					register int R0 	__asm("r0");
					register int R1 	__asm("r1");
					register int R2 	__asm("r2");
					register int R3 	__asm("r3");
					register int R4 	__asm("r4");
					register int R5 	__asm("r5");
					register int R6 	__asm("r6");
					register int R7 	__asm("r7");
					register int R8 	__asm("r8");
					register int R9 	__asm("r9");
					register int R10	__asm("r10");
					register int R11	__asm("r11");
					register int R12	__asm("r12");
					register int R13	__asm("r13");
					register int R14	__asm("r14");
					register int R15	__asm("r15");
					
					int reg_values[NUM_OF_REG] = 
											{ 
												R0, R1, R2, R3, R4, R5, R6,
												R7, R8, R9, R10, R11, R12,
												R13, R14, R15
											};						
						
					pc.printf("\r\n\n-------------------------");
					pc.printf("\r\n| MODE = REG_VIEW\t|");
					pc.printf("\r\n-------------------------");
					
					for(int i = 0; i < NUM_OF_REG; i++)
					{
						pc.printf("\r\nReg %d", i);
						pc.printf(":\t0x%x", reg_values[i]);
					}
					
					pc.printf("\r\n->");		
					display_flag = 0;							
				}
			}				 
      break;
/*******************************************************************************************
    \fn Memory View Function
		\brief Memory view case statement. Outputs the starting memory address of the program
********************************************************************************************/			
			case(MEM_VIEW):
			{
				if (display_flag == 1)
				{
					register int R13	__asm("r13");
					int *stackPtr = (int*)R13;
					
					pc.printf("\r\n\n-------------------------");
					pc.printf("\r\n| MODE = MEM_VIEW\t|");
					pc.printf("\r\n-------------------------\r\n");
					pc.printf("\rR13 Stack Pointer = 0x%x", R13);
					for(int i = 0; i < 16; i++)
					{
						pc.printf("\r\nWord %d: %x", i, *stackPtr++);
					}
					pc.printf("\r\n->");
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

//*****************************************************************************/
///	\fn double get_Freq(uint16_t* samples) 
///	\param uint16_t* samples - pointer to array of samples
///	\brief - calculates frequency from 1000 samples using peak detection	
///						the difference between 2 samples is evaluated for +/-
///						positive = moving down the curve, i.e. sample1 is > sample2
///						negative = moving up the curve, i.e. sample1 is < sample2
///						when a difference in dir is found, we have crossed a peak / trough
///						when the peak is found, the index (sample #) is stored
///						difference between the two peaks sample # is the taken as the period
///						divide period by sample rate to get signal frequency
///	\return - double, calculated frequency in Hz
//*****************************************************************************/
double get_Freq(uint16_t* samples) 
{
	double sample_period	= .0001;//sample frequency
	int dir 					= 0;
	int dir_old 			= 0;
	int peak1					=	0;	
	int peak2					= 0;
	
	for(int i = 0; i < SMP_CNT - 1; i++)
	{
		dir_old = dir;
		
		if(samples[i] - samples[i+1] < 0)
		{//sample value is increasing, moving up the curve
			dir = 1; //moving up
		}
		else
			dir = 0;
		
		if(peak1 !=0 && peak2 !=0 && i > 0)
				break;
		
		else if(peak1 == 0 && dir != dir_old && i > 0)//not the same, we found a peak/trough
			peak1 = i;//save peak or trough index (sample #)
		
		else if(peak1 !=0 && dir != dir_old && i > 0)
			peak2 = i;//save second peak or trough index
	}
	
	freq = 1 / (2*(peak2 - peak1) * sample_period);
	return freq;
}

//*****************************************************************************/
///	\fn double get_Density() 
///	\brief - calculates density using measured temperature
///	\return - double, density, in kg/(m^3)		
//*****************************************************************************/
double get_Density()
{
	return 1000*(1 - ((temp+288.9414)/(508929.2*(temp+68.12963)))*(pow(temp-3.9863,2)));
}

//*****************************************************************************/
/// \fn double get_Viscosity() 
/// \brief - calculates viscosity using measured temperature
///	\return double - viscosity in kg/(m^3)
//*****************************************************************************/
double get_Viscosity()
{
	return .000024 * pow(10,(247.8/((temp+273.15)-140.0)));
}

//*****************************************************************************/
///	\fn double get_Re() 
///	\brief - calculates Re number using the calulated Density and Viscosity
///	\return - double, Reynolds number, unitless
//*****************************************************************************/
double get_Re()
{
	return (get_Density() * (.0254*velocity) * PID_IN) / get_Viscosity();
}

//*****************************************************************************/
///	\fn double get_St() 
///	\brief - calculates St number using the calulated Reynolds Number	
///	\return - double, St number, unitless
//*****************************************************************************/
double get_St()
{
	return 0.2684 - (1.0356/sqrt(get_Re()));
}

//*****************************************************************************/
/// \fn double get_Vel() 
/// \brief - calculates velocity from calculated frequency and St number
///						equation is iterative
///						requires ~3 runs for velocity to converge
/// \return - double, velocity in in/s
//*****************************************************************************/
void get_Vel()
{
	while(vel_cnt < 3)
	{
		velocity 	= (freq*BLUFF_BODY_IN)/(get_St());
		vel_cnt++;
	}
}

//*****************************************************************************/
/// \fn double get_Flow() 
/// \brief - calculates flow using calculated velocity
///	\return - double, flow rate in gallons/min
//*****************************************************************************/
double get_Flow()
{
	//get_Vel();
	flow = (2.45 * pow(PID_IN, 2) * (60*velocity)) / 231;
	return flow;
}

//*****************************************************************************/
/// \fn void get_temp(void) 
/// \brief - reads temp from I2C sensor, sets global temp variable
//*****************************************************************************/
void get_temp(void)
{
	temp = temp_sensor.read();
}
