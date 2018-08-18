/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        filter_time.c
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


#include "filter_time.h"
#include "flash_app.h"
#include "user_config.h"
#include <string.h>

/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
static uint32_t s_time_filter[FILTER_NUM];



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


void filter_time_updateToFlash()
{
	flash_app_writeBlock((uint8_t *)s_time_filter, FILLTER_TIME_BLOCK, sizeof(s_time_filter));
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
void filter_time_init()
{
    bool readOk = flash_app_readData((uint8_t*)s_time_filter,FILLTER_TIME_BLOCK,sizeof(s_time_filter));

    if(!readOk)
    {
    	memcpy(s_time_filter,g_userConfig.filterLifeTime,sizeof(g_userConfig));
    	filter_time_updateToFlash();
    }

//    flash_app_eraseBlock(FILLTER_TIME_BLOCK);

}

void filter_time_minusTime(uint32_t second)
{
	for(uint8_t i = 0; i< FILTER_NUM; i++)
	{
		if(s_time_filter[i] > second)
		{
			s_time_filter[i] -= second;
		}
		else // need new filter
		{
			s_time_filter[i] = 0;
		}
	}
	filter_time_updateToFlash();
}

uint16_t filter_time_getFilterHour(uint8_t filIndex)
{
	if(s_time_filter[filIndex]%3600 >0)
	{
		return (uint16_t)(s_time_filter[filIndex]/3600 +1);
	}
	else
	{
		return (uint16_t)(s_time_filter[filIndex]/3600);
	}
}

uint32_t filter_time_getFilterSecond(uint8_t filIndex)
{
	return (s_time_filter[filIndex]);
}

void filter_time_resetTimeAtIndex(uint8_t filIndex)
{
	s_time_filter[filIndex] = g_userConfig.filterLifeTime[filIndex];
	filter_time_updateToFlash();
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
