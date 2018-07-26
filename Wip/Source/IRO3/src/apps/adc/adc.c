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
#define TIME_OUT_MAX  (10)

#define ADC_LOW_VALUE  (0)

#define ADC_MAX_SAMPLE  (200)

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

LOCAL uint16_t s_adc_tds_value = 0;

LOCAL uint16_t s_adc_low_value = 0;

LOCAL uint32_t s_sum_adc = 0;

LOCAL uint16_t s_sma_adc = 0;

LOCAL QUEUE_NODE_T* s_adc_sample_queue = NULL;

//LOCAL uint16_t s_adc_low_value = 0;

/******************************************************************************
* Local functions
******************************************************************************/

LOCAL void ADC_PushDataToQueue (uint16_t data )
{
	uint16_t sample = 0;
		if(!QUEUE_QueueIsFull(s_adc_sample_queue))
		{
			s_sum_adc = s_sum_adc + data;
			QUEUE_EnQueue(s_adc_sample_queue,&data);
			s_sma_adc  = data;
		}
		else
		{

			QUEUE_DeQueue(s_adc_sample_queue,&sample);
			s_sum_adc = s_sum_adc + data - sample;
			s_sma_adc = s_sum_adc/s_adc_sample_queue->max_count;
			QUEUE_EnQueue(s_adc_sample_queue,&data);

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
PUBLIC void ADC_Init()
{   
//	R_Config_S12AD0_Start();
	s_adc_sample_queue = QUEUE_InitQueue(ADC_SAMPLE_QUEUE_SIZE,sizeof(uint16_t));
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
PUBLIC ERR_E  ADC_ReadTds(ad_channel_t channel)
{
	ERR_E ret = OK;
	uint16_t sample_cnt = 0;
	uint16_t low_value_cnt = 0;
	uint16_t high_value_cnt = 0;
	uint16_t adc_temp = 0;
	uint32_t high_value_sum = 0;
	uint32_t low_value_sum = 0;
	uint16_t adc_low_result = 0;
	uint16_t adc_high_result = 0;
	uint16_t adc_sma_value = 0;
	if(channel >= ADC_MAX ) return ERR;
	g_adc_flag = 0U;
  // cal SMA
	for (sample_cnt = 0;sample_cnt <= ADC_MAX_SAMPLE;sample_cnt ++ )
	{
		if(ADC_Read(channel,&adc_temp) != OK)
		{
			return ERR;
		}
		else
		{
			if(adc_temp > ADC_HIGH_VALUE)
			{
				high_value_cnt ++;
				high_value_sum += adc_temp;
			}

		}
	}


	if(high_value_cnt == 0) return ERR;
	adc_high_result = (high_value_cnt == 0)?0:(high_value_sum/high_value_cnt);
	adc_sma_value = (high_value_sum/(2*high_value_cnt));

//	adc_sma_value = (high_value_sum/high_value_cnt);

	high_value_cnt = 0;
	low_value_cnt  = 0;
	high_value_sum = 0;
	low_value_sum  = 0;


	for (sample_cnt = 0;sample_cnt <= ADC_MAX_SAMPLE;sample_cnt ++ )
	{
		if(ADC_Read(channel,&adc_temp) != OK)
		{
			return ERR;
		}
		else
		{
			if(adc_temp < adc_sma_value)
			{

					low_value_cnt ++;
					low_value_sum += adc_temp;

			}
			else if(adc_temp > adc_sma_value)
			{
				high_value_cnt ++;
				high_value_sum += adc_temp;
			}

		}
	}

	 adc_low_result = (low_value_cnt == 0)?0:(low_value_sum/low_value_cnt);
	 adc_high_result = (high_value_cnt == 0)?0:(high_value_sum/high_value_cnt);
	s_adc_tds_value  = adc_high_result - adc_low_result;
	s_adc_low_value = adc_sma_value;
	ADC_PushDataToQueue(s_adc_tds_value);
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
//PUBLIC ERR_E  ADC_ReadTds(ad_channel_t channel)
//{
//	ERR_E ret = OK;
//	uint16_t sample_cnt = 0;
//	uint16_t low_value_cnt = 0;
//	uint16_t high_value_cnt = 0;
//	uint16_t adc_temp = 0;
//	uint32_t high_value_sum = 0;
//	uint32_t low_value_sum = 0;
//	uint16_t adc_low_result = 0;
//	uint16_t adc_high_result = 0;
//	if(channel >= ADC_MAX ) return ERR;
//	g_adc_flag = 0U;
//
//	for (sample_cnt = 0;sample_cnt <= ADC_MAX_SAMPLE;sample_cnt ++ )
//	{
//		if(ADC_Read(channel,&adc_temp) != OK)
//		{
//			return ERR;
//		}
//		else
//		{
//			if(adc_temp < ADC_LOW_VALUE)
//			{
//				low_value_cnt ++;
//				low_value_sum += adc_temp;
//			}
//			else if(adc_temp > ADC_HIGH_VALUE)
//			{
//				high_value_cnt ++;
//				high_value_sum += adc_temp;
//			}
//
//
//		}
//	}
//	 adc_low_result = (low_value_cnt == 0)?0:(low_value_sum/low_value_cnt);
//	 adc_high_result = (high_value_cnt == 0)?0:(high_value_sum/high_value_cnt);
//	s_adc_tds_value  = (adc_high_result + s_adc_tds_value)/2;
//	s_adc_low_value = (high_value_cnt + s_adc_low_value)/2;
//	ADC_PushDataToQueue(s_adc_tds_value);
//	 return ret;
//
//}
PUBLIC uint16_t  ADC_GetAdcTdsValue()
{

	return s_sma_adc;
//return s_adc_tds_value;


}

PUBLIC uint16_t  ADC_GetAdcLowValue()
{

	return s_adc_low_value;
//return s_adc_tds_value;


}

PUBLIC uint16_t  ADC_GetTdsValue()
{

	uint32		calib_value[7] =  {2100,1985,1730,1675,1610,1111,1018};
	uint32				fix_value[7]   = {0,4,17,19,22,25,28};
	float				calculate_value = 0;
	float				slope;
	uint16_t            tds_return = 0;
	uint32				index_level = 0;
	uint16_t adc0_value = s_sma_adc;

	if(adc0_value > calib_value[0] | (adc0_value == 0))
	{
		tds_return = 0;
		return tds_return;
	}

	if(adc0_value < calib_value[6])
	{
		tds_return = fix_value[6];
		return tds_return;
	}

	for(index_level = 0; index_level < 6 ; index_level++)
	{

		if((adc0_value <= calib_value[index_level]) && (adc0_value >= calib_value[index_level+1]) )
		{
			slope = ((float)(fix_value[index_level+1] - fix_value[index_level]))/((float)(calib_value[index_level] - calib_value[index_level+1]));
			calculate_value = fix_value[index_level] + slope * ( calib_value[index_level] - adc0_value);
			tds_return= (calculate_value <= 0)?0:(uint16_t) (calculate_value+0.5);
			break;
		}
	}


	return tds_return;

}


