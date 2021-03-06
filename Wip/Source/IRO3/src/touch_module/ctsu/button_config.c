
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
* File Name    : button_config.c
* Version      : 2.0 
* Description  : This file contains all TOUCH button configurations.
***********************************************************************************************************************/
#include "r_touch_button_if.h"
#include "r_touch.h"
extern touch_cfg_t g_touch_cfg_on_g_ctsu_cfg_self;
extern void App_TOUCH_Button_Notification(void*);
touch_button_cfg_t const Button_RX07_on_g_touch_cfg_on_g_ctsu_cfg_self = { 
     .button.rx = 7,
     .button.tx = 255,
     .debounce = 20,
     .hold_max = 1000,
     .p_callback = App_TOUCH_Button_Notification,
     .enable.press = true,
     .enable.release = true,
     .enable.hold =false,
     .p_touch_cfg = &g_touch_cfg_on_g_ctsu_cfg_self,
};
extern touch_cfg_t g_touch_cfg_on_g_ctsu_cfg_self;
extern void App_TOUCH_Button_Notification(void*);
touch_button_cfg_t const Button_RX08_on_g_touch_cfg_on_g_ctsu_cfg_self = { 
     .button.rx = 8,
     .button.tx = 255,
     .debounce = 20,
     .hold_max = 1000,
     .p_callback = App_TOUCH_Button_Notification,
     .enable.press = true,
     .enable.release = true,
     .enable.hold =false,
     .p_touch_cfg = &g_touch_cfg_on_g_ctsu_cfg_self,
};
extern touch_cfg_t g_touch_cfg_on_g_ctsu_cfg_self;
extern void App_TOUCH_Button_Notification(void*);
touch_button_cfg_t const Button_RX06_on_g_touch_cfg_on_g_ctsu_cfg_self = { 
     .button.rx = 6,
     .button.tx = 255,
     .debounce = 20,
     .hold_max = 1000,
     .p_callback = App_TOUCH_Button_Notification,
     .enable.press = true,
     .enable.release = true,
     .enable.hold = false,
     .p_touch_cfg = &g_touch_cfg_on_g_ctsu_cfg_self,
};
extern touch_cfg_t g_touch_cfg_on_g_ctsu_cfg_self;
extern void App_TOUCH_Button_Notification(void*);
touch_button_cfg_t const Button_RX09_on_g_touch_cfg_on_g_ctsu_cfg_self = { 
     .button.rx = 9,
     .button.tx = 255,
     .debounce = 20,
     .hold_max = 1000,
     .p_callback = App_TOUCH_Button_Notification,
     .enable.press = true,
     .enable.release = true,
     .enable.hold =false,
     .p_touch_cfg = &g_touch_cfg_on_g_ctsu_cfg_self,
};

#define CONFIGURATION_COUNT    (4)
touch_button_cfg_t const * const g_all_button_configs[CONFIGURATION_COUNT + 1] = 
{
#if (CONFIGURATION_COUNT > 0)
    &Button_RX07_on_g_touch_cfg_on_g_ctsu_cfg_self,&Button_RX08_on_g_touch_cfg_on_g_ctsu_cfg_self,&Button_RX06_on_g_touch_cfg_on_g_ctsu_cfg_self,&Button_RX09_on_g_touch_cfg_on_g_ctsu_cfg_self,
#endif
    NULL,
};

const uint16_t g_button_config_count = CONFIGURATION_COUNT;
