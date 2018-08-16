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
#define BLINK_CYCLE_ERROR 1000
#define BLINK_ON_TIME_ERROR 700


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
bool blinkLedEnable = false;
uint32_t timeStartBlinkCycle;

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
void blinkLedErrorProcess()
{
	if(blinkLedEnable)
	{
		if(elapsedTime(g_sysTime, timeStartBlinkCycle) < BLINK_ON_TIME_ERROR)
		{
			Led7seg_turnOnLed();
		}
		else if(elapsedTime(g_sysTime, timeStartBlinkCycle) < BLINK_CYCLE_ERROR)
		{
			Led7seg_turnOffLed();
		}
		else{
			timeStartBlinkCycle = g_sysTime;
		}
	}
}
void errorDisplayProcess()
{
	if(ErrorCheck_haveError())
	{
		if(elapsedTime(g_sysTime, timeStartDisplayError) >= TIME_SHOW_1_ERROR)
		{
			timeStartDisplayError = g_sysTime;
			currentError = ErrorCheck_getNextError(currentError);
		}
	}
}
/******************************************************************************
* Global functions
******************************************************************************/
void Display_process()
{
	Buzzer_process();
	errorDisplayProcess();
	blinkLedErrorProcess();
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

void Display_showFilterTime(uint8_t filter)
{
	Led_switchMachineStateLed(MACHINE_STATE_LED_FILTER);
	Led7seg_SetNumberInLed4(filter_time_getFilterHour(filter));
	Led7seg_SetNumberInLed1(filter +1);
}

void Display_showFilterLifeTime(uint8_t filter)
{
	Led_switchMachineStateLed(MACHINE_STATE_LED_FILTER);
	Led7seg_SetNumberInLed4(g_userConfig.filterLifeTime[filter]/3600);
	Led7seg_SetNumberInLed1(filter +1);
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
	blinkLedEnable = true;
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
void Display_showNewError(ErrorType_t newError)
{
	currentError = newError;
	timeStartDisplayError = g_sysTime;
	Buzzer_blinkError();
	timeStartBlinkCycle = g_sysTime;
}

void Display_turnOffBlinkled()
{
	blinkLedEnable = false;
	Led7seg_turnOnLed();
}

