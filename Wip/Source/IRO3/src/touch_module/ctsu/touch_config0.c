
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
* File Name    : touch_config.c
* Version      : 2.0
* Description  : This file contains TOUCH middleware configuration settings.
***********************************************************************************************************************/
#include "r_touch_rx_if.h"
#include "r_touch.h"

#pragma section _TOUCH

static touch_common_parameter_t g_touch_cfg_on_g_ctsu_cfg_self_common_parameter =
{
    .drift_hold_limit = 50,
    .on_limit = (SELF_MSA == 0) ? (UINT16_MAX):(SELF_MSA),
#if defined(SLIDER_USE) || defined(WHEEL_USE)
    .max_touched_sensors = 2,
#else
    .max_touched_sensors = 1,
#endif
};

static touch_sensor_parameter_t g_touch_cfg_on_g_ctsu_cfg_self_sensor_parameter[] =
{

    [0] = {.threshold = SELF_TS06_THR, .hysteresis = SELF_TS06_HYS, .dt_limit = SELF_TOUCH_ON, .dr_limit = SELF_TOUCH_OFF, .drift_rate = SELF_DRIFT_FREQUENCY, .drift_rate_plus = 0, .drift_rate_minus = 0, .recalib_delay = 50, .recalib_threshold = 10},
    [1] = {.threshold = SELF_TS07_THR, .hysteresis = SELF_TS07_HYS, .dt_limit = SELF_TOUCH_ON, .dr_limit = SELF_TOUCH_OFF, .drift_rate = SELF_DRIFT_FREQUENCY, .drift_rate_plus = 0, .drift_rate_minus = 0, .recalib_delay = 50, .recalib_threshold = 10},
    [2] = {.threshold = SELF_TS08_THR, .hysteresis = SELF_TS08_HYS, .dt_limit = SELF_TOUCH_ON, .dr_limit = SELF_TOUCH_OFF, .drift_rate = SELF_DRIFT_FREQUENCY, .drift_rate_plus = 0, .drift_rate_minus = 0, .recalib_delay = 50, .recalib_threshold = 10},
    [3] = {.threshold = SELF_TS09_THR, .hysteresis = SELF_TS09_HYS, .dt_limit = SELF_TOUCH_ON, .dr_limit = SELF_TOUCH_OFF, .drift_rate = SELF_DRIFT_FREQUENCY, .drift_rate_plus = 0, .drift_rate_minus = 0, .recalib_delay = 50, .recalib_threshold = 10},
};

#pragma section

uint8_t g_touch_cfg_on_g_ctsu_cfg_self_binary[(4/8) + 1];

static uint8_t g_touch_cfg_on_g_ctsu_cfg_self_buffer[128];

extern ctsu_cfg_t g_ctsu_cfg_self;

touch_cfg_t g_touch_cfg_on_g_ctsu_cfg_self =
{
    .p_ctsu_cfg = &g_ctsu_cfg_self,
    .p_common = &g_touch_cfg_on_g_ctsu_cfg_self_common_parameter,
    .p_sensor = &g_touch_cfg_on_g_ctsu_cfg_self_sensor_parameter[0],
    .p_binary_result = g_touch_cfg_on_g_ctsu_cfg_self_binary,
    .p_callback = NULL,
    .num_ignored = 0,
    .p_ignored = NULL,
    .buffer = {
        .p_start = g_touch_cfg_on_g_ctsu_cfg_self_buffer,
        .size = sizeof(g_touch_cfg_on_g_ctsu_cfg_self_buffer),
    },
    .custom =
    {
        .p_filter = NULL,
        .p_touch_detect = NULL,
        .p_filter_instance = NULL,
        .num_filter_instances = 0,
    },
};
    