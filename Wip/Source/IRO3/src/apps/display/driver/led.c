/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        led.c
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


#include "led.h"
#include "gpio.h"

/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/



const gpio_port_pin_t LED_KEY_RED_PIN[] = {GPIO_PORT_B_PIN_6,GPIO_PORT_C_PIN_3,GPIO_PORT_C_PIN_6,GPIO_PORT_5_PIN_4};
const gpio_port_pin_t LED_KEY_GREEN_PIN[] = {GPIO_PORT_C_PIN_2,GPIO_PORT_C_PIN_5,GPIO_PORT_C_PIN_7,GPIO_PORT_5_PIN_5};
const gpio_port_pin_t LED_MACHINE_STATE_PIN[LED_MACHINE_STATE_NUM] = {LED_MACHINE_STATE_TDS_OUT,LED_MACHINE_STATE_TDS_IN,LED_MACHINE_STATE_FILTER};

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/


/******************************************************************************
* Local variables
******************************************************************************/

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

void Led_SetLedKeyState(LedKeyName_t ledName,LedKeyColor_t color, LedState_t state)
{
	if(color == LED_KEY_COLLOR_GREEN)
	{
		R_GPIO_PinWrite(LED_KEY_GREEN_PIN[ledName],state == LED_STATE_ON? GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH);
	}else
	{
		R_GPIO_PinWrite(LED_KEY_RED_PIN[ledName],state == LED_STATE_ON? GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH);
	}
}

void Led_turnOnLedKey()
{
	for(uint8_t i = 0; i< PIN_KEY_NUM; i++)
	{
		R_GPIO_PinWrite(LED_KEY_GREEN_PIN[i],GPIO_LEVEL_LOW);
	}
}
void Led_turnOffLedKey()
{
	for(uint8_t i = 0; i< PIN_KEY_NUM; i++)
	{
		R_GPIO_PinWrite(LED_KEY_GREEN_PIN[i],GPIO_LEVEL_HIGH);
	}
}

void Led_switchMachineStateLed(MachineStateLed_t machineState)
{
	uint8_t i;
	for ( i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		R_GPIO_PinWrite(LED_MACHINE_STATE_PIN[i], GPIO_LEVEL_HIGH);
	}
	R_GPIO_PinWrite(LED_MACHINE_STATE_PIN[machineState], GPIO_LEVEL_LOW);
}

void Led_turnOffMachineStateled()
{
	uint8_t i;
	for ( i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		R_GPIO_PinWrite(LED_MACHINE_STATE_PIN[i], GPIO_LEVEL_HIGH);
	}
}
void Led_turnOnAll()
{
	Led_turnOnLedKey();
	for (uint8_t i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		R_GPIO_PinWrite(LED_MACHINE_STATE_PIN[i], GPIO_LEVEL_LOW);
	}
}
