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
#include "flash_app.h"
#include "filter_time.h"
#include "UIControl.h"
#include "pumpControl.h"
#include "timer.h"




/******************************************************************************
* External objects
******************************************************************************/
extern volatile uint8_t g_run200usFlag;
extern volatile uint8_t g_run1msFlag;
extern uint8_t g_adc_flag;
/******************************************************************************
* Global variables
******************************************************************************/


uint16_t g_adc_result;

/******************************************************************************
* Constants and macros
******************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define TIME_TO_DISPLAY_TDS  (2000) //2 secs
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
uint8_t s_timeOut100ms;
LOCAL uint8_t s_pwm_cnt = 0;
//LOCAL BOOLEAN s_is_timeout = TRUE;



/******************************************************************************
* Local functions
******************************************************************************/
void run200usTask();
void run1msTask();
void run100msTask();
void run_DisplayTds();

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
	uint16_t s_adc_h2o_det = 0;
	R_Config_CMT0_Start();
	R_Config_CMT1_Start();
//	TOUCH_init();
	TOUCH_init();
	UART_Init();
	flash_app_init();
	ADC_Init();
	TIMER_Init();
	Display_turnOnAllIn1s();
	/* Main loop */
	while(1)
	{

		//////////////////////
    	if(g_run200usFlag == 1)
    	{
    		run200usTask();
    		g_run200usFlag= 0;
    	}
    	if(g_run1msFlag == 1)
    	{
    		run1msTask();
    		g_run1msFlag= 0;
    	}
    	if(s_timeOut100ms >= 100)
    	{
    		run100msTask();
    		s_timeOut100ms = 0;
    	}
    	if(g_adc_flag)
    	{
    		ADC_UpdateTds (s_pwm_cnt);

    	}

    	TIMER_CheckTimerEvent();
	}
}

void run200usTask()
{
	Display_process();
}
void run1msTask()
{
	TOUCH_process();
	if(s_timeOut100ms <100)
	{
		s_timeOut100ms++;
	}

}
void run100msTask()
{
	UIControl_process();
	pumpControl_process();
}

void abort(void)
{
	while(1)
	{
		;
	}
}
