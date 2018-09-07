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
		.adc_value = {2670,2677,2687,2704,2718,2734,2746,2760,2771,2784,2795,2807,2871,2918,2958,2992,3019,3043,3067,3149,4000},
		.tds_value = {0,6,10,20,30,40,50,60,70,80,90,100,170,240,310,380,450,520,600,1000,2000}

};


LOCAL const TDS_CALIB_PARAM_T TDS_OUT_CONFIG_DEFAULD =
{
		.adc_value = {2670,2677,2687,2704,2718,2734,2746,2760,2771,2784,2795,2807,2871,2918,2958,2992,3019,3043,3067,3149,3261},
		.tds_value = {0,6,10,20,30,40,50,60,70,80,90,100,170,240,310,380,450,520,600,1000,2000}

};


LOCAL TDS_CONFIG_T s_tds_calib_param;

LOCAL TDS_T  s_tds_in;

LOCAL TDS_T  s_tds_out;

LOCAL uint16_t s_200ms_cnt ;

LOCAL uint16_t s_adc_h2o_det = 0;

LOCAL uint16_t s_cnt_h2o_det = 0;

LOCAL bool s_is_h2O_det = FALSE;

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
    	s_tds_calib_param.tds_out_max    = TDS_OUT_MAX_DEFAULT;
    	s_tds_calib_param.adc_h2o_det = ADC_H2O_DET_DEFAULT;
    	flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));
    }


}

LOCAL bool ADC_GetIndexCalibFromTds(TDS_E channel,uint16_t tds_value,uint8_t *index_ret)
{

	uint8_t				ret_index = 0;
	uint8_t				index_level = 0;
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);

	if(tds_value > calib_param->tds_value[CALIB_POINT_MAX -1]) return false;
	for(index_level = 0; index_level < (CALIB_POINT_MAX -1) ; index_level++)
	{
		if((tds_value >= calib_param->tds_value[index_level]) && (tds_value < calib_param->tds_value[index_level+1]) )
		{
			if((2*tds_value) > (calib_param->tds_value[index_level] + calib_param->tds_value[index_level+1]))
				ret_index = index_level+1;
			else ret_index = index_level;
			break;
		}
	}
	*index_ret = ret_index;
	return true;
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
	R_Config_S12AD0_Create();
	s_tds_in.adc_sample = QUEUE_InitQueue(ADC_SAMPLE_QUEUE_SIZE,sizeof(int16_t));
	s_tds_out.adc_sample = QUEUE_InitQueue(ADC_SAMPLE_QUEUE_SIZE,sizeof(int16_t));
	ADC_InitConfigFlash();
	s_200ms_cnt = 0;
	PWM = 0;
	g_adc_flag = 0U;
	R_Config_S12AD0_Start();
	while((QUEUE_QueueIsEmpty(s_tds_out.adc_sample))||((QUEUE_QueueIsEmpty(s_tds_in.adc_sample))))
	{
    	if(g_adc_flag)
    	{
    		ADC_UpdateTds (0);
    	}

	}

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
//		char dbg[UART_SEND_MAX_LEN];
//		sprintf(dbg," %d,",adc0_value);
//		UART_Debug (dbg);
	}
	else if(channel == TDS_OUT_VALUE)
	{
		adc0_value = s_tds_out.sma_tds_adc;
//		char dbg[UART_SEND_MAX_LEN];
//		sprintf(dbg,"%d\r\n",adc0_value);
//		UART_Debug (dbg);
	}


	if(adc0_value < calib_param->adc_value[0] )
	{
		tds_return = 0;
		return tds_return;
	}

	if(adc0_value > calib_param->adc_value[CALIB_POINT_MAX-1])
	{
		tds_return = calib_param->tds_value[CALIB_POINT_MAX-1];
		return tds_return;
	}

	for(index_level = 0; index_level < (CALIB_POINT_MAX -1) ; index_level++)
	{

		if((adc0_value >= calib_param->adc_value[index_level]) && (adc0_value <= calib_param->adc_value[index_level+1]) )
		{
			slope = ((float)(calib_param->tds_value[index_level+1] - calib_param->tds_value[index_level]))/((float)(calib_param->adc_value[index_level+1] - calib_param->adc_value[index_level]));
			calculate_value = calib_param->tds_value[index_level] + slope * (adc0_value - calib_param->adc_value[index_level]);
			tds_return= (calculate_value <= 0)?0:(uint16_t) (calculate_value+0.5);
			break;
		}
	}

	return tds_return;

}

