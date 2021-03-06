
/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         led.h
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

#ifndef APPS_DISPLAY_LED_H_
#define APPS_DISPLAY_LED_H_


/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/



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
	MACHINE_STATE_LED_FILTER  = 0,
	MACHINE_STATE_LED_TDS_IN,
	MACHINE_STATE_LED_TDS_OUT,
}MachineStateLed_t;
/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
//tat ca cac ham này có nhiệm vụ cho phép on off led nào trong LedKeyName_t tương ứng với các trạng thái của MachineStateLed_t, hay của key

void Led_SetLedKeyState(LedKeyName_t ledName,LedKeyColor_t color, LedState_t state);
void Led_turnOnLedKey();
void Led_turnOffLedKey();

void Led_switchMachineStateLed(MachineStateLed_t machineState);
void Led_turnOffMachineStateled();
void Led_turnOnMachineStateled();

void Led_turnOnAll();
void Led_turnAllLedMachineState(LedState_t state);
void Led_turnAllLedKey(LedKeyColor_t color);
void Led_turnOffAllLedKey();

/******************************************************************************
* Inline functions
******************************************************************************/







#endif /* APPS_DISPLAY_LED_H_ */
