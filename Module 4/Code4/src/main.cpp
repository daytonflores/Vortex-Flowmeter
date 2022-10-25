/**----------------------------------------------------------------------------
 
   \file main.cpp

--                                                                           --
--              ECEN 5803 Mastering Embedded System Architecture             --
--                  Project 1 Module 4                                       --
--                Microcontroller Firmware                                   --
--                      main.cpp                                            --
--                                                                           --
-------------------------------------------------------------------------------
--
--  Designed for:  University of Colorado at Boulder
--               
--                
--  Designed by:  Tim Scherr
--  Revised by:  Student's name 
-- 
-- Version: 3.0
-- Date of current revision:  2022-06-20  
-- Target Microcontroller: ST STM32F401RE 
-- Tools used:  ARM mbed compiler
--              ARM mbed SDK
--              ST Nucleo STM32F401RE Board
--               
-- 
-- Functional Description:  Main code file generated by mbed, and then
--                           modified to implement a super loop bare metal OS.
--
--      Copyright (c) 2015, 2016, 2022 Tim Scherr  All rights reserved.
--
*/

#define MAIN
#include "shared.h"
#undef MAIN

#include "NHD_0216HZ.h"
#include "DS1631.h"
#include "pindef.h"

#define LD2 PA_5
#define BAUD_RATE 921600

//DigitalOut greenLED(LED1);

extern volatile uint16_t SwTimerIsrCounter; 

Ticker tick;          //  Creates a timer interrupt using mbed methods

 /****************      ECEN 5803 add code as indicated   ***************/
   // Add code to control LED LD2 here, 
   // including a function to flip the LED state on and off
DigitalOut led_onboard(LD2);


//Serial pc(USBTX, USBRX);
Serial pc(UART_TX, UART_RX);
 
void led_toggle(){
	led_onboard = !led_onboard;
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main() 
{
/****************      ECEN 5803 add code as indicated   **********************/
               //  Add code to call timer0 function every 100 uS
	  pc.baud(BAUD_RATE);
		led_onboard.write(0);
		tick.attach_us(&timer0, 100);
	
 // Print the initial banner
    pc.printf("\r\nHello World!\n\n\r");
    uint32_t  count = 0;   


// initialize serial buffer pointers
   rx_in_ptr =  rx_buf; /* pointer to the receive in data */
   rx_out_ptr = rx_buf; /* pointer to the receive out data*/
   tx_in_ptr =  tx_buf; /* pointer to the transmit in data*/
   tx_out_ptr = tx_buf; /* pointer to the transmit out */	
   

/****************      ECEN 5803 add code as indicated   **********************/
    // uncomment this section after adding monitor code.  
   /* send a starting message to the terminal  */                    

   UART_direct_msg_put("\r\nSystem Reset\r\nCode ver. ");
   UART_direct_msg_put( CODE_VERSION );
   UART_direct_msg_put("\r\n");
   UART_direct_msg_put( COPYRIGHT );
   UART_direct_msg_put("\r\n");

   set_display_mode();                                      
   chk_UART_msg();
   	
    while(1)       /// Cyclical Executive Loop
    {

        count++;               // counts the number of times through the loop
//      __enable_interrupts();
//      __clear_watchdog_timer();
				__enable_irq();

/****************      ECEN 5803 add code as indicated   **********************/
    // uncomment this section after adding monitor code. 
  
        serial();            // Polls the serial port
        chk_UART_msg();     // checks for a serial port message received
        monitor();           // Sends serial port output messages depending
                         //     on commands received and display mode
 
        if ((SwTimerIsrCounter & 0x1FFF) > 0x0FFF)  
/****************      ECEN 5803 add code as indicated   **********************/
         /// Add comment explaining what the timer statement above does
        {
//            flip();  // Toggle Green LED
					led_toggle();
        }


		//Write your code here for any additional tasks
		
		

    }   /// End while(1) loop

}
  