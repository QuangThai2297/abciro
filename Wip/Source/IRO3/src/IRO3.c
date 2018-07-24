/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        main.c
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

#include "r_smc_entry.h"
#include "config.h"
#include "gpio.h"
#include "adc.h"
#include "display.h"
#include "touch_app.h"




/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/
PUBLIC volatile uint8_t g_state = IDLE_STATE;

PUBLIC volatile uint8_t  g_led_number = 0;

uint16_t g_adc_result;

/******************************************************************************
* Constants and macros
******************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/


/******************************************************************************
* Local variables
******************************************************************************/

uint16_t value ;
uint32_t time_out = 0;
uint32_t i = 0;
//LOCAL BOOLEAN s_is_timeout = TRUE;



/******************************************************************************
* Local functions
******************************************************************************/
void run1msTask();

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
void main(void);
void main(void)
{


	R_Config_CMT0_Start();
	R_Config_CMT1_Start();
	g_state = IDLE_STATE;
	TOUCH_init();
	ADC_Init();
	GPIO_Init();
	ADC_ReadTds(ADCHANNEL0);
	//test code
	Display_SetNumberInLed4(ADC_GetTdsValue());
	Display_SetNumberInLed1(g_led_number);
	//end test code

	/* Main loop */
	while(1)
	{
		//////////////////////
    	if(g_run1msFlag == 1)
    	{
    		run1msTask();
    		g_run1msFlag= 0;
    	}
		switch (g_state)
		{
			case IDLE_STATE:
			{

				ADC_ReadTds(ADCHANNEL0);
			}
			if(g_sysTime == 100)
			{
				//Display_SetNumberInLed4(ADC_GetAdcTdsValue());
				Display_SetNumberInLed4(ADC_GetTdsValue());

			}

//			if(g_sysTime == 1500)
//			{
//				Display_SetNumberInLed4(ADC_GetAdcLowValue());
//			}
			break;
			case SETTING_STATE:
			// @ quan: handle here
			break;
			case CALIB_TDS_STATE:

			break;
			default :
			break;
		}
	}
}
void run1msTask()
{
	Display_scanLed();
	TOUCH_process();
}


void abort(void)
{
	while(1)
	{
		;
	}
}
