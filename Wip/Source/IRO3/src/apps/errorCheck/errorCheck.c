/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        errorCheck.c
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


#include "errorCheck.h"
#include "gpio.h"
#include "timeCheck.h"
#include "user_config.h"
#include "adc.h"
#include "filter_time.h"
#include "pumpControl.h"
#include "adc.h"

/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define MIN_TIME_WATER_STABILITY 	20000
#define MIN_TIME_WATER_HAVE 		10000
#define PUMP_RUN_OVER_TIME 			18000000
/******************************************************************************
* Local types
******************************************************************************/



typedef enum
{
	WATER_IN_ERROR_NOMAL = 0,
	WATER_IN_ERROR_LOST,
	WATER_IN_ERROR_NO_STABILITY,
}WaterInError_t;
/******************************************************************************
* Local function prototypes
******************************************************************************/
void checkFilter();
void checkWaterIn();
void checkPumpRunTime();
void checkTdsLimit();
void checkH2ODet();

void newErrorOccur(ErrorType_t error);

/******************************************************************************
* Local variables
******************************************************************************/
bool currentErrors[MAX_ERROR_NUM];
ErrorType_t currentDisplay;
uint32_t lastTimeShow;

bool s_oldWaterInState = true;
WaterInError_t s_waterInError = WATER_IN_ERROR_NOMAL;
uint32_t s_timeHaveWater = 0;
uint32_t s_timeLostWater = 0;
uint8_t s_waterInBlinkCnt = 0;
bool lastError = false;
ErrorType_t s_recentNewError;
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
void newErrorOccur(ErrorType_t error)
{
	currentErrors[error] = true;
	ErroCheck_newError_cb(error);
	s_recentNewError = error;
//	Display_turnBuzzer10Time();
//	currentDisplay = error;
//	lastTimeShow = g_sysTime;
}

void checkFilter()
{
	for(uint8_t i = 0; i<FILTER_NUM ; i++)
	{
		if(filter_time_getFilterSecond(i) == 0)
		{
			if(currentErrors[i] == false)
				newErrorOccur(i);
		}
		else
		{
			currentErrors[i] = false;
		}
	}
}
void checkWaterIn()
{
	if(CHECK_CO_AP_THAP )
	{
		if(!s_oldWaterInState)
		{
			s_oldWaterInState = true;
			s_timeHaveWater = g_sysTime;
		}
		else if(elapsedTime(g_sysTime,s_timeHaveWater) > MIN_TIME_WATER_HAVE)
		{
			s_waterInBlinkCnt = 0;
			if(s_waterInError != WATER_IN_ERROR_NOMAL)
			{
				s_waterInError = WATER_IN_ERROR_NOMAL;
				currentErrors[ERROR_TYPE_INCOME_WATER_LOST] = false;
				currentErrors[ERROR_TYPE_INCOME_WATER_NO_STABILITY] = false;
			}
		}
		if(s_waterInError == WATER_IN_ERROR_LOST)
		{
			s_waterInError = WATER_IN_ERROR_NOMAL;
			currentErrors[ERROR_TYPE_INCOME_WATER_LOST] = false;
			if(s_recentNewError == ERROR_TYPE_INCOME_WATER_LOST)
			{
				Display_cancelError();
			}
		}

	}
	else if((!CHECK_CO_AP_THAP)&& s_oldWaterInState)
	{
		s_oldWaterInState = false;
		s_timeLostWater = g_sysTime;

		if((elapsedTime(g_sysTime,s_timeHaveWater) < MIN_TIME_WATER_HAVE))
		{
			if((++s_waterInBlinkCnt >= 5) && (s_waterInError != WATER_IN_ERROR_NO_STABILITY))
			{
				s_waterInError = WATER_IN_ERROR_NO_STABILITY;
				currentErrors[ERROR_TYPE_INCOME_WATER_LOST] = false;
				newErrorOccur(ERROR_TYPE_INCOME_WATER_NO_STABILITY);
				s_waterInBlinkCnt = 0;
			}
		}
	}
	if((!CHECK_CO_AP_THAP)&& (!s_oldWaterInState)&&
			(elapsedTime(g_sysTime, s_timeLostWater) > MIN_TIME_WATER_STABILITY) &&
			(s_waterInError != WATER_IN_ERROR_LOST))
	{
		s_waterInError = WATER_IN_ERROR_LOST;
		currentErrors[ERROR_TYPE_INCOME_WATER_NO_STABILITY] = false;
		newErrorOccur(ERROR_TYPE_INCOME_WATER_LOST);
	}
}
void checkPumpRunTime()
{
	uint32_t timeRun = pumpControl_getTimePumpRun();
	if((timeRun > PUMP_RUN_OVER_TIME) && (!currentErrors[ERROR_TYPE_PUMP_RUN_OVER_TIME]))
	{
		newErrorOccur(ERROR_TYPE_PUMP_RUN_OVER_TIME);
	}
}
void checkTdsLimit()
{
	if((ADC_GetTdsValue(TDS_IN_VALUE) > g_userConfig.tdsLimitIn) && (!currentErrors[ERROR_TYPE_TDS_IN]))
	{
		newErrorOccur(ERROR_TYPE_TDS_IN);
	}
	if((ADC_GetTdsValue(TDS_OUT_VALUE) > g_userConfig.tdsLimitOut)&& (!currentErrors[ERROR_TYPE_TDS_OUT]))
	{
		newErrorOccur(ERROR_TYPE_TDS_OUT);
	}
}
void checkH2ODet()
{
	if(ADC_GetH2oDet() && (!currentErrors[ERROR_TYPE_LEAK_WATER]))
	{
		pumpControl_turnOffAll();
		newErrorOccur(ERROR_TYPE_LEAK_WATER);
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
void ErrorCheck_init()
{
	for(int8_t i = 0; i<MAX_ERROR_NUM ; i++)
	{
		currentErrors[i] = false;
	}
}
void ErrorCheck_process()
{
	checkFilter();
	checkWaterIn();
	checkPumpRunTime();
	checkTdsLimit();
	checkH2ODet();
	if((lastError == true) && (ErrorCheck_haveError()== false))
	{
		ErrorCheck_allErrorAreRemoved_cb();
	}
	lastError = ErrorCheck_haveError();
}
bool ErrorCheck_haveError()
{
	for(uint8_t i=0; i< MAX_ERROR_NUM; i++)
	{
		if(currentErrors[i] == true)
			return true;
	}
	return false;
}

ErrorType_t ErrorCheck_getNextError(ErrorType_t currentError)
{
	ErrorType_t nextError = currentError;
	while(1)
	{
		if(++nextError == MAX_ERROR_NUM)
			nextError = ERROR_TYPE_FILTER_1;
		if(currentErrors[nextError])
		{
			return nextError;
		}
	}
	return currentError;
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


