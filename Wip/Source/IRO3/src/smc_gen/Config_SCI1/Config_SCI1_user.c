/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2016, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : Config_SCI1_user.c
* Version      : 1.2.0
* Device(s)    : R5F51303AxFM
* Description  : This file implements device driver for Config_SCI1.
* Creation Date: 2018-09-19
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_SCI1.h"
/* Start user code for include. Do not edit comment generated here */
#include "queue.h"
#include "uart.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_sci1_tx_address;                /* SCI1 transmit buffer address */
extern volatile uint16_t  g_sci1_tx_count;                   /* SCI1 transmit data number */
extern volatile uint8_t * gp_sci1_rx_address;                /* SCI1 receive buffer address */
extern volatile uint16_t  g_sci1_rx_count;                   /* SCI1 receive data number */
extern volatile uint16_t  g_sci1_rx_length;                  /* SCI1 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* Global variable used to receive a character from PC terminal */
volatile uint8_t g_rx_char;
/* Flag used to control transmission to PC terminal */
volatile uint8_t g_tx_flag = 0U;
/* Flag used to detect whether data is received */
volatile uint8_t g_rx_flag;
/* Flag used to detect completion of transmission */
static volatile uint8_t SCI1_txdone;

static QUEUE_NODE_T* s_uart_data_queue ;

LOCAL volatile uint16_t  s_rev_index = 0;

LOCAL  uint16_t  s_rev_index_pre = 0;

LOCAL volatile bool  s_rev_done = false;
/* Sends SCI1 data and waits for transmit end flag */
MD_STATUS R_SCI1_AsyncTransmit(uint8_t * const tx_buf, const uint16_t
tx_num);
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_SCI1_Create_UserInit
* Description  : This function adds user code after initializing the SCI1 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_SCI1_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
	s_uart_data_queue =  QUEUE_InitQueue(MAX_QUEUE_DATA_UART,sizeof(uint8_t));		//tạo 1 queue chứa  MAX_QUEUE_DATA_UART kí tự

    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI1_transmit_interrupt
