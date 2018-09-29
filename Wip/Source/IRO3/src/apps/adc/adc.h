
/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         Adc.h
 *
 * @author    	trongkn
 * 
 * @version   1.0
 * 
 * @date      
 * 
 * @brief     Brief description of the file
 *
 * Detailed Description of the file. If not used, remove the separator above.
 *
 */

#ifndef ADC_H_
#define ADC_H_

/******************************************************************************
* Includes
******************************************************************************/


#include <config.h>
#include "r_cg_s12ad.h"
#include <queue.h>
#include "r_cg_userdefine.h"
#include "flash_app.h"
#include "uart.h"
/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros 
******************************************************************************/

#define  ADC_SAMPLE_QUEUE_SIZE			(8)	//số lượng giá trị ADC(trung bình)(ADC_H - ADC_L) lưu trong queue
#define  TDS_OUT_CHANNEL    ADCHANNEL0		//kênh cảm biến TDS_OUT sử dụng pin P40, kênh ADC AN000
#define  TDS_IN_CHANNEL     ADCHANNEL1		//--------------TDS_IN--------------P41, ---------AN001
#define  H20_CHANNEL_DETECT   ADCHANNEL20	//kênh cảm biến dò nước ------------PE4, ---------AN020
#define  ADC_SAMPLE_CAL_MAX         (500)	//số lần đọc ADC để tính trung bình ADC

#define UPDATE_TDS_IN (0)
#define UPDATE_TDS_OUT (1)
#define UPDATE_VALUE   (2)
#define TDS_OUT_VALUE_MIN  (0)
#define TDS_OUT_MAX_DEFAULT (100)
#define ADC_H2O_DET_DEFAULT (4000)
#define H2O_DET_CNT_MAX   (20)
#define TDS_OUT_VALUE_MAX  (1000)
#define CALIB_POINT_MAX (21)
#define TDS_THRESHOLD (1)			//
#define CNT_THRESHOLD_MAX (10)
/******************************************************************************
* Types
******************************************************************************/

/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *
 * Detailed explanation.
 */
	
typedef struct
{
	uint16_t high_cnt;			/*đếm max là ADC_SAMPLE_CAL_MAX*/
	uint16_t low_cnt;
	int32_t  sum_adc_high;		//tổng các giá trị ADC mức cao
	int32_t  sum_adc_low;		//tổng giá trị ADC mức thấp
	int16_t  sma_tds_adc;		//giá trị ADC trung bình (= sum_tds_adc/ADC_SAMPLE_QUEUE_SIZE) là giá trị trước khi được quy đổi sang tds
	int32_t  sum_tds_adc;		//= tổng ADC_SAMPLE_QUEUE_SIZE giá trị (sum_adc_high/high_cnt)-(sum_adc_low/low_cnt)
	uint16_t tds_display;		//giá trị hiển thị thực tế
	uint8_t  cnt_down;			//biến đếm thời gian để xác định có cho update hiển thị giá trị tds mới hay k
	uint8_t cnt_increase;		//
	QUEUE_NODE_T* adc_sample;	//tạo 1 queue cho kênh TDS lưu các giá trị ADC_H - ADC_L trung bình

}TDS_T;							//tất cả thuộc tính liên quan đến 1 kênh TDS



typedef struct  {
	int16_t adc_value[CALIB_POINT_MAX];			//
	uint16_t tds_value[CALIB_POINT_MAX];		//
} TDS_CALIB_PARAM_T;							//bảng giá trị quy đổi


typedef struct
{
	TDS_CALIB_PARAM_T tds_in;	//
	TDS_CALIB_PARAM_T tds_out;		//
	uint16_t  tds_out_max;
	uint16_t  adc_h2o_det;
} TDS_CONFIG_T;

typedef enum
{
	TDS_IN_VALUE = 0,	//
	TDS_OUT_VALUE =1,		//
	TDS_VALUE_MAX
} TDS_E;
/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
PUBLIC void	ADC_Init(void);							//

PUBLIC int16_t  ADC_GetAdcTdsOutValue();			//

PUBLIC int16_t  ADC_GetAdcTdsInValue();				//

PUBLIC uint16_t  ADC_GetTdsValue(TDS_E channel);	//

PUBLIC void   ADC_UpdateTds (uint8_t state);		//

PUBLIC ERR_E ADC_SetTdsOutMax(uint16_t value);

PUBLIC uint16_t ADC_GetTdsOutMax();

PUBLIC ERR_E ADC_CalibTdsValue(uint16_t tdsvalue,TDS_E channel);

PUBLIC void ADC_UpdateTdsDisplay();					//

PUBLIC uint16_t  ADC_GetTdsValueDisplay(TDS_E channel);

PUBLIC ERR_E ADC_CalibTdsValueFromUart(uint16_t tdsvalue,TDS_E channel,uint8_t index);

PUBLIC bool ADC_GetH2oDet();

PUBLIC ERR_E ADC_GetAdcTable(TDS_E channel,uint8_t* out);

PUBLIC ERR_E ADC_GetTdsTable(TDS_E channel,uint8_t* out);


/******************************************************************************
* Inline functions
******************************************************************************/



#endif 


