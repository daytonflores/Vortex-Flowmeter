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

#define USER_BUTTON1 PB_4
#define USER_BUTTON2 PB_5
#define USER_BUTTON3 PB_3
#define USER_BUTTON4 PA_10

#define LED1 PA_5
#define LED2 PA_13

//Define input bus
BusIn button_bus(USER_BUTTON4, USER_BUTTON3, USER_BUTTON2, USER_BUTTON1);

//Define output bus for the  LED
BusOut led_bus(LED2, LED1);


/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){
    while(1){
        if(button_bus == 0b0001){
            led_bus = 0b01;
        }
        else if(button_bus == 0b0010){
            led_bus = 0b10;
        }
        else if(button_bus == 0b0100){
            led_bus = 0b10;
        }
        else if(button_bus == 0b1000){
            led_bus = 0b01;
        }
        else{
            led_bus = 0b00;
        }
    }
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