* Description  : This function is TXI1 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_SCI1_TXI1
#pragma interrupt r_Config_SCI1_transmit_interrupt(vect=VECT(SCI1,TXI1),fint)
#else
#pragma interrupt r_Config_SCI1_transmit_interrupt(vect=VECT(SCI1,TXI1))
#endif
static void r_Config_SCI1_transmit_interrupt(void)
{
    if (0U < g_sci1_tx_count)
    {
        SCI1.TDR = *gp_sci1_tx_address;
        gp_sci1_tx_address++;
        g_sci1_tx_count--;
    }
    else
    {
        SCI1.SCR.BIT.TIE = 0U;
        SCI1.SCR.BIT.TEIE = 1U;
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI1_transmitend_interrupt
* Description  : This function is TEI1 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_SCI1_TEI1
#pragma interrupt r_Config_SCI1_transmitend_interrupt(vect=VECT(SCI1,TEI1),fint)
#else
#pragma interrupt r_Config_SCI1_transmitend_interrupt(vect=VECT(SCI1,TEI1))
#endif
static void r_Config_SCI1_transmitend_interrupt(void)
{
    PORT2.PMR.BYTE &= 0xBFU;
    SCI1.SCR.BIT.TIE = 0U;
    SCI1.SCR.BIT.TE = 0U;
    SCI1.SCR.BIT.TEIE = 0U;

    r_Config_SCI1_callback_transmitend();
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI1_receive_interrupt
* Description  : This function is RXI1 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_SCI1_RXI1
#pragma interrupt r_Config_SCI1_receive_interrupt(vect=VECT(SCI1,RXI1),fint)
#else
#pragma interrupt r_Config_SCI1_receive_interrupt(vect=VECT(SCI1,RXI1))
#endif
static void r_Config_SCI1_receive_interrupt(void)
{
    if (g_sci1_rx_length > g_sci1_rx_count)
    {
        *gp_sci1_rx_address = SCI1.RDR;
        gp_sci1_rx_address++;
        g_sci1_rx_count++;

        if (g_sci1_rx_length <= g_sci1_rx_count)
        {
            r_Config_SCI1_callback_receiveend();
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI1_receiveerror_interrupt
* Description  : This function is ERI1 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_SCI1_ERI1
#pragma interrupt r_Config_SCI1_receiveerror_interrupt(vect=VECT(SCI1,ERI1),fint)
#else
#pragma interrupt r_Config_SCI1_receiveerror_interrupt(vect=VECT(SCI1,ERI1))
#endif
static void r_Config_SCI1_receiveerror_interrupt(void)
{
    uint8_t err_type;

    r_Config_SCI1_callback_receiveerror();

    /* Clear overrun, framing and parity error flags */
    err_type = SCI1.SSR.BYTE;
    err_type &= 0xC7U;
    err_type |= 0xC0U;
    SCI1.SSR.BYTE = err_type;
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI1_callback_transmitend
* Description  : This function is a callback function when SCI1 finishes transmission
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI1_callback_transmitend(void)
{
    /* Start user code for r_Config_SCI1_callback_transmitend. Do not edit comment generated here */
	SCI1_txdone = 1U;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI1_callback_receiveend
* Description  : This function is a callback function when SCI1 finishes reception
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI1_callback_receiveend(void)
{
    /* Start user code for r_Config_SCI1_callback_receiveend. Do not edit comment generated here */
	g_rx_flag = 1U;
	/* Set SCI1 receive buffer address and restart reception */
	R_Config_SCI1_Serial_Receive((uint8_t *)&g_rx_char, 1);
	QUEUE_EnQueue(s_uart_data_queue,&g_rx_char);
	//đẩy kí tự nhận được vào trong queue,nếu số kí tự > MAX_QUEUE_DATA_UART thì k đẩy vào đc(xem lại queue.c)
//	s_rev_index_pre = s_rev_index;
	s_rev_index = s_rev_index+1;			//đếm số byte nhận được từ uart
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_SCI1_callback_receiveerror
* Description  : This function is a callback function when SCI1 reception encounters error
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

static void r_Config_SCI1_callback_receiveerror(void)
{
    /* Start user code for r_Config_SCI1_callback_receiveerror. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
MD_STATUS R_SCI1_AsyncTransmit (uint8_t * const tx_buf, const uint16_t
tx_num)
{
 MD_STATUS status = MD_OK;

 /* Clear transmit completion flag before transmission */
 SCI1_txdone = 0U;
/* Set SCI1 transmit buffer address and start transmission */
 status = R_Config_SCI1_Serial_Send(tx_buf, tx_num);
 /* Wait for transmit end flag */
 while (0U == SCI1_txdone)
 {
 /* Wait */
 }
 return (status);
}
/**********************************************************************
*********
* End of function R_SCI2_AsyncTransmit
*****************************************************************
**************/

/*đọc tối đa maxlen byte dữ liệu từ queue vào data
 * return : ret: số byte thực tế nhận được từ queue*/
PUBLIC uint16_t  UART_ReadData(uint8_t * data,uint16_t maxlen)
{	// ở đây data[30] nên maxlen = MAX_QUEUE_DATA_UART :số byte max trong queue
	uint16_t ret = 0;
	if(data  == NULL)
	{
	 return ret;
	}

	while(!QUEUE_QueueIsEmpty(s_uart_data_queue))		//nếu queue chưa rỗng
	{
	    QUEUE_DeQueue(s_uart_data_queue,data+ret);
		ret ++;											//số byte trong queue ret<maxlen
		if(ret > maxlen) return ret;					//đọc đủ maxlen nhưng trong queue vẫn còn dữ liệu
	}

	//nếu đọc <= maxlen byte, tức là đọc hết dữ liệu trong queue
	//xóa bộ đếm
	s_rev_index = 0;
	s_rev_index_pre = 0;
	s_rev_done = false;
	g_rx_flag = 0U;
	return ret;
}


// timer 1 ms check if
//kiểm tra nhận xong
PUBLIC void UART_CheckDataReadDonePacket (void )
{
	if((s_rev_index == s_rev_index_pre) && (g_rx_flag))
	{
		s_rev_done = true;  			//uart nhận xong khi mà s_rev_index k thay đổi nữa
	}
	s_rev_index_pre = s_rev_index;
}


PUBLIC bool UART_IsDoneFrame(void)
{
	return s_rev_done;
}
/* End user code. Do not edit comment generated here */   




