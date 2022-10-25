/**----------------------------------------------------------------------------
 
   \file main.cpp

--                                                                           --
--              ECEN 5803 Mastering Embedded System Architecture             --
--                  Project 1 Module 5                                      --
--                Microcontroller Firmware                                   --
--                      main.cpp                                            --
--                                                                           --
-------------------------------------------------------------------------------
--
--  Designed for:  University of Colorado at Boulder
--               
--                
--  Designed by:  Tim Scherr
--  Revised by:  Mark Sherman & Dayton Flores 
-- 
-- Version: 3.0
-- Date of current revision:  2022-07-09  
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
#include "pindef.h"
#include "stm32f401xe.h"

//extern volatile uint16_t SwTimerIsrCounter; 

Ticker tick;          //  Creates a timer interrupt using mbed methods

//Define the LCD, temperature sensor, UART
//Serial pc(UART_TX, UART_RX);  
NHD_0216HZ lcd(SPI_CS, SPI_MOSI, SPI_SCLK);

//*****************************************************************************/
/// \fn void blinkLED(void)
///
///  \brief - inverts onboard LED output
//*****************************************************************************/

#define PWM1	PA_8
#define PWM2	PB_10

#define ADC_IN0	PA_0
#define ADC_IN1	PA_1
#define ADC_IN2	PA_4

AnalogIn vref(ADC_IN0);
AnalogIn virt(ADC_IN1);
AnalogIn int_temp(ADC_IN2);

PwmOut current_loop(PWM1);
PwmOut pulse_out(PWM2);

const double MAX_FLOW = 333;
const double MAX_FREQ = 2000;
const double MIN_FLOW = 1.7;
const double MIN_FREQ = 10;
double adc_data = 0;

