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
extern volatile uint8_t g_pwm_value;

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

LOCAL const TDS_CALIB_PARAM_T TDS_IN_CONFIG_DEFAULD =
{
		.adc_value = {2270,2210,2100,1870,1367,1180,777,146,(-200),(-350),(-805),(-940)},
		.tds_value = {0,4,10,20,31,50,118,260,400,600,1210,4000}

};


LOCAL const TDS_CALIB_PARAM_T TDS_OUT_CONFIG_DEFAULD =
{
		.adc_value = {2270,2210,2100,1870,1367,1180,777,146,(-200),(-350),(-805),(-940)},
		.tds_value = {0,4,10,20,31,50,118,260,400,600,1210,4000}

};


LOCAL TDS_CONFIG_T s_tds_calib_param;

LOCAL TDS_T  s_tds_in;

LOCAL TDS_T  s_tds_out;

LOCAL uint16_t s_interval_get_tds = 0;

LOCAL uint16_t s_200ms_cnt ;

//LOCAL uint16_t s_adc_low_value = 0;

/******************************************************************************
* Local functions
******************************************************************************/

LOCAL void ADC_PushDataToQueue (int16_t data ,TDS_T* tds)
{
	     int16_t sample = 0;
		if(!QUEUE_QueueIsFull(tds->adc_sample))
		{
			tds->sum_tds_adc +=  data;
			QUEUE_EnQueue(tds->adc_sample,&data);
			tds->sma_tds_adc  = data;
		}
		else
		{

			QUEUE_DeQueue(tds->adc_sample,&sample);
			tds->sum_tds_adc = (tds->sum_tds_adc) + data - sample;
		//	tds->sign   = (tds->sum_tds_adc >=0 )?0:1;
			tds->sma_tds_adc = (int16_t) tds->sum_tds_adc/((tds->adc_sample)->max_count);
			QUEUE_EnQueue(tds->adc_sample,&data);

		}
}


LOCAL void ADC_InitConfigFlash()
{
    bool readOk = flash_app_readData((uint8_t*)&s_tds_calib_param,TDS_PARAM_BLOCK,sizeof(s_tds_calib_param));

    if(!readOk)
    {
    	memcpy(&(s_tds_calib_param.tds_in),&TDS_IN_CONFIG_DEFAULD,sizeof(s_tds_calib_param.tds_in));
    	memcpy(&(s_tds_calib_param.tds_out),&TDS_OUT_CONFIG_DEFAULD,sizeof(s_tds_calib_param.tds_out));
    	s_tds_calib_param.tds_out_max   = TDS_OUT_MAX_DEFAULT;
    	flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));
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

	s_tds_in.adc_sample = QUEUE_InitQueue(ADC_SAMPLE_QUEUE_SIZE,sizeof(int16_t));
	s_tds_out.adc_sample = QUEUE_InitQueue(ADC_SAMPLE_QUEUE_SIZE,sizeof(int16_t));
	ADC_InitConfigFlash();
	//
	s_200ms_cnt = 0;
	PWM = 0;
	g_adc_flag = 0U;
	R_Config_S12AD0_Start();
}




PUBLIC int16_t  ADC_GetAdcTdsInValue()
{
	char dbg[UART_SEND_MAX_LEN];
	sprintf(dbg,"ADC_GetAdcTdsInValue = %d\r\n",s_tds_in.sma_tds_adc);
	UART_Debug (dbg);
	return (s_tds_in.sma_tds_adc);
//return s_adc_tds_value;


}

PUBLIC int16_t  ADC_GetAdcTdsOutValue()
{

	return s_tds_out.sma_tds_adc;
//return s_adc_tds_value;


}

