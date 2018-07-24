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
 * Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_touch_rx_config.h
 * Description  : This file contains global options used to configure the R_Touch API
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           03.19.2015 1.00    Initial Release.
 ***********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @file
 ***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @defgroup CONFIG Middleware Configuration options
 * @brief This file contains global options used to configure the TOUCH API.
 * @{
 * These parameters must be copied to the ${PROJECT_ROOT}/r_config/r_touch_rx_config.h, which is included in the
 * compiler include path.
 ***********************************************************************************************************************/
#ifndef R_TOUCH_RX_CONFIG_REF_H_
#define R_TOUCH_RX_CONFIG_REF_H_

/**
 * Specify whether to include code for API parameter checking.
 *    - 1 : Enabled	(Assertion messages routed to Renesas Virtual Console if BSP_CFG_IO_LIB_ENABLE==1)
 *    - 0 : No parameter checking (No assertions)
 */
#define TOUCH_CFG_PARAM_CHECKING_ENABLE			(0)

/**
 * Define maximum control blocks the user is allowed to open. Default=1
 */
#define TOUCH_CFG_MAX_CONTROL_BLOCK_COUNT				(1)

/**
 * Perform drift compensation and sensitivity optimization for non-touched sensors while a sensor is touched.
 */
#define TOUCH_CFG_COMPENSATE_WHILE_TOUCHED		(0)

/**
 * Define maximum number of upper layer callbacks (Default = 1)
 */
#define TOUCH_CFG_MAX_UPPER_LAYER_CALLBACKS			(3)

/**
 * Define the depth of the Single-Input-Single-Output averaging filter depth:= pow(2,N).
 */
#define TOUCH_CFG_FILTER_DEPTH					(1)

/**
 * Define if interval for drift compensation should be variable.
 */
#define TOUCH_CFG_VARIABLE_DRIFT_RATES				(0)

/**
 * Define the type of multi-touch rejection algorithm.
 * 0 := No Multi-Touch Rejection. (Value in max_touched_sensors is ignored)
 * 1 := Multi-Touch rejection will release all touched sensors, if non-debounced touch count is > max_touched_sensors.
 * 2 := Multi-Touch rejection will release all touched sensors, if debounced touch count is > max_touched_sensors.
 * 3 := Multi-Touch rejection will release all touched sensors, if (current debounced sensors - previously debounced sensors) > max_touched_sensors
 */
#define MULTITOUCH_REJECTION_TYPE					(0)

#endif /* R_TOUCH_RX_CONFIG_REF_H_ */
/*******************************************************************************************************************//**
 * @} (end defgroup CONFIG)
 ***********************************************************************************************************************/
