/*----------------------------------------------------------------------------
LAB EXERCISE 11.3 - I2C interface
SERIAL COMMUNICATION
 ----------------------------------------
    Access the virtual temperature sensor via I2C interface, print the current temperature
    to the PC via UART
    
    Input: temperature sensor (DS1631)
    Output: PC
    
    GOOD LUCK!
 *----------------------------------------------------------------------------*/
#include "mbed.h"
#include "pindef.h"

#define I2C_SDA PB_9
#define I2C_SCL PB_8
#define UART_TX PA_2
#define UART_RX PA_3
#define DS1631_CMD_START_CONVERT_T 0x51
#define DS1631_CMD_READ_TEMPERATURE 0xAA

//I2C interface
I2C i2c(I2C_SDA, I2C_SCL);
Serial pc(UART_TX, UART_RX);

//I2C address of temperature sensor DS1631
const int ds1631_addr = 0x90;

char ds1631_cmd[] = {};
char ds1631_data[2];

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){
    while(1){
        /*
        Write the Start Convert T command to the sensor
        Write the Read Temperature command to the sensor
        Read the 16-bit temperature data
        */

        ds1631_cmd[0] = DS1631_CMD_START_CONVERT_T;
        i2c.write(ds1631_addr, ds1631_cmd, 1, 0);
        ds1631_cmd[0] = DS1631_CMD_READ_TEMPERATURE;
        i2c.write(ds1631_addr, ds1631_cmd, 1, 0);
        i2c.read(ds1631_addr, ds1631_data, 2, 0);
        
        //Convert temperature to Celsius
        float temp = (float((ds1631_data[0] << 8) | ds1631_data[1]) / 256);
        
        //Print temperature to the serial monitor

        printf("Temp = %f degrees C\r\n", temp);
        wait(1);
        
        //Write your code here
        
        
    }
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
