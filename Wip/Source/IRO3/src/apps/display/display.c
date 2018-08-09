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
	Led7seg_scanLed();
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