PUBLIC uint16_t  ADC_GetTdsValue(TDS_E channel)
{


	float				calculate_value = 0;
	float				slope;
	uint16_t            tds_return = 0;
	uint32_t				index_level = 0;
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	int16_t adc0_value;
	if(channel == TDS_IN_VALUE)
	{
		adc0_value = s_tds_in.sma_tds_adc;
	}
	else if(channel == TDS_OUT_VALUE)
	{
		adc0_value = s_tds_out.sma_tds_adc;
	}

	char dbg[UART_SEND_MAX_LEN];
	sprintf(dbg,"ADC_GetAdcTdsInValue = %d\r\n",adc0_value);
	UART_Debug (dbg);
	if(adc0_value > calib_param->adc_value[0] )
	{
		tds_return = 0;
		return tds_return;
	}

	if(adc0_value < calib_param->adc_value[CALIB_POINT_MAX-1])
	{
		tds_return = calib_param->tds_value[CALIB_POINT_MAX-1];
		return tds_return;
	}

	for(index_level = 0; index_level < (CALIB_POINT_MAX -1) ; index_level++)
	{

		if((adc0_value <= calib_param->adc_value[index_level]) && (adc0_value >= calib_param->adc_value[index_level+1]) )
		{
			slope = ((float)(calib_param->tds_value[index_level+1] - calib_param->tds_value[index_level]))/((float)(calib_param->adc_value[index_level] - calib_param->adc_value[index_level+1]));
			calculate_value = calib_param->tds_value[index_level] + slope * ( calib_param->adc_value[index_level] - adc0_value);
			tds_return= (calculate_value <= 0)?0:(uint16_t) (calculate_value+0.5);
			break;
		}
	}


	return tds_return;

}

PUBLIC uint16_t  ADC_GetTdsValueDisplay(TDS_E channel)
{
	uint16_t            tds_return = 0;
	if(s_interval_get_tds == 0)
	{
		tds_return = ADC_GetTdsValue(channel);
	}
	return tds_return;
}


PUBLIC void   ADC_UpdateTds (uint8_t state)
{
	uint16_t adc_result_tds_in = 0;
	uint16_t adc_result_tds_out = 0;
	int16_t  adc_tds_in = 0;
	int16_t  adc_tds_out = 0;

	s_200ms_cnt = s_200ms_cnt +1;
	if(s_200ms_cnt >= ADC_SAMPLE_CAL_MAX)
	{
		s_200ms_cnt = 0;
		//update tds in
		adc_tds_in = ((s_tds_in.high_cnt == 0)| (s_tds_in.low_cnt==0))?0: \
				((s_tds_in.sum_adc_high/s_tds_in.high_cnt) - (s_tds_in.sum_adc_low/s_tds_in.low_cnt));
		ADC_PushDataToQueue(adc_tds_in,&s_tds_in);

	//	update tds out
		adc_tds_out = ((s_tds_out.high_cnt == 0)| (s_tds_out.low_cnt==0))?0: \
				((s_tds_out.sum_adc_high/s_tds_out.high_cnt) - (s_tds_out.sum_adc_low/s_tds_out.low_cnt));
		ADC_PushDataToQueue(adc_tds_out,&s_tds_out);

		s_tds_in.high_cnt = 0;
		s_tds_in.low_cnt  = 0;
		s_tds_in.sum_adc_high = 0;
		s_tds_in.sum_adc_low  = 0;
		s_tds_out.high_cnt = 0;
		s_tds_out.low_cnt  = 0;
		s_tds_out.sum_adc_high = 0;
		s_tds_out.sum_adc_low  = 0;

		goto end_function;
	}
	R_Config_S12AD0_Get_ValueResult(TDS_IN_CHANNEL, &adc_result_tds_in);
	R_Config_S12AD0_Get_ValueResult(TDS_OUT_CHANNEL,&adc_result_tds_out);
	if(g_pwm_value ==  0)
	{

		s_tds_in.low_cnt ++;
		s_tds_in.sum_adc_low  += adc_result_tds_in;

		s_tds_out.low_cnt ++;
		s_tds_out.sum_adc_low  += adc_result_tds_out;

	}
	else
	{

		s_tds_in.high_cnt ++;
		s_tds_in.sum_adc_high  += adc_result_tds_in;

		s_tds_out.high_cnt ++;
		s_tds_out.sum_adc_high  += adc_result_tds_out;

	}


    end_function:


	g_adc_flag = 0U;
}


PUBLIC ERR_E ADC_SetTdsOutMax(uint16_t value)
{
	if((value > TDS_OUT_VALUE_MIN)&&(value < TDS_OUT_VALUE_MAX))
	{
		s_tds_calib_param.tds_out_max =  value;
		flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));
	   return OK;
	}
	else
	{
		return ERR_PARAM;
	}

}

PUBLIC uint16_t ADC_GetTdsOutMax()
{
	return s_tds_calib_param.tds_out_max;
}


PUBLIC ERR_E ADC_CalibTdsValue(uint16_t tdsvalue,TDS_E channel)
{
   return OK;
}
