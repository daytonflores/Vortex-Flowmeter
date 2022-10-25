/*----------------------------------------------------------------------------
LAB EXERCISE 8.1 - DIGITAL INPUTS AND OUTPUTS
PROGRAMMING USING MBED API
 ----------------------------------------
 In this exercise you need to use the mbed API functions to:
 
	1) Define BusIn, BusOut interfaces for inputs and outputs
	2) The LED is controlled by the button:
			+ USER_BUTTON - LED1
			
	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"

//Define input bus
	//Write your code here
DigitalIn button_press1(PA_3);

//Define output bus for the  LED
	//Write your code here
DigitalOut led1(PA_13);

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){
	led1 = 0;
	
	while(1){
		
		//Check which button was pressed and light up the corresponding LED
		//Write your code here
		if(button_press1){
			led1 = ~led1;
		}
	}
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
