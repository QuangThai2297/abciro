/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        display.c
*
* @author    quanvu
*
* @version   1.0
*
* @date
*
* @brief
*
*******************************************************************************
*
* Detailed Description of the file. If not used, remove the separator above.
*
******************************************************************************/


#include <config.h>
#include <display.h>
#include "r_gpio_rx_if.h"

/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
const uint8_t LED7_CODE[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff};
const gpio_port_pin_t LED7_PIN[] = {GPIO_PORT_A_PIN_3,GPIO_PORT_A_PIN_0,GPIO_PORT_B_PIN_3,GPIO_PORT_A_PIN_6,GPIO_PORT_A_PIN_4,GPIO_PORT_A_PIN_1,GPIO_PORT_B_PIN_5,GPIO_PORT_B_PIN_1};
const gpio_port_pin_t LED7_DIGITS[] = {GPIO_PORT_B_PIN_7,GPIO_PORT_4_PIN_4,GPIO_PORT_4_PIN_3,GPIO_PORT_4_PIN_2,GPIO_PORT_E_PIN_2};
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/
uint8_t getDigitFromNumber(uint8_t index,uint16_t number);


/******************************************************************************
* Local variables
******************************************************************************/
uint8_t led4Digits[4] = {0xff,0xff,0xff,0xff};
uint8_t led1Number = 0;
uint8_t ledIndex;

/******************************************************************************
* Local functions
******************************************************************************/
/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */

/******************************************************************************
* Global functions
******************************************************************************/

/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
void Display_showDigitAtIndex(uint8_t digit,uint8_t index)
{
	for(int i = 0; i<5; i++)
	{
		R_GPIO_PinWrite(LED7_DIGITS[i],GPIO_LEVEL_HIGH);
	}
	for(int i = 0; i<7;i++)
	{
		R_GPIO_PinWrite(LED7_PIN[i],(digit & (1<<i)));
	}
	R_GPIO_PinWrite(LED7_DIGITS[index],GPIO_LEVEL_LOW);
}

/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
void Display_scanLed(void)
{
	uint8_t digit = 0;
	if(ledIndex < 4)
	{
		Display_showDigitAtIndex(led4Digits[ledIndex],ledIndex);
	}else{
		digit = led1Number;
		Display_showDigitAtIndex(digit,ledIndex);
	}

	if(ledIndex++ == 4)
	{
		ledIndex = 0;
	}
}

/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
void Display_SetNumberInLed1(uint8_t number)
{
	led1Number = LED7_CODE[number];
}

/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
void Display_SetNumberInLed4(uint16_t number)
{
	for(int i = 0; i<4; i++)
	{
		led4Digits[i] = LED7_CODE[number%10];
		number /= 10;
	}
	if(led4Digits[3] == LED7_CODE[0])
	{
		led4Digits[3] = LED7_CODE[10];
		if(led4Digits[2] == LED7_CODE[0])
		{
			led4Digits[2] = LED7_CODE[10];
			if(led4Digits[1] == LED7_CODE[0])
			{
				led4Digits[1] = LED7_CODE[10];
			}
		}
	}
}

