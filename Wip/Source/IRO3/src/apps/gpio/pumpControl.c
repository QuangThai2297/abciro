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
#include "timer.h"
#include "filter_time.h"

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
#define TURN_ON_VAN_XA 		R_GPIO_PinWrite(PIN_VAN_XA,GPIO_LEVEL_HIGH)
#define TURN_OFF_VAN_XA 	R_GPIO_PinWrite(PIN_VAN_XA,GPIO_LEVEL_LOW)

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/
void processVanXa();
void processPump();
void onVanXaInMs(uint32_t msTime);

/******************************************************************************
* Local variables
******************************************************************************/
bool s_pumpIsOn = false;
uint32_t s_pumpStartTime = 0;
uint32_t timeOffVanXa;
bool vanXaIsOn = false;
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
void processVanXa()
{
	if(vanXaIsOn && timeIsAfter(g_sysTime , timeOffVanXa) )
	{
		TURN_OFF_VAN_XA;
		vanXaIsOn = false;
	}
}
void onVanXaInMs(uint32_t msTime)
{
	timeOffVanXa = g_sysTime + msTime;
	TURN_ON_VAN_XA;
	vanXaIsOn = true;
}
void processPump()
{
	if(CHECK_CO_AP_THAP && CHECK_CO_AP_CAO )
	{
		if(!s_pumpIsOn)
		{
			s_pumpIsOn = true;
			TURN_ON_PUMP;
			s_pumpStartTime = g_sysTime;
			onVanXaInMs(15000);
		}
	}
	else
	{
		if(s_pumpIsOn)
		{
			s_pumpIsOn = false;
			TURN_OFF_PUMP;
			uint32_t pumpRuningTime = elapsedTime(g_sysTime , s_pumpStartTime);
			filter_time_minusTime(pumpRuningTime);
			onVanXaInMs(5000);
		}
	}
}
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
	processPump();
	processVanXa();
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