PUBLIC uint16_t  ADC_GetTdsValueDisplay(TDS_E channel)
{

	if(channel == TDS_IN_VALUE)
	{
		return s_tds_in.tds_display;
	}
	else
	{
		return s_tds_out.tds_display;
	}

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
		adc_tds_in = (s_tds_in.high_cnt == 0)?0: \
				(s_tds_in.sum_adc_high/s_tds_in.high_cnt);
		ADC_PushDataToQueue(adc_tds_in,&s_tds_in);

	//	update tds out
		adc_tds_out = (s_tds_out.high_cnt == 0)?0: \
				(s_tds_out.sum_adc_high/s_tds_out.high_cnt);
		ADC_PushDataToQueue(adc_tds_out,&s_tds_out);

		char dbg[UART_SEND_MAX_LEN];
		sprintf(dbg," %d,",(s_tds_in.sma_tds_adc));
		UART_Debug (dbg);

		sprintf(dbg," %d\r\n",(s_tds_out.sma_tds_adc));
		UART_Debug (dbg);

		s_tds_in.high_cnt = 0;
		s_tds_in.low_cnt  = 0;
		s_tds_in.sum_adc_high = 0;
		s_tds_in.sum_adc_low  = 0;
		s_tds_out.high_cnt = 0;
		s_tds_out.low_cnt  = 0;
		s_tds_out.sum_adc_high = 0;
		s_tds_out.sum_adc_low  = 0;
	    //check h2o det
		R_Config_S12AD0_Get_ValueResult(H20_CHANNEL_DETECT,&s_adc_h2o_det);
		if(s_adc_h2o_det < s_tds_calib_param.adc_h2o_det)
		{
			s_cnt_h2o_det ++;
			if(s_cnt_h2o_det> H2O_DET_CNT_MAX)
			{
				s_is_h2O_det =TRUE;
				s_cnt_h2o_det = 0;
			}
		}
		else
		{
			s_cnt_h2o_det = 0;
		}
		goto end_function;
	}
	R_Config_S12AD0_Get_ValueResult(TDS_IN_CHANNEL, &adc_result_tds_in);
	R_Config_S12AD0_Get_ValueResult(TDS_OUT_CHANNEL,&adc_result_tds_out);
		s_tds_in.high_cnt ++;
		s_tds_in.sum_adc_high  += adc_result_tds_in;

		s_tds_out.high_cnt ++;
		s_tds_out.sum_adc_high  += adc_result_tds_out;


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
	uint8_t index = 0;
	ERR_E f_ret = OK;
	bool ret = false;
	if(ADC_GetIndexCalibFromTds(channel,tdsvalue,&index) == true)
	{
		if(channel  == TDS_IN_VALUE)
		{
			s_tds_calib_param.tds_in.tds_value[index] = tdsvalue;
			s_tds_calib_param.tds_in.adc_value[index] = s_tds_in.sma_tds_adc;
		}
		else if(channel  == TDS_OUT_VALUE)
		{
			s_tds_calib_param.tds_out.tds_value[index] = tdsvalue;
			s_tds_calib_param.tds_out.adc_value[index] = s_tds_out.sma_tds_adc;
		}
		ret = flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));
		f_ret = (ret == true)?OK:ERR;
		return f_ret;
	}
	else return ERR;

}


PUBLIC void ADC_ClearH2oDet()
{
	s_is_h2O_det = 0;
}

PUBLIC bool ADC_GetH2oDet()
{
	return s_is_h2O_det ;
}

