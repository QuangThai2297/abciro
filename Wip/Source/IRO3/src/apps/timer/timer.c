/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        timer.c
*
* @author    trongkn
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

#include <timer.h>

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



/******************************************************************************
* Local functions
******************************************************************************/


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

PUBLIC void delay_clk (uint16_t delay)
{	
	uint16_t time = delay;
	uint16_t i = 0; 
	//while (time--!=0);
	for(i=0;i<time;i++)
	{
		
	}
}

uint32_t elapsedTime(uint32_t newTime,uint32_t oldTime)
{
	if(newTime >= oldTime)
	{
		return (newTime - oldTime);
	}else
	{
		return (newTime + (0xffffffff - oldTime));
	}
}
/**
 * @brief One line documentation 
 * interrup every 1 ms
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
