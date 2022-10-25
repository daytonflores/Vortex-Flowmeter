/*----------------------------------------------------------------------------
LAB EXERCISE 8.2 - INTERRUPT IN/OUT
PROGRAMMING USING MBED API
 ----------------------------------------
 In this exercise you need to use the mbed API functions to:
 
	1) Define InterruptIn and ISR for the button press
	2) In the interrupt service routine, the LED is used to indicate when a
	button was pressed:
			+ USER_BUTTON - LED1

	3) Put the processor into sleep mode upon exiting from the ISR
			
	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"

//Define outputs

	//Write your code here


//Define interrupt inputs

	//Write your code here


//Define ISRs for the interrupts
void button_1_handler(){
	
	//Write your code here
	
}

//void button_2_handler(){
//	
	////Write your code here
//	
//}
//
//void button_3_handler(){
//	
	////Write your code here
//	
//}
//
//void button_4_handler(){
//	
	////Write your code here
//	
//}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){
	
	__enable_irq();			//enable interrupts
	
	//initially turn off  LED
	//Write your code here
	
	//Interrupt handlers
	//Attach the address of the ISR to the rising edge
	
		//Write your code here
	
	//Sleep on exit
	while(1){
		
		//Write your code here
	
	}
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
