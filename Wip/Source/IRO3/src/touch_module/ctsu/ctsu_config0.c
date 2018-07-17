
/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : ctsu_config.c
* Version      : 2.0
* Description  : This file contains CTSU SFR settings.
***********************************************************************************************************************/
#include "r_ctsu.h"
#include "r_ctsu_rx_config.h"
#include "r_ctsu_rx_if.h"

#if defined(CTSURICOA_TS)
#undef CTSURICOA_TS
#define CTSURICOA_TS	(0x5F)
#endif
#if defined(CTSURICOA_MUTUAL0)
#undef CTSURICOA_MUTUAL0
#define CTSURICOA_MUTUAL0	(0x3F)
#endif
#if defined(CTSURICOA_MUTUAL1)
#define CTSURICOA_MUTUAL1	(0x3F)
#endif
#if defined(CTSURICOA_MUTUAL2)
#define CTSURICOA_MUTUAL2	(0x3F)
#endif
#if defined(CTSURICOA_MUTUAL3)
#define CTSURICOA_MUTUAL3	(0x3F)
#endif
#if defined(CTSURICOA_MUTUAL4)
#define CTSURICOA_MUTUAL4	(0x3F)
#endif
#if defined(CTSURICOA_MUTUAL5)
#define CTSURICOA_MUTUAL5	(0x3F)
#endif
#if defined(CTSURICOA_MUTUAL6)
#define CTSURICOA_MUTUAL6	(0x3F)
#endif
#if defined(CTSURICOA_MUTUAL7)
#define CTSURICOA_MUTUAL7	(0x3F)
#endif


#pragma section _CTSU

static ctsu_sensor_setting_t sensor_setting_g_ctsu_cfg_self[] =
{
	{ .ctsussc = CTSUSSC_TS06,  .ctsuso0 = CTSUSO0_TS06,  .ctsuso1 = CTSUSO1_TS06, },
	{ .ctsussc = CTSUSSC_TS07,  .ctsuso0 = CTSUSO0_TS07,  .ctsuso1 = CTSUSO1_TS07, },
	{ .ctsussc = CTSUSSC_TS08,  .ctsuso0 = CTSUSO0_TS08,  .ctsuso1 = CTSUSO1_TS08, },
	{ .ctsussc = CTSUSSC_TS09,  .ctsuso0 = CTSUSO0_TS09,  .ctsuso1 = CTSUSO1_TS09, },
};
	
static ctsu_const_sfrs_t ctsu_const_sfrs_g_ctsu_cfg_self = {
	.ctsucr0.byte     = (0x00),
	.ctsucr1.byte     = ((_01_CTSUMD_SELF_MULTI<<6)|(_00_CTSUCLK_PCLK<<4)|(SELF_CTSUATUNE1<<3)|(_0_CTSUATUNE0_NORMAL<<2)|(_1_CTSUCSW_ON<<1)|(_1_CTSUPON_HW_POWER_ON<<0)),
	.ctsusdprs.byte   = ((SELF_CTSUSOFF<<6)|(SELF_CTSUPRMODE<<4)|(SELF_CTSUPRRATIO<<0)),
	.ctsusst.byte     = (_00010000_CTSUSST_RECOMMEND),
	.ctsuchac0.byte   = 0|(SELF_ENABLE_TS06<<6)|(SELF_ENABLE_TS07<<7),
	.ctsuchac1.byte   = 0|(SELF_ENABLE_TS08<<0)|(SELF_ENABLE_TS09<<1),
#if !defined(BSP_MCU_RX113)
	.ctsuchac2.byte   = 0,
	.ctsuchac3.byte   = 0,
	.ctsuchac4.byte   = 0,
#endif

	.ctsuchtrc0.byte  = 0,
	.ctsuchtrc1.byte  = 0,
#if !defined(BSP_MCU_RX113)
	.ctsuchtrc2.byte  = 0,
	.ctsuchtrc3.byte  = 0,
	.ctsuchtrc4.byte  = 0,
#endif
	.ctsudclkc.byte   = ((_11_CTSUSSCNT<<4)|(_00_CTSUSSMOD<<0)),
};

#pragma section

static uint16_t ctsu_sensor_data_g_ctsu_cfg_self[8];

#if (CTSU_CFG_ENABLE_CORRECTION > 0)
static uint16_t ctsu_sensor_ico_ideal[] =
{
    _19_2UA,
    _18_0UA,
    _16_8UA,
    _15_6UA,
    _14_4UA,
    _13_2UA,
    _12_0UA,
    _10_8UA,
    _09_6UA,
    _08_4UA,
    _07_2UA,
    _06_0UA,
    _04_8UA,
    _03_6UA,
    _02_4UA,
    _01_2UA
};

static uint16_t ctsu_correction_buffer[(2*61) + 8];
#endif

ctsu_cfg_t g_ctsu_cfg_self = {
    /* One Time Settings */
    .p_ctsu_settings = &ctsu_const_sfrs_g_ctsu_cfg_self,
    /* Per channel settings */
    .p_sensor_settings = (ctsu_sensor_setting_t*)sensor_setting_g_ctsu_cfg_self,
    .p_sensor_data = ctsu_sensor_data_g_ctsu_cfg_self,
    .pclkb_hz = 32000000,	/* XXX: Replace with value used for PCLKB when tuning. */
    .p_callback = NULL,
#if (CTSU_CFG_ENABLE_CORRECTION > 0)
    .p_correction_ideal_sensor_ico = &ctsu_sensor_ico_ideal[0],
    .p_correction_buffer = &ctsu_correction_buffer[0],
#endif
};
	