uint16_t sample[SMP_CNT] = 
	{
		32767,
		50116,
		62202,
		65361,
		58633,
		44060,
		26062,
		10097,
		1009,
		1553,
		11565,
		28007,
		45894,
		59799,
		65505,
		61281,
		48408,
		30792,
		13774,
		2517,
		436,
		8161,
		23349,
		41394,
		56822,
		64954,
		63324,
		52425,
		35564,
		17854,
		4668,
		5,
		5278,
		18890,
		36710,
		53335,
		63721,
		64718,
		56024,
		40276,
		22250,
		7415,
		268,
		2979,
		14725,
		31944,
		49412,
		61831,
		65435,
		59130,
		44830,
		26870,
		10699,
		1221,
		1312,
		10943,
		27194,
		45135,
		59325,
		65459,
		61677,
		49127,
		31614,
		14451,
		2844,
		312,
		7625,
		22563,
		40596,
		56255,
		64790,
		63611,
		53077,
		36383,
		18592,
		5100,
		0,
		4839,
		18148,
		35892,
		52687,
		63441,
		64891,
		56597,
		41075,
		23034,
		7944,
		384,
		2646,
		14044,
		31121,
		48697,
		61442,
		65489,
		59611,
		45591,
		27682,
		11315,
		1454,
		1091,
		10336,
		26384,
		44369,
		58834,
		65393,
		62056,
		49835,
		32438,
		15140,
		3189,
		209,
		7104,
		21784,
		39794,
		55674,
		64605,
		63879,
		53717,
		37200,
		19339,
		5550,
		17,
		4417,
		17416,
		35071,
		52027,
		63142,
		65043,
		57155,
		41869,
		23823,
		8490,
		520,
		2331,
		13374,
		30299,
		47972,
		61034,
		65522,
		60075,
		46345,
		28497,
		11944,
		1707,
		891,
		9743,
		25579,
		43595,
		58327,
		65306,
		62416,
		50533,
		33261,
		15840,
		3552,
		126,
		6600,
		21011,
		38988,
		55078,
		64400,
		64128,
		54344,
		38015,
		20094,
		6018,
		54,
		4013,
		16693,
		34249,
		51355,
		62823,
		65175,
		57697,
		42657,
		24618,
		9050,
		676,
		2036,
		12716,
		29478,
		47238,
		60609,
		65534,
		60521,
		47090,
		29315,
		12586,
		1979,
		710,
		9164,
		24778,
		42814,
		57804,
		65199,
		62757,
		51219,
		34084,
		16550,
		3934,
		64,
		6113,
		20246,
		38177,
		54467,
		64175,
		64357,
		54957,
		38826,
		20858,
		6502,
		112,
		3627,
		15981,
		33426,
		50671,
		62486,
		65286,
		58223,
		43439,
		25418,
		9626,
		853,
		1759,
		12071,
		28660,
		46495,
		60166,
		65526,
		60950,
		47826,
		30135,
		13241,
		2270,
		549,
		8601,
		23982,
		42027,
		57264,
		65071,
		63079,
		51894,
		34907,
		17271,
		4335,
		23,
		5643,
		19489,
		37364,
		53844,
		63931,
		64566,
		55556,
		39633,
		21629,
		7002,
		191,
		3260,
		15279,
		32602,
		49976,
		62129,
		65377,
		58734,
		44215,
		26223,
		10216,
		1050,
		1503,
		11439,
		27844,
		45743,
		59705,
		65497,
		61362,
		48553,
		30956,
		13909,
		2581,
		409,
		8052,
		23191,
		41235,
		56710,
		64923,
		63383,
		52556,
		35728,
		18001,
		4753,
		2,
		5189,
		18741,
		36547,
		53206,
		63666,
		64754,
		56140,
		40436,
		22407,
		7519,
		290,
		2911,
		14588,
		31779,
		49269,
		61755,
		65448,
		59228,
		44983,
		27032,
		10821,
		1266,
		1266,
		10821,
		27032,
		44983,
		59228,
		65448,
		61755,
		49269,
		31779,
		14588,
		2911,
		290,
		7519,
		22407,
		40436,
		56140,
		64754,
		63666,
		53206,
		36547,
		18741,
		5189,
		2,
		4753,
		18001,
		35728,
		52556,
		63383,
		64923,
		56710,
		41235,
		23191,
		8052,
		409,
		2581,
		13909,
		30956,
		48553,
		61362,
		65497,
		59705,
		45743,
		27844,
		11439,
		1503,
		1050,
		10216,
		26223,
		44215,
		58734,
		65377,
		62129,
		49976,
		32602,
		15279,
		3260,
		191,
		7002,
		21629,
		39633,
		55556,
		64566,
		63931,
		53844,
		37364,
		19489,
		5643,
		23,
		4335,
		17271,
		34907,
		51894,
		63079,
		65071,
		57264,
		42027,
		23982,
		8601,
		549,
		2270,
		13241,
		30135,
		47826,
		60950,
		65526,
		60166,
		46495,
		28660,
		12071,
		1759,
		853,
		9626,
		25418,
		43439,
		58223,
		65286,
		62486,
		50671,
		33426,
		15981,
		3627,
		112,
		6502,
		20858,
		38826,
		54957,
		64357,
		64175,
		54467,
		38177,
		20246,
		6113,
		64,
		3934,
		16550,
		34084,
		51219,
		62757,
		65199,
		57804,
		42814,
		24778,
		9164,
		710,
		1979,
		12586,
		29315,
		47090,
		60521,
		65534,
		60609,
		47238,
		29478,
		12716,
		2036,
		676,
		9050,
		24618,
		42657,
		57697,
		65175,
		62823,
		51355,
		34249,
		16693,
		4013,
		54,
		6018,
		20094,
		38015,
		54344,
		64128,
		64400,
		55078,
		38988,
		21011,
		6600,
		126,
		3552,
		15840,
		33261,
		50533,
		62416,
		65306,
		58327,
		43595,
		25579,
		9743,
		891,
		1707,
		11944,
		28497,
		46345,
		60075,
		65522,
		61034,
		47972,
		30299,
		13374,
		2331,
		520,
		8490,
		23823,
		41869,
		57155,
		65043,
		63142,
		52027,
		35071,
		17416,
		4417,
		17,
		5550,
		19339,
		37200,
		53717,
		63879,
		64605,
		55674,
		39794,
		21784,
		7104,
		209,
		3189,
		15140,
		32438,
		49835,
		62056,
		65393,
		58834,
		44369,
		26384,
		10336,
		1091,
		1454,
		11315,
		27682,
		45591,
		59611,
		65489,
		61442,
		48697,
		31121,
		14044,
		2646,
		384,
		7944,
		23034,
		41075,
		56597,
		64891,
		63441,
		52687,
		35892,
		18148,
		4839,
		0,
		5100,
		18592,
		36383,
		53077,
		63611,
		64790,
		56255,
		40596,
		22563,
		7625,
		312,
		2844,
		14451,
		31614,
		49127,
		61677,
		65459,
		59325,
		45135,
		27194,
		10943,
		1312,
		1221,
		10699,
		26870,
		44830,
		59130,
		65435,
		61831,
		49412,
		31944,
		14725,
		2979,
		268,
		7415,
		22250,
		40276,
		56024,
		64718,
		63721,
		53335,
		36710,
		18890,
		5278,
		5,
		4668,
		17854,
		35564,
		52425,
		63324,
		64954,
		56822,
		41394,
		23349,
		8161,
		436,
		2517,
		13774,
		30792,
		48408,
		61281,
		65505,
		59799,
		45894,
		28007,
		11565,
		1553,
		1009,
		10097,
		26062,
		44060,
		58633,
		65361,
		62202,
		50116,
		32767,
		15418,
		3332,
		173,
		6901,
		21474,
		39472,
		55437,
		64525,
		63981,
		53969,
		37527,
		19640,
		5735,
		29,
		4253,
		17126,
		34742,
		51760,
		63017,
		65098,
		57373,
		42185,
		24140,
		8712,
		580,
		2210,
		13109,
		29970,
		47680,
		60866,
		65529,
		60256,
		46644,
		28824,
		12199,
		1813,
		816,
		9510,
		25258,
		43284,
		58119,
		65266,
		62555,
		50809,
		33590,
		16122,
		3703,
		99,
		6404,
		20704,
		38664,
		54835,
		64313,
		64222,
		54591,
		38340,
		20399,
		6209,
		75,
		3857,
		16407,
		33920,
		51083,
		62690,
		65222,
		57909,
		42971,
		24938,
		9279,
		744,
		1923,
		12457,
		29151,
		46942,
		60434,
		65534,
		60695,
		47386,
		29642,
		12847,
		2093,
		643,
		8937,
		24459,
		42500,
		57590,
		65150,
		62888,
		51490,
		34413,
		16837,
		4092,
		45,
		5923,
		19943,
		37852,
		54219,
		64080,
		64443,
		55198,
		39150,
		21165,
		6700,
		141,
		3478,
		15699,
		33096,
		50394,
		62345,
		65325,
		58430,
		43750,
		25740,
		9860,
		929,
		1655,
		11817,
		28334,
		46195,
		59984,
		65517,
		61117,
		48118,
		30463,
		13507,
		2392,
		491,
		8379,
		23665,
		41711,
		57044,
		65014,
		63203,
		52160,
		35235,
		17562,
		4500,
		12,
		5459,
		19189,
		37037,
		53590,
		63827,
		64643,
		55791,
		39955,
		21939,
		7207,
		228,
		3118,
		15001,
		32273,
		49694,
		61982,
		65408,
		58934,
		44523,
		26546,
		10456,
		1134,
		1406,
		11190,
		27519,
		45440,
		59516,
		65480,
		61521,
		48841,
		31285,
		14179,
		2711,
		359,
		7837,
		22877,
		40916,
		56484,
		64858,
		63498,
		52818,
		36056,
		18296,
		4925,
		0,
		5013,
		18444,
		36219,
		52948,
		63555,
		64824,
		56370,
		40756,
		22720,
		7730,
		335,
		2777,
		14315,
		31450,
		48984,
		61600,
		65470,
		59421,
		45288,
		27357,
		11067,
		1359,
		1177,
		10577,
		26708,
		44676,
		59032,
		65422,
		61907,
		49553,
		32108,
		14863,
		3048,
		248,
		7311,
		22095,
		40116,
		55908,
		64681,
		63775,
		53463,
		36874,
		19039,
		5368,
		8,
		4584,
		17708,
		35399,
		52293,
		63264,
		64985,
		56933,
		41552,
		23507,
		8270,
		463,
		2455,
		13640,
		30627,
		48263,
		61199,
		65511,
		59891,
		46045,
		28170,
		11690,
		1603,
		968,
		9978,
		25901,
		43905,
		58532,
		65343,
		62274,
		50255,
		32932,
		15558,
		3405,
		157,
		6800,
		21319,
		39311,
		55318,
		64484,
		64031,
		54095,
		37690,
		19791,
		5829,
		37,
		4172,
		16981,
		34578,
		51625,
		62953,
		65125,
		57482,
		42343,
		24299,
		8824,
		611,
		2151,
		12978,
		29806,
		47533,
		60781,
		65532,
		60345,
		46793,
		28987,
		12328,
		1868,
		780,
		9394,
		25098,
		43127,
		58015,
		65244,
		62623,
		50946,
		33755,
		16265,
		3779,
		86,
		6306,
		20551,
		38502,
		54713,
		64268,
		64268,
		54713,
		38502,
		20551,
		6306,
		86,
		3779,
		16265,
		33755,
		50946,
		62623,
		65244,
		58015,
		43127,
		25098,
		9394,
		780,
		1868,
		12328,
		28987,
		46793,
		60345,
		65532,
		60781,
		47533,
		29806,
		12978,
		2151,
		611,
		8824,
		24299,
		42343,
		57482,
		65125,
		62953,
		51625,
		34578,
		16981,
		4172,
		37,
		5829,
		19791,
		37690,
		54095,
		64031,
		64484,
		55318,
		39311,
		21319,
		6800,
		157,
		3405,
		15558,
		32932,
		50255,
		62274,
		65343,
		58532,
		43905,
		25901,
		9978,
		968,
};
uint16_t* samplePTR = sample;

