/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        uart_drv.c
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


#include <uart.h>

/******************************************************************************
* External objects
******************************************************************************/

///* Global variable for changing CMT0 interval */
//volatile uint16_t interval_level = 1;
///* String used to print message at PC terminal */
//static char print_str[250];
///* Flag used to detect whether data is received from PC terminal */
//extern volatile uint8_t g_rx_flag;
///* Global variable used for storing data received from PC terminal */
//extern volatile uint8_t g_rx_char;
///* Sends SCI2 data and waits for transmit end flag. */


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
typedef ERR_E(* PACKET_FUNC_PTR)(uint8_t* input,uint8_t* out );

/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *
 * Detailed explanation.
 */
typedef struct parse_cmd_process_tag
{
	uint8_t					msg_id;
	const uint8_t				packet_str[COMMAND_MAX_LEN];
    PACKET_FUNC_PTR			packet_func_ptr;
} PARSE_PACKET_PROCESS_T;

/******************************************************************************
* Local function prototypes
******************************************************************************/
/* Sends SCI2 data and waits for transmit end flag. */
MD_STATUS R_SCI1_AsyncTransmit(uint8_t * const tx_buf, const uint16_t
tx_num);

LOCAL ERR_E UART_HandleCalibTds(uint8_t* value,uint8_t* out );

LOCAL ERR_E UART_HandleSoftVer(uint8_t* value,uint8_t* out );

LOCAL ERR_E UART_HandleDbg(uint8_t* value,uint8_t* out );

LOCAL ERR_E UART_HandleTdsParam(uint8_t* value,uint8_t* out );

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
* Local variables
******************************************************************************/

/* String used to print message at PC terminal */
static char print_str[UART_SEND_MAX_LEN];

/* Global variable used for storing data received from PC terminal */
extern volatile uint8_t g_rx_char;

LOCAL bool s_dbg_en = FALSE;

LOCAL const PARSE_PACKET_PROCESS_T packet_process_table[] =
{


	{1,"CALIB_TDS",UART_HandleCalibTds},
	{2,"SOFTWAREVER",UART_HandleSoftVer},
	{3,"DEBUG_EN",UART_HandleDbg},
	{4,"TDS_PARAM",UART_HandleTdsParam},
	{5,"RESET",NULL}


};

/******************************************************************************
* Local functions
******************************************************************************/

LOCAL void UART_Reply(uint8_t * data,uint8_t* msg_id,ERR_E err_code)
{
	sprintf(print_str,"[%s,%d,%s]",msg_id,err_code,data);
	R_SCI1_AsyncTransmit((uint8_t *)print_str, (uint16_t)strlen(print_str));

}


LOCAL ERR_E UART_HandleSoftVer (uint8* value,uint8* out )
{

	strcpy((char*)out,(char*)VERSION_SOFTWARE);
	return OK;

}


LOCAL ERR_E UART_HandleDbg (uint8* value,uint8* out )
{

	uint8_t  param[9] = {0};
	int param_int = 0;
	if(strlen ((char*)value) > 8)
	{

		return ERR_PARAM;
	}

	strcpy((char*)param,(char*)value);
	param_int = atoi((char*)param);
	s_dbg_en = (param_int == 1)?1:0;
	return OK;

}

