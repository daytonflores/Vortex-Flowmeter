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

#define USER_BUTTON1 PB_4
#define USER_BUTTON2 PB_5
#define USER_BUTTON3 PB_3
#define USER_BUTTON4 PA_10

#define LED1 PA_5
#define LED2 PA_13

//Define outputs
BusOut led_bus(LED2, LED1);

//Define interrupt inputs
InterruptIn button_press1(USER_BUTTON1);
InterruptIn button_press2(USER_BUTTON2);
InterruptIn button_press3(USER_BUTTON3);
InterruptIn button_press4(USER_BUTTON4);


//Define ISRs for the interrupts
void button_1_handler(){
    led_bus = 0b01;
}

void button_2_handler(){
    led_bus = 0b00;
}

void button_3_handler(){
    led_bus = 0b10;
}

void button_4_handler(){
    led_bus = 0b00;
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){
    
    __enable_irq();         //enable interrupts
    
    //initially turn off  LED
    led_bus = 0b00;
    
    //Interrupt handlers
    //Attach the address of the ISR to the rising edge
    button_press1.rise(&button_1_handler);
    button_press2.rise(&button_2_handler);
    button_press3.rise(&button_3_handler);
    button_press4.rise(&button_4_handler);
    
    //Sleep on exit
    __wfi();
    
    while(1){
        
        //Write your code here
    
    }
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
