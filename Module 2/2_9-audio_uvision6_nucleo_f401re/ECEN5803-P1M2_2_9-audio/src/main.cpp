/*----------------------------------------------------------------------------
LAB EXERCISE 9 - Analog input and PWM
 ----------------------------------------
    Use two potentiometers to adjust the volume and pitch of the output sound wave.
    
    Inputs: Virtual potentiometers 1 and 2
    Output: Virtual speaker,  Real PC

    GOOD LUCK!
 *----------------------------------------------------------------------------*/

#include "mbed.h"
#include "pindef.h"

#define POTENTIOMETER1 PA_0
#define POTENTIOMETER2 PA_1

#define SPEAKER1 PB_10

#define MIN_POTEN 0.75
#define MAX_POTEN 1.00

/*
 * Goal is to find linear equation y = mx + b for period
 * Start by using 2 known points (MIN_FREQ, MIN_POTEN), (MAX_FREQ, MAX_POTEN) to find slope m, y-intercept b
 */
#define MIN_FREQ 320.0
#define MAX_FREQ 8000.0
#define M_FREQ ((MAX_FREQ - MIN_FREQ)/(MAX_POTEN - MIN_POTEN))
#define B_FREQ (MAX_FREQ - M_FREQ*MAX_POTEN)

/*
 * Goal is to find linear equation y = mx + b for duty cycle
 * Start by using 2 known points (MIN_VOL, MIN_POTEN), (MAX_VOL, MAX_POTEN) to find slope m, y-intercept b
 * Now plug in reading from volume1 potentiometer to find adjusted duty cycle
 */
#define MIN_VOL 0.1
#define MAX_VOL 1.0
#define M_VOL ((MAX_VOL - MIN_VOL)/(MAX_POTEN - MIN_POTEN))
#define B_VOL (MAX_VOL - M_VOL*MAX_POTEN)

/*
Define the PWM speaker output
Define analog inputs
Define serial output
*/
PwmOut speaker1(SPEAKER1);
AnalogIn volume1(POTENTIOMETER1);
AnalogIn pitch1(POTENTIOMETER2);

//Define variables
float i;

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/

int main(){
    while(1){
        /*
        Print values of the potentiometers to the PC serial terminal
        Create a saw-tooth sound wave
        Make the period and volume adjustable using the potentiometers
        */

    /*
     * Now plug in reading from pitch1 potentiometer into y = mx + b to find adjusted period
     */
    speaker1.period(1/((pitch1.read()*M_FREQ) + B_FREQ));
    
    /*
     * Now plug in reading from volume1 potentiometer into y = mx + b to find adjusted duty cycle
     * Generate saw-tooth wave from min adjusted volume up to volume1's adjusted volume
     * Using tiny increments of i will allow for finer tuning of each saw-tooth (smoother sound)
     */
    for(i = MIN_VOL*M_VOL + B_VOL; i < volume1.read()*M_VOL + B_VOL; i += 0.0005){
        speaker1 = i;
    }

    /*
     * Print readings to serial port
     * Then drop duty cycle back down to stay true to saw-tooth wave
     * Then wait before taking readings
     */
    printf("Volume = %f%%, Pitch = %f Hz\n", (volume1.read()*M_VOL + B_VOL)*100, (pitch1.read()*M_FREQ + B_FREQ));
    speaker1 = MIN_VOL*M_VOL + B_VOL;
    wait(0.25);

    }
}

// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************
