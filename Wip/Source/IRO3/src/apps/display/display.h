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
#include "led7seg.h"
#include "led.h"
#include "buzzer.h"
#include "errorCheck.h"

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/



/******************************************************************************
* Types
******************************************************************************/
typedef enum
{
	ERROR_CODE_INCOME_WATER_LOST = 4,
	ERROR_CODE_INCOME_WATER_NO_STABILITY = 2,
	ERROR_CODE_PUMP_RUN_OVER_TIME = 1,
	ERROR_CODE_LEAK_WATER = 5,
}ErrorCode_t;


/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/


void Display_process();
void Display_showStart();

void Display_showTdsOut();
void Display_showTdsIn();
void Display_showFilterLifeTime(uint8_t filter);
void Display_showFilterTime(uint8_t filter);


void Display_showCurentError();
void Display_showTdsInLimit();
void Display_showTdsOutLimit();
void Display_showNewError(ErrorType_t newError);

void Display_turnOffBlinkled();

/******************************************************************************
* Inline functions
******************************************************************************/


#endif /* APPS_DISPLAY_DISPLAY_H_ */



