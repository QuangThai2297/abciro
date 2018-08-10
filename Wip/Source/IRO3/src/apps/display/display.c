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
#include "display.h"
#include "adc.h"
#include "filter_time.h"
#include "timeCheck.h"
#include "gpio.h"
#include "errorCheck.h"
#include "user_config.h"
/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define TIME_SHOW_1_ERROR 5000


/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/



/******************************************************************************
* Local variables
******************************************************************************/
ErrorType_t currentError;
uint32_t timeStartDisplayError;
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
void Display_process()
{
	Buzzer_process();
	if(ErrorCheck_haveError())
	{
		if(elapsedTime(g_sysTime, timeStartDisplayError) >= TIME_SHOW_1_ERROR)
		{
			timeStartDisplayError = g_sysTime;
			currentError = ErrorCheck_getNextError(currentError);
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

void Display_showFilterTime(uint8_t filter)
{
	Led_switchMachineStateLed(MACHINE_STATE_LED_FILTER);
	Led7seg_SetNumberInLed4(filter_time_getFilterHour(filter));
	Led7seg_SetNumberInLed1(filter +1);
}
extern volatile uint8_t g_run200usFlag;

void Display_turnOnAllIn1s()
{
	Led_turnOnAll();
	Led7seg_SetNumberInLed1(8);
	Led7seg_SetNumberInLed4(8888);
	R_GPIO_PinWrite(BUZZER_PIN, GPIO_LEVEL_HIGH);
	uint32_t timeStart = g_sysTime;
	while((g_sysTime - timeStart) < 1500)
	{
    	if(g_run200usFlag == 1)
    	{
    		Led7seg_scanLed();
    		g_run200usFlag= 0;
    	}
	}
	R_GPIO_PinWrite(BUZZER_PIN, GPIO_LEVEL_LOW);

}

void Display_showTdsOut()
{
	Led7seg_SetNumberInLed1(LED_7SEG_OFF);
	Led7seg_SetNumberInLed4(ADC_GetTdsValueDisplay(TDS_OUT_VALUE));
	Led_switchMachineStateLed(MACHINE_STATE_LED_TDS_OUT);
}
void Display_showTdsIn()
{
	Led7seg_SetNumberInLed1(LED_7SEG_OFF);
	Led7seg_SetNumberInLed4(ADC_GetTdsValueDisplay(TDS_IN_VALUE));
	Led_switchMachineStateLed(MACHINE_STATE_LED_TDS_IN);
}

void Display_showGenaralError(ErrorType_t error)
{
	ErrorCode_t code;
	switch (error) {
		case ERROR_TYPE_INCOME_WATER_LOST:
			code = ERROR_CODE_INCOME_WATER_LOST;
			break;
		case ERROR_TYPE_INCOME_WATER_NO_STABILITY:
			code = ERROR_CODE_INCOME_WATER_NO_STABILITY;
			break;
		case ERROR_TYPE_LEAK_WATER:
			code = ERROR_CODE_LEAK_WATER;
			break;
		case ERROR_TYPE_PUMP_RUN_OVER_TIME:
			code = ERROR_CODE_PUMP_RUN_OVER_TIME;
			break;
		default:
			break;
	}
	Led_turnOffMachineStateled();
	Led7seg_SetNumberInLed1(LED_7SEG_ERROR);
	Led7seg_SetNumberInLed4(code);
}

void Display_showFilterExpired(uint8_t filter)
{
	Led_switchMachineStateLed(MACHINE_STATE_LED_FILTER);
	Led7seg_SetNumberInLed4(0);
	Led7seg_SetNumberInLed1(filter +1);
}
void Display_showCurentError()
{
	if(currentError <= ERROR_TYPE_FILTER_9)
	{
		Display_showFilterExpired(currentError);
	}
	else if(currentError <= ERROR_TYPE_LEAK_WATER)
	{
		Display_showGenaralError(currentError);
	}
	else if(currentError == ERROR_TYPE_TDS_IN)
	{
		Display_showTdsIn();
	}
	else if(currentError == ERROR_TYPE_TDS_OUT)
	{
		Display_showTdsOut();
	}
}

void Display_showTdsInLimit()
{
	Led7seg_SetNumberInLed1(LED_7SEG_OFF);
	Led7seg_SetNumberInLed4(g_userConfig.tdsLimitIn);
	Led_switchMachineStateLed(MACHINE_STATE_LED_TDS_IN);
}
void Display_showTdsOutLimit()
{
	Led7seg_SetNumberInLed1(LED_7SEG_OFF);
	Led7seg_SetNumberInLed4(g_userConfig.tdsLimitOut);
	Led_switchMachineStateLed(MACHINE_STATE_LED_TDS_OUT);
}
// callback
void ErroCheck_newError_cb(ErrorType_t newError)
{
	currentError = newError;
	timeStartDisplayError = g_sysTime;
	Buzzer_blinkError();
}

