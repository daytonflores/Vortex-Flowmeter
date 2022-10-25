/*----------------------------------------------------------------------------
LAB EXERCISE 12 - Real-Time Operating System
 ----------------------------------------
	Integrate functions developed in previous modules and run then concurrently
	in the mbed RTOS. The following four threads have to be implemented:
	
	1. Display the temperature on the LCD and to the PC
	2. Adjust the brightness of the RGB LED using a potentiometer
	3. Display an incrementing counter on the LCD and to the PC
	4. Blink an LED
  
  NOTE:  For this lab, the LCD, temp sensor and POT are virtual devices.

	GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include "rtos.h"
#include "DS1631.h"
#include "NHD_0216HZ.h"
#include "pindef.h"

#define DS1631_ADDR 0x90
#define ANALOG_IN1 PA_0
#define LD2 PA_5
#define PWM1 PB_10
#define MAX_COUNT 5

/*
Define the mutex
Define the LCD display and the temperature sensor
Define other inputs and outputs
*/

Mutex lcd_mutex;
Mutex uart_mutex;
NHD_0216HZ lcd(SPI_CS, SPI_MOSI, SPI_SCLK);
DS1631 temp_sensor(I2C_SDA, I2C_SCL, DS1631_ADDR);

AnalogIn potentiometer(ANALOG_IN1);
DigitalOut led_onboard(LD2);
PwmOut led_external(PWM1);

float temperature_value = 0;
float potentiometer_value = 0;
int i;

//Display temperature on the LCD
void temp_thread(void const *args){
	while(1){
		lcd_mutex.lock();
		lcd.set_cursor(0, 0);
		temperature_value = temp_sensor.read();
		lcd.printf("Temperature (C)");
		lcd.set_cursor(0, 1);
		lcd.printf("%f", temperature_value);
		Thread::wait(2000);
		lcd_mutex.unlock();
	}
}

//Adjust the brightness of the RGB LED
void adjust_brightness(void const *args){
	while(1){
		potentiometer_value = potentiometer.read();
		led_external.write(potentiometer_value);
		Thread::wait(100);
	}
}

//Blink an LED
void led1_thread(void const *args){
	while(1){
    led_onboard.write(!led_onboard);
		Thread::wait(100);
	}
}

//Display a counter on the LCD
void count_thread(void const *args){
	while(1){
		lcd_mutex.lock();
		lcd.clr_lcd();
		lcd.set_cursor(0, 0);
		lcd.printf("Count");
		for(i = 1; i <= MAX_COUNT; i++){
			lcd.set_cursor(0, 1);
			lcd.printf("%d out of %d", i, MAX_COUNT);
			wait_ms(1000.0/MAX_COUNT);
		}
		Thread::wait(1000);
	  lcd_mutex.unlock();
	}
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

osThreadDef(temp_thread, osPriorityNormal, DEFAULT_STACK_SIZE);
osThreadDef(adjust_brightness, osPriorityNormal, DEFAULT_STACK_SIZE);
osThreadDef(led1_thread, osPriorityNormal, DEFAULT_STACK_SIZE);
osThreadDef(count_thread, osPriorityNormal, DEFAULT_STACK_SIZE);

int main(){
	/*
	Initialise and clear the LCD display
	Start all threads
	Wait for timer interrupt
	*/
	led_onboard.write(0);
	led_external.write(0);
	lcd.init_lcd();
	lcd.clr_lcd();

	osThreadCreate(osThread(temp_thread), NULL);
	osThreadCreate(osThread(adjust_brightness), NULL);
	osThreadCreate(osThread(led1_thread), NULL);
	osThreadCreate(osThread(count_thread), NULL);
	
	while(1){
		Thread::wait(1000);
	}
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
