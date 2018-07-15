/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        adc.c
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


#include <adc.h>
#include "r_smc_entry.h"
//#include "Config_S12AD0.h"
/******************************************************************************
* External objects
******************************************************************************/
extern uint8_t g_adc_flag;

/******************************************************************************
* Global variables
******************************************************************************/

/* Global flag to indicate A/D conversion operation is completed */

/******************************************************************************
* Constants and macros
******************************************************************************/
#define TIME_OUT_MAX  (100)

#define ADC_LOW_VALUE  (100)

#define ADC_HIGH_VALUE  (500)

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
PUBLIC void ADC_Init()
{   
//	R_Config_S12AD0_Start();

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
PUBLIC ERR_E  ADC_Read(ad_channel_t channel,uint16_t* adc_result)
{
	ERR_E ret = OK;
	uint16_t time_out = 0;
	if(channel >= ADC_MAX ) return ERR;
	g_adc_flag = 0U;
	R_Config_S12AD0_Start();
	for (time_out = 0;time_out <= TIME_OUT_MAX;time_out ++ )
	{
		 if (1U == g_adc_flag)
		 {
			 break;
		 }
		 else if(time_out == TIME_OUT_MAX)
		 {
			 return ERR_TIMEOUT;
		 }
	}

	/* Read the A/D conversion result and store in variable */
	R_Config_S12AD0_Get_ValueResult(channel, adc_result);
	g_adc_flag = 0U;

	 return ret;

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
PUBLIC ERR_E  ADC_ReadTds(ad_channel_t channel,uint16_t* adc_high_result,uint16_t* adc_low_result,uint16_t sample)
{
	ERR_E ret = OK;
	uint16_t sample_cnt = 0;
	uint16_t low_value_cnt = 0;
	uint16_t high_value_cnt = 0;
	uint16_t adc_temp = 0;
	uint32_t high_value_sum = 0;
	uint32_t low_value_sum = 0;
	if(channel >= ADC_MAX ) return ERR;
	g_adc_flag = 0U;

	for (sample_cnt = 0;sample_cnt <= sample;sample_cnt ++ )
	{
		if(ADC_Read(channel,&adc_temp) != OK)
		{
			return ERR;
		}
		else
		{
			if(adc_temp < ADC_LOW_VALUE)
			{
				low_value_cnt ++;
				low_value_sum += adc_temp;
			}
			else if(adc_temp > ADC_HIGH_VALUE)
			{
				high_value_cnt ++;
				high_value_sum += adc_temp;
			}
		}
	}
	(uint16_t*) adc_low_result = (low_value_cnt == 0)?0:(uint16_t*)(low_value_sum/low_value_cnt);
	(uint16_t*) adc_high_result = (high_value_cnt == 0)?0:(uint16_t*)(high_value_sum/high_value_cnt);

	 return ret;

}




