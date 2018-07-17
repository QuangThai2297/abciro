/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : ctsu_pin_info.c
* Version      : 1.0
* Description  : This file contains information about the GPIO pins which can be used as CTSU Pins
***********************************************************************************************************************/

#include "ctsu_pin_info.h"

#if defined(BSP_MCU_RX113)
#define MPC_PIN_FOR_CTSU    (0x19)
const gpio_port_pin_t g_ctsu_port_pin_as_tscap[] =
{
 [0] = {GPIO_PORT_2_PIN_6},
};

const gpio_port_pin_t g_ctsu_port_pin_as_tspin[] =
{
		[0] =  {GPIO_PORT_0_PIN_7},
		[1] =  {GPIO_PORT_0_PIN_4},
		[2] =  {GPIO_PORT_0_PIN_2},
		[3] =  {GPIO_PORT_J_PIN_3},
		[4] =  {GPIO_PORT_2_PIN_5},
		[5] =  {GPIO_PORT_2_PIN_4},
		[6] =  {GPIO_PORT_2_PIN_3},
		[7] =  {GPIO_PORT_2_PIN_2},
		[8] =  {GPIO_PORT_2_PIN_1},
		[9] =  {GPIO_PORT_2_PIN_0},
		[10] = {GPIO_PORT_2_PIN_7},
		[11] = {GPIO_PORT_3_PIN_2},
};
#elif defined(BSP_MCU_RX130)
#define MPC_PIN_FOR_CTSU    (0x19)
const gpio_port_pin_t g_ctsu_port_pin_as_tscap[] =
{
 [0] = {GPIO_PORT_C_PIN_4},
};
const gpio_port_pin_t g_ctsu_port_pin_as_tspin[] =
{
 [0] =  {GPIO_PORT_3_PIN_2},
 [1] =  {GPIO_PORT_3_PIN_1},
 [2] =  {GPIO_PORT_3_PIN_0},
 [3] =  {GPIO_PORT_2_PIN_7},
 [4] =  {GPIO_PORT_2_PIN_6},
 [5] =  {GPIO_PORT_1_PIN_5},
 [6] =  {GPIO_PORT_1_PIN_4},
 [7] =  {GPIO_PORT_H_PIN_3},
 [8] =  {GPIO_PORT_H_PIN_2},
 [9] =  {GPIO_PORT_H_PIN_1},
 [10] = {GPIO_PORT_H_PIN_0},
 [11] = {GPIO_PORT_5_PIN_5},
 [12] = {GPIO_PORT_5_PIN_4},
 [13] = {GPIO_PORT_C_PIN_7},
 [14] = {GPIO_PORT_C_PIN_6},
 [15] = {GPIO_PORT_C_PIN_5},
 [16] = {GPIO_PORT_C_PIN_3},
 [17] = {GPIO_PORT_C_PIN_2},
 [18] = {GPIO_PORT_B_PIN_7},
 [19] = {GPIO_PORT_B_PIN_6},
 [20] = {GPIO_PORT_B_PIN_5},
// [21] = {GPIO_PORT_B_PIN_4},
 [22] = {GPIO_PORT_B_PIN_3},
// [23] = {GPIO_PORT_B_PIN_2},
 [24] = {GPIO_PORT_B_PIN_1},
 [25] = {GPIO_PORT_B_PIN_0},
 [26] = {GPIO_PORT_A_PIN_6},
// [27] = {GPIO_PORT_A_PIN_5},
 [28] = {GPIO_PORT_A_PIN_4},
 [29] = {GPIO_PORT_A_PIN_3},
// [30] = {GPIO_PORT_A_PIN_2},
 [31] = {GPIO_PORT_A_PIN_1},
 [32] = {GPIO_PORT_A_PIN_0},
 [33] = {GPIO_PORT_E_PIN_4},
 [34] = {GPIO_PORT_E_PIN_3},
 [35] = {GPIO_PORT_E_PIN_2},
};
#elif defined(BSP_MCU_RX230) || defined(BSP_MCU_RX231)
#define MPC_PIN_FOR_CTSU    (0x19)
const gpio_port_pin_t g_ctsu_port_pin_as_tscap[] =
{
 [0] = {GPIO_PORT_C_PIN_4},
};
const gpio_port_pin_t g_ctsu_port_pin_as_tspin[] =
{
 [0] =  {GPIO_PORT_3_PIN_4},
 [1] =  {GPIO_PORT_3_PIN_3},
 [2] =  {GPIO_PORT_2_PIN_7},
 [3] =  {GPIO_PORT_2_PIN_6},
 [4] =  {GPIO_PORT_2_PIN_5},
 [5] =  {GPIO_PORT_2_PIN_4},
 [6] =  {GPIO_PORT_2_PIN_3},
 [7] =  {GPIO_PORT_2_PIN_2},
 [8] =  {GPIO_PORT_2_PIN_1},
 [9] =  {GPIO_PORT_2_PIN_0},
 [12] = {GPIO_PORT_1_PIN_5},
 [13] = {GPIO_PORT_1_PIN_4},
 [15] = {GPIO_PORT_5_PIN_5},
 [16] = {GPIO_PORT_5_PIN_4},
 [17] = {GPIO_PORT_5_PIN_3},
 [18] = {GPIO_PORT_5_PIN_2},
 [19] = {GPIO_PORT_5_PIN_1},
 [20] = {GPIO_PORT_5_PIN_0},
 [22] = {GPIO_PORT_C_PIN_6},
 [23] = {GPIO_PORT_C_PIN_5},
 [27] = {GPIO_PORT_C_PIN_3},
 [30] = {GPIO_PORT_C_PIN_2},
 [33] = {GPIO_PORT_C_PIN_1},
 [35] = {GPIO_PORT_C_PIN_0},
};
#endif

const ctsu_pin_info_t g_ctsu_pin_info =
{
    .pin_function = MPC_PIN_FOR_CTSU,
    .tscap_count = (sizeof(g_ctsu_port_pin_as_tscap))/sizeof(gpio_port_pin_t),
    .tspin_count = (sizeof(g_ctsu_port_pin_as_tspin))/sizeof(gpio_port_pin_t),
    .p_tscap = g_ctsu_port_pin_as_tscap,
    .p_tspin = g_ctsu_port_pin_as_tspin,
};
