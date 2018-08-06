/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        pumpControl.c
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

#include <pumpControl.h>
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
#define CHECK_CO_AP_THAP 	(R_GPIO_PinRead(PIN_AP_THAP)== GPIO_LEVEL_HIGH)
#define CHECK_CO_AP_CAO 	(R_GPIO_PinRead(PIN_AP_CAO)== GPIO_LEVEL_HIGH)
#define TURN_ON_PUMP 		R_GPIO_PinWrite(PIN_DK_BOM,GPIO_LEVEL_HIGH)
#define TURN_OFF_PUMP 		R_GPIO_PinWrite(PIN_DK_BOM,GPIO_LEVEL_LOW)

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/


/******************************************************************************
* Local variables
******************************************************************************/
bool s_pumpIsOn = false;


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
void pumpControl_process()
{
	if(CHECK_CO_AP_THAP && CHECK_CO_AP_CAO )
	{
		if(!s_pumpIsOn)
		{
			TURN_ON_PUMP;
		}
	}
	else
	{
		if(s_pumpIsOn)
		{
			TURN_OFF_PUMP;
		}
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


