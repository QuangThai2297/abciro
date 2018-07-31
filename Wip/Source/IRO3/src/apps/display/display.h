/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         display.h
 *
 * @author    	quanvu
 *
 * @version   1.0
 *
 * @date
 *
 * @brief     Brief description of the file
 *
 * Detailed Description of the file. If not used, remove the separator above.
 *
 */

#ifndef APPS_DISPLAY_DISPLAY_H_
#define APPS_DISPLAY_DISPLAY_H_


/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>
#include <stdint.h>
/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/
#define LED_7SEG_OFF 10



/******************************************************************************
* Types
******************************************************************************/
typedef enum LED_KEY_NAME
{
	LED_KEY_NAME_SELECT = 0,
	LED_KEY_NAME_PLUS,
	LED_KEY_NAME_MINUS,
	LED_KEY_NAME_SET,
	LED_NAME_FILTER,
	LED_NAME_TDS_IN,
	LED_NAME_TDS_OUT
}LedKeyName_t;

typedef enum LED_FUNC_NAME
{
	LED_FUNC_NAME_FILTER = 0,
	LED_FUNC_NAME_TDS_IN,
	LED_FUNC_NAME_TDS_OUT
}LedFuncName_t;

typedef enum LED_KEY_COLLOR
{
	LED_KEY_COLLOR_RED = 0,
	LED_KEY_COLLOR_GREEN
}LedKeyColor_t;

typedef enum LED_STATE
{
	LED_STATE_OFF = 0,
	LED_STATE_ON
}LedState_t;

typedef enum
{
	MACHINE_STATE_LED_TDS_OUT = 0,
	MACHINE_STATE_LED_TDS_IN,
	MACHINE_STATE_LED_FILTER,
}MachineStateLed_t;
/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
//void Display_scanLed(void);
void Display_SetNumberInLed4(uint16_t number); // set number for 4 digit led
void Display_SetNumberInLed1(int8_t number); // set number for 1 digit led
void Display_scanLed(void);
void Display_process();
void Display_onBuzzerInMs(uint16_t msTime);

void Display_SetLedKeyState(LedKeyName_t ledName,LedKeyColor_t color, LedState_t state);
void Display_turnOnLedKey();
void Display_turnOffLedKey();

void Display_switchMachineStateLed(MachineStateLed_t machineState);

void Display_showFilterTime(uint8_t filter);

void Display_turnOnAllIn1s();

/******************************************************************************
* Inline functions
******************************************************************************/


#endif /* APPS_DISPLAY_DISPLAY_H_ */