//*****************************************************************************/
/// \fn void read_adc() 
/// \brief - reads adc value from ADC1
//*****************************************************************************/
void read_adc()
{
	ADC1->CR2 |= 0x40000000;
	while(!(ADC1->SR & 2)) {}       /* wait for conv complete */
		adc_data = ADC1->DR;
}

/*----------------------------------------------------------------------------
 MAIN function
 *----------------------------------------------------------------------------*/
int main() 
{
	ADC1->CR2 	= 0;                  /* SW trigger */
	ADC1->SQR3 = 1;                 
  ADC1->SQR1	= 0;               
  ADC1->CR2	|= 1;

	read_adc();
	
	double freq = get_Freq(sample);
	get_Vel();
	tick.attach_us(timer0, 100);
  uint32_t  count = 0;

// initialize serial buffer pointers
  rx_in_ptr =  rx_buf; /* pointer to the receive in data */
  rx_out_ptr = rx_buf; /* pointer to the receive out data*/
  tx_in_ptr =  tx_buf; /* pointer to the transmit in data*/
  tx_out_ptr = tx_buf; /* pointer to the transmit out */	
   

/****************      ECEN 5803 add code as indicated   **********************/
    // uncomment this section after adding monitor code.  
   /* send a starting message to the terminal  */                    
	
   set_display_mode();                                      
   
	 while(1)       /// Cyclical Executive Loop
    {
			count++;               // counts the number of times through the loop
//      __enable_interrupts();
//      __clear_watchdog_timer();

/****************      ECEN 5803 add code as indicated   **********************/
    // uncomment this section after you add the monitor code. 
    
       serial();            // Polls the serial port
       chk_UART_msg();     // checks for a serial port message received
       monitor();           // Sends serial port output messages depending
                         //     on commands received and display mode
 
   /****************      ECEN 5803 add code as indicated   ***************/
   
//  readADC()
			
//			
////  calculate flow()
			double flow = get_Flow();

////  LCD_Display()   // use the SPI port to send flow number
			lcd.init_lcd();
			lcd.clr_lcd();											//clear LCD output
			lcd.set_cursor(0,0);								//set cursor back to origin
			lcd.printf("Flow = %f", flow);			//print temp to LCD

////  4-20 output ()    // use PWM1 channel 1  proporional rate to flow
////-----------------------------------------------------------------------------------		
			double duty_cycle = flow/(MAX_FLOW - MIN_FLOW);
			current_loop = duty_cycle;
//			
////  Pulse output()   // use PWM1 channel 2  propotional pulse rate to frequency
////-----------------------------------------------------------------------------------	
			duty_cycle = freq/(MAX_FREQ - MIN_FREQ);
			pulse_out = duty_cycle;
//-----------------------------------------------------------------------------------
//  End ECEN 5803 code addition
		}
}