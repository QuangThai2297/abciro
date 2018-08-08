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

/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define MIN_TIME_WATER_STABILITY 10000
#define PUMP_RUN_OVER_TIME 18000000
/******************************************************************************
* Local types
******************************************************************************/
typedef enum
{
	ERROR_CODE_INCOME_WATER_LOST = 4,
	ERROR_CODE_INCOME_WATER_NO_STABILITY = 2,
	ERROR_CODE_PUMP_RUN_OVER_TIME = 1,
	ERROR_CODE_LEAK_WATER = 5,
}ErrorCode_t;
typedef enum
{
	ERROR_TYPE_FILTER_1 = 0,
	ERROR_TYPE_FILTER_2,
	ERROR_TYPE_FILTER_3,
	ERROR_TYPE_FILTER_4,
	ERROR_TYPE_FILTER_5,
	ERROR_TYPE_FILTER_6,
	ERROR_TYPE_FILTER_7,
	ERROR_TYPE_FILTER_8,
	ERROR_TYPE_FILTER_9,
	ERROR_TYPE_INCOME_WATER_LOST,
	ERROR_TYPE_INCOME_WATER_NO_STABILITY ,
	ERROR_TYPE_PUMP_RUN_OVER_TIME ,
	ERROR_TYPE_LEAK_WATER,
	ERROR_TYPE_TDS_IN,
	ERROR_TYPE_TDS_OUT,
	MAX_ERROR_NUM,
}ErrorType_t;

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
void newErrorOccur(ErrorType_t error);
void checkPumpRunTime();


/******************************************************************************
* Local variables
******************************************************************************/
bool currentErrors[MAX_ERROR_NUM];
uint8_t errorCount = 0;
ErrorType_t currentDisplay;
uint32_t lastTimeShow;

bool s_oldWaterInState = true;
WaterInError_t s_waterInError = WATER_IN_ERROR_NOMAL;
uint32_t s_timeHaveWater = 0;
uint8_t s_waterInBlinkCnt = 0;
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
//	Display_turnBuzzer10Time();
//	currentDisplay = error;
//	lastTimeShow = g_sysTime;
}
void checkFilter()
{
	for(uint8_t i = 0; i<FILTER_NUM ; i++)
	{
		if((filter_time_getFilterSecond(i) == 0) && (currentErrors[i] == false))
		{
			newErrorOccur(i);
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
		else if((elapsedTime(g_sysTime,s_timeHaveWater) > MIN_TIME_WATER_STABILITY) && (s_waterInError != WATER_IN_ERROR_NOMAL))
		{
			s_waterInError = WATER_IN_ERROR_NOMAL;
		}

	}
	else if((!CHECK_CO_AP_THAP)&& s_oldWaterInState)
	{
		s_oldWaterInState = false;
		if(s_waterInError == WATER_IN_ERROR_NOMAL)
		{
			s_waterInError = WATER_IN_ERROR_LOST;
			newErrorOccur(ERROR_TYPE_INCOME_WATER_LOST);
		}
		else if(s_waterInError == WATER_IN_ERROR_LOST)
		{
			if(++s_waterInBlinkCnt >= 5)
			{
				s_waterInError = WATER_IN_ERROR_NO_STABILITY;
				currentErrors[ERROR_TYPE_INCOME_WATER_LOST] = false;
				newErrorOccur(ERROR_TYPE_INCOME_WATER_NO_STABILITY);
			}
		}
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
}
bool ErrorCheck_haveError()
{
	return (errorCount > 0 ? true:false);
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


