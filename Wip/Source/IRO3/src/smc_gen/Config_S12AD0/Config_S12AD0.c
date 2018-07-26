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
* File Name    : Config_S12AD0.c
* Version      : 1.3.0
* Device(s)    : R5F51303AxFM
* Description  : This file implements device driver for Config_S12AD0.
* Creation Date: 2018-07-26
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
#include "Config_S12AD0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_S12AD0_Create
* Description  : This function initializes the S12AD0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD0_Create(void)
{
    /* Cancel S12AD0 module stop state */
    MSTP(S12AD) = 0U;  

    /* Disable and clear interrupt flags of S12AD0 module */
    S12AD.ADCSR.BIT.ADIE = 0U;
    IEN(S12AD, S12ADI0) = 0U;

    /* Set S12AD0 control registers */
    S12AD.ADDISCR.BYTE = _00_AD_DISCONECT_UNUSED;
    S12AD.ADCSR.WORD = _0000_AD_DBLTRIGGER_DISABLE | _0000_AD_SYNCASYNCTRG_DISABLE | 
                       _1000_AD_SCAN_END_INTERRUPT_ENABLE | _0000_AD_SINGLE_SCAN_MODE;
    S12AD.ADHVREFCNT.BYTE |= _00_AD_HIGH_POTENTIAL_AVCC0 | _00_AD_LOW_POTENTIAL_AVSS0;
    S12AD.ADBUFEN.BYTE |= _00_AD_STORAGE_BUFF_UNUSED;
    S12AD.ADCER.WORD = _0000_AD_AUTO_CLEARING_DISABLE | _0000_AD_SELFTDIAGST_DISABLE | _0000_AD_RIGHT_ALIGNMENT;
    S12AD.ADADC.BYTE = _00_AD_1_TIME_CONVERSION | _00_AD_ADDITION_MODE;

    /* Set channels and sampling time */
    S12AD.ADANSA0.WORD = _0001_AD_AN000_USED | _0002_AD_AN001_USED;
    S12AD.ADSSTR0 = _06_AD0_SAMPLING_STATE_0;
    S12AD.ADSSTR1 = _06_AD0_SAMPLING_STATE_1;

    /* Set compare control register */
    S12AD.ADCMPCR.WORD = _0000_AD_WINDOWB_DISABLE | _0000_AD_WINDOWA_DISABLE | _0000_AD_WINDOWFUNCTION_DISABLE;

    /* Set ELC scan end event generation condition */
    S12AD.ADELCCR.BYTE = _02_ALL_SCAN_COMPLETION;

    /* Set interrupt priority level */
    IPR(S12AD, S12ADI0) = _0F_AD_PRIORITY_LEVEL15;

    /* Set AN000 pin */
    PORT4.PMR.BYTE &= 0xFEU;
    PORT4.PDR.BYTE &= 0xFEU;
    MPC.P40PFS.BYTE = 0x80U;

    /* Set AN001 pin */
    PORT4.PMR.BYTE &= 0xFDU;
    PORT4.PDR.BYTE &= 0xFDU;
    MPC.P41PFS.BYTE = 0x80U;

    R_Config_S12AD0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_S12AD0_Start
* Description  : This function starts the AD0 converter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD0_Start(void)
{
    IR(S12AD, S12ADI0) = 0U;
    IEN(S12AD, S12ADI0) = 1U;
    S12AD.ADCSR.BIT.ADST = 1U;
}

/***********************************************************************************************************************
* Function Name: R_Config_S12AD0_Stop
* Description  : This function stop the AD0 converter
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD0_Stop(void)
{
    S12AD.ADCSR.BIT.ADST = 0U;
    IEN(S12AD, S12ADI0) = 0U;
    IR(S12AD, S12ADI0) = 0U;
}

/***********************************************************************************************************************
* Function Name: R_Config_S12AD0_Get_ValueResult
* Description  : This function gets result from the AD0 converter
* Arguments    : channel -
*                    channel of data register to be read
*                buffer -
*                    buffer pointer
* Return Value : None
***********************************************************************************************************************/

void R_Config_S12AD0_Get_ValueResult(ad_channel_t channel, uint16_t * const buffer)
{
    if (channel == ADSELFDIAGNOSIS)
    {
        *buffer = (uint16_t)(S12AD.ADRD.WORD);
    }
    else if (channel == ADCHANNEL0)
    {
        *buffer = (uint16_t)(S12AD.ADDR0);
    }
    else if (channel == ADCHANNEL1)
    {
        *buffer = (uint16_t)(S12AD.ADDR1);
    }
    else if (channel == ADCHANNEL2)
    {
        *buffer = (uint16_t)(S12AD.ADDR2);
    }
    else if (channel == ADCHANNEL3)
    {
        *buffer = (uint16_t)(S12AD.ADDR3);
    }
    else if (channel == ADCHANNEL4)
    {
        *buffer = (uint16_t)(S12AD.ADDR4);
    }
    else if (channel == ADCHANNEL5)
    {
        *buffer = (uint16_t)(S12AD.ADDR5);
    }
    else if (channel == ADCHANNEL6)
    {
        *buffer = (uint16_t)(S12AD.ADDR6);
    }
    else if (channel == ADCHANNEL7)
    {
        *buffer = (uint16_t)(S12AD.ADDR7);
    }
    else if (channel == ADCHANNEL16)
    {
        *buffer = (uint16_t)(S12AD.ADDR16);
    }
    else if (channel == ADCHANNEL17)
    {
        *buffer = (uint16_t)(S12AD.ADDR17);
    }
    else if (channel == ADCHANNEL18)
    {
        *buffer = (uint16_t)(S12AD.ADDR18);
    }
    else if (channel == ADCHANNEL19)
    {
        *buffer = (uint16_t)(S12AD.ADDR19);
    }
    else if (channel == ADCHANNEL20)
    {
        *buffer = (uint16_t)(S12AD.ADDR20);
    }
    else if (channel == ADCHANNEL21)
    {
        *buffer = (uint16_t)(S12AD.ADDR21);
    }
    else if (channel == ADCHANNEL24)
    {
        *buffer = (uint16_t)(S12AD.ADDR24);
    }
    else if (channel == ADCHANNEL25)
    {
        *buffer = (uint16_t)(S12AD.ADDR25);
    }
    else if (channel == ADCHANNEL26)
    {
        *buffer = (uint16_t)(S12AD.ADDR26);
    }
    else if (channel == ADTEMPSENSOR)
    {
        *buffer = (uint16_t)(S12AD.ADTSDR);
    }
    else if (channel == ADINTERREFVOLT)
    {
        *buffer = (uint16_t)(S12AD.ADOCDR);
    }
    else if (channel == ADDATADUPLICATION)
    {
        *buffer = (uint16_t)(S12AD.ADDBLDR);
    }
    else
    {
        /* Do Nothing */ 
    }
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */   