PUBLIC void ADC_UpdateTdsDisplay()
{
	uint16_t            tds_return = 0;

	tds_return = ADC_GetTdsValue(TDS_IN_VALUE);
	if((abs(tds_return - s_tds_in.tds_display) > TDS_THRESHOLD) || (s_tds_in.cnt_increase > CNT_THRESHOLD_MAX) || (s_tds_in.cnt_down > CNT_THRESHOLD_MAX))
	{
		s_tds_in.tds_display = tds_return;
		s_tds_in.cnt_down = 0;
		s_tds_in.cnt_increase = 0;
	}
	else
	{
		if(s_tds_in.tds_display > tds_return)
		{
			s_tds_in.cnt_down  = s_tds_in.cnt_down+1;
		}
		else if(s_tds_in.tds_display < tds_return)
		{
			s_tds_in.cnt_increase = s_tds_in.cnt_increase+1;
		}
		else
		{
			s_tds_in.cnt_down = 0;
			s_tds_in.cnt_increase = 0;
		}

	}

	//tds out
	tds_return = ADC_GetTdsValue(TDS_OUT_VALUE);
		if((abs(tds_return - s_tds_out.tds_display) > TDS_THRESHOLD) || (s_tds_out.cnt_increase > CNT_THRESHOLD_MAX) || (s_tds_out.cnt_down > CNT_THRESHOLD_MAX))
		{
			s_tds_out.tds_display = tds_return;
			s_tds_out.cnt_down = 0;
			s_tds_out.cnt_increase = 0;
		}
		else
		{
			if(s_tds_out.tds_display > tds_return)
			{
				s_tds_out.cnt_down  = s_tds_out.tds_display+1;
			}
			else if(s_tds_out.tds_display < tds_return)
			{
				s_tds_out.cnt_increase = s_tds_out.cnt_increase+1;
			}
			else
			{
				s_tds_out.cnt_down = 0;
				s_tds_out.cnt_increase = 0;
			}

		}


}


PUBLIC ERR_E ADC_GetCalibTdsParam(TDS_E channel,uint8_t* out)
{
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	for(uint8_t i = 0; i< CALIB_POINT_MAX; i++)
	{
		char calibStr[20]= "";
		sprintf(calibStr,"(%d,%d)",calib_param->tds_value[i],calib_param->adc_value[i]);
		strcat((char*)out,calibStr);
	}
//	sprintf(out,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",calib_param->tds_value[0],calib_param->tds_value[1],calib_param->tds_value[2],calib_param->tds_value[3],calib_param->tds_value[4],calib_param->tds_value[5] \
//			,calib_param->tds_value[6],calib_param->tds_value[7],calib_param->tds_value[8],calib_param->tds_value[9],calib_param->tds_value[10],calib_param->tds_value[11],calib_param->adc_value[0],calib_param->adc_value[1],calib_param->adc_value[2],calib_param->adc_value[3],calib_param->adc_value[4],calib_param->adc_value[5] \
//			,calib_param->adc_value[6],calib_param->adc_value[7],calib_param->adc_value[8],calib_param->adc_value[9],calib_param->adc_value[10],calib_param->adc_value[11]
//			);
	return OK;
}

PUBLIC ERR_E ADC_CalibTdsValueFromUart(uint16_t tdsvalue,TDS_E channel,uint8_t index)
{
	ERR_E f_ret = OK;
	bool ret = false;
	if(index > (CALIB_POINT_MAX -1))
	{
		return ERR;
	}
	else
	{
		if(channel  == TDS_IN_VALUE)
		{
			s_tds_calib_param.tds_in.tds_value[index] = tdsvalue;
			s_tds_calib_param.tds_in.adc_value[index] = (index == 0)?(s_tds_in.sma_tds_adc - 10) :s_tds_in.sma_tds_adc;
		}
		else if(channel  == TDS_OUT_VALUE)
		{
			s_tds_calib_param.tds_out.tds_value[index] = tdsvalue;
			s_tds_calib_param.tds_out.adc_value[index] = (index == 0)?(s_tds_out.sma_tds_adc -10):s_tds_out.sma_tds_adc;
		}
		ret = flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));
		f_ret = (ret == true)?OK:ERR;
		return f_ret;
	}
}
