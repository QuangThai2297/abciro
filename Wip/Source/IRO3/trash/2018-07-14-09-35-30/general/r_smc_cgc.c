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
* File Name    : r_smc_cgc.c
* Version      : 1.1.1
* Device(s)    : R5F51303AxFM
* Description  : This file implements cgc setting
* Creation Date: 2018-07-14
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_smc_cgc.h"
#include "platform.h"

/***********************************************************************************************************************
* Function Name: R_CGC_Create
* Description  : This function Used to initializes the clock generator
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_CGC_Create(void)
{
    uint32_t w_count;

    /* Stop sub-clock */
    SYSTEM.SOSCCR.BIT.SOSTP = 1U;

    /* Wait for the register modification to complete */
    while (1U != SYSTEM.SOSCCR.BIT.SOSTP);

    /* Stop sub-clock */
    RTC.RCR3.BIT.RTCEN = 0U;

    /* Wait for the register modification to complete */
    while (0U != RTC.RCR3.BIT.RTCEN);

    /* Wait for 5 sub-clock cycles */
    for (w_count = 0U; w_count < _0001_CGC_SUBSTPWT_WAIT; w_count++)
    {
        nop();
    }

    /* Set sub-clock drive capacity */
    while (1U != RTC.RCR3.BIT.RTCDV)
    {
        RTC.RCR3.BIT.RTCDV = 1U;
    }
    
    /* Set sub-clock */
    SYSTEM.SOSCCR.BIT.SOSTP = 0U;

    /* Wait for the register modification to complete */
    while (0U != SYSTEM.SOSCCR.BIT.SOSTP);

    /* Wait for sub-clock to be stable */
    for (w_count = 0U; w_count < _00000001_CGC_SUBOSCWT_WAIT; w_count++)
    {
        nop();
    }

    /* Set LOCO */
    SYSTEM.LOCOCR.BIT.LCSTP = 0U;

    R_CGC_Create_UserInit();

}