LOCAL ERR_E UART_HandleTdsParam (uint8* value,uint8* out )
{

	uint8_t  param[9] = {0};
	int param_int = 0;
	TDS_E channel_tds = TDS_IN_VALUE;
	if(strlen ((char*)value) > 8)
	{

		return ERR_PARAM;
	}
    //in 0: out 1

	strcpy((char*)param,(char*)value);
	param_int = atoi((char*)param);
	channel_tds =  (param_int == 0)?TDS_IN_VALUE:TDS_OUT_VALUE;
	ADC_GetCalibTdsParam(channel_tds,out);
	return OK;

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
LOCAL ERR_E UART_HandleCalibTds (uint8* value,uint8* out )
{
	uint8 data[20] = {0};
		//parser data in uart then handle
		ERR_E ret = OK;
		uint8_t	*msg_field[MSG_FIELDS_MAX];

		uint8_t index = 0;
		uint8_t	*msg_token;
		TDS_E channel_tds = TDS_IN_VALUE;

		// generic indexers
		int 	i;
		int   tds = 0;
	//	len  = MIN(19,strlen_s((char*)value));
		strcpy(data,value);
		memset( msg_field, 0x00, sizeof(msg_field));
		int channel;
		i				= 0;
		msg_token		= data;
		msg_field[i]	= msg_token;

		while (*msg_token != '\0')
		{


			if ( *msg_token == ':' )
			{

				// terminate string after field separator or end-of-message characters
				*msg_token = '\0';
				// save position of the next token
				msg_field[++i] = msg_token + 1;


			}
			msg_token++;

		}
		if(i<2) return  ERR;
		//0:1:2 mean chanel_in:index1:tds2
		channel = atoi((char*)msg_field[0]);
		channel_tds =  (channel == 0)?TDS_IN_VALUE:TDS_OUT_VALUE;
		index = atoi((char*)msg_field[1]);
		tds   =  atoi((char*)msg_field[2]);
		return	ADC_CalibTdsValueFromUart(tds,channel_tds,index);
//	return OK;

}

LOCAL void UART_HandleProcess(uint8 * data,uint16_t len)
{

	uint8_t	*msg_field[MSG_FIELDS_MAX];
	ERR_E ret = NOT_SUPPORT;
	// pointer used in NMEA message tokenizer
	uint8_t	*msg_token;
	uint8_t data_reply[UART_SEND_MAX_LEN] = {0};
	// generic indexers
	int 	i, j;
	if(*data != MSG_FIELD_SOF) goto end_handle;
	memset( msg_field, 0x00, sizeof(msg_field));

	i				= 0;
	msg_token		= data+1;
	msg_field[i]	= msg_token;

	while (*msg_token != '\0')
	{

		if (( *msg_token == MSG_FIELD_SEPARATOR ) | ( *msg_token == MSG_FIELD_EOF ))
		{

		// terminate string after field separator or end-of-message characters
		*msg_token = '\0';

		// save position of the next token
		msg_field[++i] = msg_token + 1;

		}

		msg_token++;

	}

	for( j = 0; j < ARR_SIZE(packet_process_table); j++)
	{

		if(	strcmp((char*) packet_process_table[j].packet_str,(char*) msg_field[MSG_FIELD_TYPE]) == 0)
		{
			ret = packet_process_table[j].packet_func_ptr((uint8_t*)msg_field[MSG_FIELD_DATA],(uint8_t*)data_reply );
			break;
		}

	}

	end_handle:
	UART_Reply(data_reply,(uint8_t*)packet_process_table[j].packet_str,ret);
	return;


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
PUBLIC void UART_Init(void)
{
	//init UART here
	R_Config_SCI1_Create();
	/* Set SCI2 receive buffer address and enable receive interrupt */
	R_Config_SCI1_Serial_Receive((uint8_t *)&g_rx_char, 1);
	/* Enable SCI2 operation */
	R_Config_SCI1_Start();

	sprintf(print_str, "UART init done\r\n");
	R_SCI1_AsyncTransmit((uint8_t *)print_str, (uint16_t)strlen(print_str));
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
PUBLIC void UART_UartPuts (uint8_t *s)
{
	sprintf(print_str,"%s", s);
	R_SCI1_AsyncTransmit((uint8_t *)print_str, (uint16_t)strlen(print_str));
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
PUBLIC void UART_Debug(uint8_t * data)
{  
//	#ifdef DEBUG_ENABLE
	if(s_dbg_en)
	{
		UART_UartPuts(data);
	}
//	#endif
}

PUBLIC void UART_Process()
{
	uint8_t data[30] ={0};
	uint16_t len = 0;
	if(UART_IsDoneFrame())
	{
		len = UART_ReadData(data,30);
		UART_HandleProcess(data,len);
	}
}


