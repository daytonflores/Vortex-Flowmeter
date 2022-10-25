/*----------------------------------------------------------------------------
LAB EXERCISE 11.4- SPI and I2C interface
SERIAL COMMUNICATION
 ----------------------------------------
 Display the temperature from the virtual DS1631 temperature sensor on the 
      virtual LCD
 
 Input: virtual DS1631 temperature sensor
 Output: virtual LCD display
    
    GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "NHD_0216HZ.h"
#include "DS1631.h"
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

//LCD display ST7066U
NHD_0216HZ st7066u(SPI_CS, SPI_MOSI, SPI_SCLK);

//UART
Serial serial_device(UART_TX, UART_RX);

//Define a variable to store temperature measurement
float temp;

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main() {
  st7066u.init_lcd();
    
    //Write your code here
    
    
    while(1){
        /*
        Read the temperature from the DS1631
        Update the LCD with new temperature measurement
        */
        ds1631_cmd[0] = DS1631_CMD_START_CONVERT_T;
        i2c.write(ds1631_addr, ds1631_cmd, 1, 0);
        ds1631_cmd[0] = DS1631_CMD_READ_TEMPERATURE;
        i2c.write(ds1631_addr, ds1631_cmd, 1, 0);
        i2c.read(ds1631_addr, ds1631_data, 2, 0);
        
        //Convert temperature to Celsius
        float temp = (float((ds1631_data[0] << 8) | ds1631_data[1]) / 256);
        st7066u.set_cursor(0, 0);
        st7066u.printf("Temp = %f", temp);
        serial_device.printf("Temp = %f\r\n", temp);
        
        wait(1);
    }
}

// *******************************ARM University Program Copyright (c) ARM Ltd 2014*************************************
