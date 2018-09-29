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
extern volatile uint8_t g_pwm_value;  //trạng thái mức xung 1.5khz để xác định mức logic khi đọc adc

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

LOCAL uint16_t s_200ms_cnt ;

LOCAL uint16_t s_adc_h2o_det = 0;

LOCAL uint16_t s_cnt_h2o_det = 0;

LOCAL bool s_is_h2O_det = FALSE;

//LOCAL uint16_t s_adc_low_value = 0;

/******************************************************************************
* Local functions
******************************************************************************/
/*đẩy dữ liệu là 1 giá trị  (độ chênh mức cao và thấp) vào trong queue
 * nếu queue chưa đầy thì chỉ cộng tổng,
 * đầy  thì xóa dequeue rồi mới thêm vào, sau đó tính trung bình các phần tử trong queue
 * để được giá trị ADC cuối cùng(giá trị trước khi được đổi sang tds)
 * */
LOCAL void ADC_PushDataToQueue (int16_t data ,TDS_T* tds)
{
	     int16_t sample = 0;
		if(!QUEUE_QueueIsFull(tds->adc_sample))	//nếu chưa đầy thì mới push vào queue
		{
			tds->sum_tds_adc +=  data;			//cộng thêm vào tổng các giá trị TB
			QUEUE_EnQueue(tds->adc_sample,&data);//đưa vào cuối queue
			tds->sma_tds_adc  = data;
		}
		else
		{

			QUEUE_DeQueue(tds->adc_sample,&sample); //nếu đầy thì xóa node đầu
			tds->sum_tds_adc = (tds->sum_tds_adc) + data - sample;//tổng giá trị TB + giá trị enqueue và - đi giá trị bị dequeue
		//	tds->sign   = (tds->sum_tds_adc >=0 )?0:1;
			tds->sma_tds_adc = (int16_t) tds->sum_tds_adc/((tds->adc_sample)->max_count);//tính TB các giá trị adc TB là giá trị ADC cuối cùng
			QUEUE_EnQueue(tds->adc_sample,&data);		//thêm vào cuối queue node mới vừa xong

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


/*lấy về chỉ số  index_ret là chỉ số trong mảng giá trị TDS trong bảng calib
 * tương ứng với kênh channel và giá trị tds_value cần tìm
 * param: channel : kênh in hay out
 * 		  tds_value: giá trị tds để tìm chỉ số tương ứng trong bảng
 * 		  index_ret: lưu giá trị chỉ số cần tìm
 * return: true nếu thành công,
 * 			false nếu giá trị truyền vào lớn hơn giá trị max
 * có thể giá trị tds_value truyền vào k có trong bảng thì vẫn lấy giá trị gần nhất	 */
LOCAL bool ADC_GetIndexCalibFromTds(TDS_E channel,uint16_t tds_value,uint8_t *index_ret)
{

	uint8_t				ret_index = 0;
	uint8_t				index_level = 0;
	//chọn kênh in hay out
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);

	if(tds_value > calib_param->tds_value[CALIB_POINT_MAX -1]) return false; //nếu giá trị nằm ngoài dải

	//còn nếu trong dải, đâu đó giữa phần tử i và i+1
	//thì kiểm tra nó nằm gần với giá trị cận trên hay cận dưới thì lấy chỉ số tương ứng
	for(index_level = 0; index_level < (CALIB_POINT_MAX -1) ; index_level++)
	{
		if((tds_value >= calib_param->tds_value[index_level]) && (tds_value < calib_param->tds_value[index_level+1]) )
		{
			if((2*tds_value) > (calib_param->tds_value[index_level] + calib_param->tds_value[index_level+1]))
				//lớn hơn trung bình cộng tức là nằm gần cận trên
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

/*khởi tạo các queue cho tds_in và tds_out
 * kích hoạt adc */
PUBLIC void ADC_Init()
{   
	R_Config_S12AD0_Create();	//khởi tạo ADC
	s_tds_in.adc_sample = QUEUE_InitQueue(ADC_SAMPLE_QUEUE_SIZE,sizeof(int16_t)); //tạo queue cho TDS_IN, 8 phần tử
	s_tds_out.adc_sample = QUEUE_InitQueue(ADC_SAMPLE_QUEUE_SIZE,sizeof(int16_t));//------------------OUT
	ADC_InitConfigFlash();		//khởi tạo flash
	s_200ms_cnt = 0;
	PWM = 0;
	g_adc_flag = 0U;
	R_Config_S12AD0_Start();		//kích hoạt adc
	while((QUEUE_QueueIsEmpty(s_tds_out.adc_sample))||((QUEUE_QueueIsEmpty(s_tds_in.adc_sample))))
	{
		//lúc khởi tạo queue trống nên đợi
    	if(g_adc_flag)
    	{
    		ADC_UpdateTds (0);
    	}

	}

}


/*return : giá trị s_tds_in.sma_tds_adc: giá trị ADC sau khi tính toán của kênh tds_in*/

PUBLIC int16_t  ADC_GetAdcTdsInValue()
{
	char dbg[UART_SEND_MAX_LEN];
	sprintf(dbg,"ADC_GetAdcTdsInValue = %d\r\n",s_tds_in.sma_tds_adc);
	UART_Debug (dbg);
	return (s_tds_in.sma_tds_adc);
//return s_adc_tds_value;


}

/*return : giá trị s_tds_out.sma_tds_adc: giá trị ADC sau khi tính toán của kênh tds_in*/

PUBLIC int16_t  ADC_GetAdcTdsOutValue()
{

	return s_tds_out.sma_tds_adc;
//return s_adc_tds_value;


}


/*lấy giá trị tds tương ứng với kênh channel, tức là đi quy đổi ADC-->TDS
 * para: channel: kênh tương ứng TDS_IN hay TDS_OUT
 * return : tds_return : giá trị TDS (giá trị này được nội suy từ bảng giá trị calib trên)*/
PUBLIC uint16_t  ADC_GetTdsValue(TDS_E channel)
{


	float				calculate_value = 0;
	float				slope;
	uint16_t            tds_return = 0;
	uint32_t				index_level = 0;
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	//khai báo con trỏ quản lí bảng calib của 2 kênh in và out ( 2 bảng giá trị quy đổi)
	int16_t adc0_value;
	if(channel == TDS_IN_VALUE)
	{
		adc0_value = s_tds_in.sma_tds_adc; //lấy giá trị trung bình tính được từ số phần tử trong queue
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


	if(adc0_value > calib_param->adc_value[0] ) //adc_value[0] là adc lớn nhất trong bảng ứng với tds =0
	{
		tds_return = 0;
		return tds_return;
	}

	if(adc0_value < calib_param->adc_value[CALIB_POINT_MAX-1])		//adc_value[CALIB_POINT_MAX-1] giá trị adc nhỏ nhất trong bảng ứng với tds lớn nhất
	{
		tds_return = calib_param->tds_value[CALIB_POINT_MAX-1];
		return tds_return;
	}

	//áp dụng công thức nội suy để tính giá trị ứng với giá trị adc0_value : sẽ nằm đâu đó giữa 2 điểm trong bảng giá trị
	//công thức nội suy: x=(x2- x1)/(y2 - y1)*(y - y1) +x1;
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

/*lấy giá trị TDS cần hiển thị đổi với từng kênh*/
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

void debugADC(int16_t adcIn,int16_t adcOut)
{
	char dbg[UART_SEND_MAX_LEN];
	sprintf(dbg,"%d,%d\r\n",adcIn,adcOut);
	UART_Debug (dbg);
}


/*lấy mẫu ADC và tính toán giá trị trung bình lưu vào 1 phần tử trong queue
 * đọc cả ADC đầu phát hiện dò nước
 * nếu adc chuyển đổi xong g_adc_flag =1 thì gọi hàm này
 đọc về giá trị các adc , tính toán cần thiết và xóa cờ để tiếp tục chuyển đổi
 */
PUBLIC void   ADC_UpdateTds (uint8_t state)
{
	uint16_t adc_result_tds_in = 0;
	uint16_t adc_result_tds_out = 0;
	int16_t  adc_tds_in = 0;
	int16_t  adc_tds_out = 0;

	s_200ms_cnt = s_200ms_cnt +1;
//khi chuyển đổi được 500 lần thì tính
	if(s_200ms_cnt >= ADC_SAMPLE_CAL_MAX)
	{
		s_200ms_cnt = 0;
		//update tds in
		adc_tds_in = ((s_tds_in.high_cnt == 0)| (s_tds_in.low_cnt==0))?0: \
				((s_tds_in.sum_adc_high/s_tds_in.high_cnt) - (s_tds_in.sum_adc_low/s_tds_in.low_cnt));
		//nếu đếm cnt high hoặc low còn =0 thì adc_tds_in =0, khác 0 thì adc_tds_in bằng tổng chia đếm để lấy TB
		ADC_PushDataToQueue(adc_tds_in,&s_tds_in); 	//đẩy vào dữ liệu trung bình vào queue

		//	update tds out tương tự tds in
		adc_tds_out = ((s_tds_out.high_cnt == 0)| (s_tds_out.low_cnt==0))?0: \
				((s_tds_out.sum_adc_high/s_tds_out.high_cnt) - (s_tds_out.sum_adc_low/s_tds_out.low_cnt));
		ADC_PushDataToQueue(adc_tds_out,&s_tds_out);

		debugADC(s_tds_in.sma_tds_adc,s_tds_out.sma_tds_adc);  //?
		s_tds_in.high_cnt = 0;
		s_tds_in.low_cnt  = 0;
		s_tds_in.sum_adc_high = 0;
		s_tds_in.sum_adc_low  = 0;
		s_tds_out.high_cnt = 0;
		s_tds_out.low_cnt  = 0;
		s_tds_out.sum_adc_high = 0;
		s_tds_out.sum_adc_low  = 0;
	    //check h2o det
		R_Config_S12AD0_Get_ValueResult(H20_CHANNEL_DETECT,&s_adc_h2o_det);	//đọc về giá trị đầu đo adc dò nước
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
//còn chưa đủ 500 lần thì tiếp tục đọc adc và cộng dồn vào tổng, tăng đếm
	//lấy về giá trị adc 2 đầu cảm biến tds
	R_Config_S12AD0_Get_ValueResult(TDS_IN_CHANNEL, &adc_result_tds_in);
	R_Config_S12AD0_Get_ValueResult(TDS_OUT_CHANNEL,&adc_result_tds_out);


	if(g_pwm_value ==  0)	//nếu xung pwm mức thấp, tính
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


	g_adc_flag = 0U;		//xóa cờ chuyển đổi adc
}

//set giá trị max cho tds_out, trong cấu trúc TDS_CONFIG_T
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

/*lấy giá trị tds_out_max*/
PUBLIC uint16_t ADC_GetTdsOutMax()
{
	return s_tds_calib_param.tds_out_max;
}

/*ứng với giá trị tdsvalue truyền vào thì sẽ tìm vị trí index tương ứng, và thay thế bằng giá trị mới này */
PUBLIC ERR_E ADC_CalibTdsValue(uint16_t tdsvalue,TDS_E channel)
{
	uint8_t index = 0;
	ERR_E f_ret = OK;
	bool ret = false;
	if(ADC_GetIndexCalibFromTds(channel,tdsvalue,&index) == true)		//lấy về vị trí  tương ứng gần nhất với giá trị tdsvalue
	{
		if(channel  == TDS_IN_VALUE)			//kênh in
		{
			s_tds_calib_param.tds_in.tds_value[index] = tdsvalue;			//gán bằng giá trị mới
			s_tds_calib_param.tds_in.adc_value[index] = s_tds_in.sma_tds_adc;//lấy giá trị adc tương ứng với tds
		}
		else if(channel  == TDS_OUT_VALUE)		//kênh out tương tự
		{
			s_tds_calib_param.tds_out.tds_value[index] = tdsvalue;
			s_tds_calib_param.tds_out.adc_value[index] = s_tds_out.sma_tds_adc;
		}
		ret = flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));//lưu vào flash
		f_ret = (ret == true)?OK:ERR;
		return f_ret;
	}
	else return ERR;

}

/*xóa cờ phát hiện dò nươc*/
PUBLIC void ADC_ClearH2oDet()
{
	s_is_h2O_det = 0;
}

PUBLIC bool ADC_GetH2oDet()
{
	return s_is_h2O_det ;
}


/*update giá trị hiển thị lên led 7 thanh. nếu giá trị trả về tds_return sai khác so với giá trị đang hiển thị
 * còn nhỏ hơn  ngưỡng TDS_THRESHOLD trong khoảng thời gian CNT_THRESHOLD_MAX thì giữ nguyên giá trị đang hiển thị
 * (coi như giá trị trả về là nhiễu)
 * nếu hết thời gian CNT_THRESHOLD_MAX dù có vượt hay không vượt ngưỡng thì vẫn hiển thị giá trị mới trả về*/
PUBLIC void ADC_UpdateTdsDisplay()
{
	uint16_t            tds_return = 0;

	//tds_in
	tds_return = ADC_GetTdsValue(TDS_IN_VALUE);//lấy giá trị TDS đã được nội suy
	if((abs(tds_return - s_tds_in.tds_display) > TDS_THRESHOLD) || (s_tds_in.cnt_increase > CNT_THRESHOLD_MAX) || (s_tds_in.cnt_down > CNT_THRESHOLD_MAX))
	{
		s_tds_in.tds_display = tds_return;
		s_tds_in.cnt_down = 0;
		s_tds_in.cnt_increase = 0;
	}
	else
	{
		//tăng biến đếm thời gian, ngưỡng sai khác bên phải hoặc bên trái
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

	//tds out tương tự như tds_in
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

/*lấy bảng giá trị calib (gồm các cặp giá trị tds và adc) của channel tương ứng
 * sau đó gán vào chuỗi out*/
PUBLIC ERR_E ADC_GetCalibTdsParam(TDS_E channel,uint8_t* out)
{
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	for(uint8_t i = 0; i< CALIB_POINT_MAX; i++)
	{
		char calibStr[20]= "";
		sprintf(calibStr,"(%d,%d)",calib_param->tds_value[i],calib_param->adc_value[i]);
		strcat((char*)out,calibStr);    //nối thêm chuỗi calibStr vào chuỗi out
		//cụ thể ở đây mỗi lần tăng i thì lấy thêm được 1 cặp giá trị TDS và ADC gán thêm vào 1 chuỗi out
	}
	return OK;
}

/*chỉ lấy bảng giá trị adc trong bảng calib của kênh channel tương ứng*/
PUBLIC ERR_E ADC_GetAdcTable(TDS_E channel,uint8_t* out)
{
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	for(uint8_t i = 0; i< CALIB_POINT_MAX; i++)
	{
		char calibStr[20]= "";
		if(i == (CALIB_POINT_MAX - 1))
			sprintf(calibStr,"%d",calib_param->adc_value[i]);
		else
			sprintf(calibStr,"%d,",calib_param->adc_value[i]);  //chưa đến cuối thì thêm dấu "," để ngăn cách

		strcat((char*)out,calibStr);
	}
	return OK;
}

/*tương tự như lấy giá trị adc trong hàm ADC_GetAdcTable() trên*/
PUBLIC ERR_E ADC_GetTdsTable(TDS_E channel,uint8_t* out)
{
	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	for(uint8_t i = 0; i< CALIB_POINT_MAX; i++)
	{
		char calibStr[20]= "";
		if(i == (CALIB_POINT_MAX - 1))
			sprintf(calibStr,"%d",calib_param->tds_value[i]);
		else
			sprintf(calibStr,"%d,",calib_param->tds_value[i]);
		strcat((char*)out,calibStr);
	}
	return OK;
}


/*set bảng giá trị bằng UART */
PUBLIC ERR_E ADC_CalibTdsValueFromUart(uint16_t tdsvalue,TDS_E channel,uint8_t index)
{
	ERR_E f_ret = OK;
	bool ret = false;
	if(index > (CALIB_POINT_MAX -1)) //check lỗi truyền sai giá trị index
	{
		return ERR;
	}
	else
	{
		if(channel  == TDS_IN_VALUE)		//kênh in
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
