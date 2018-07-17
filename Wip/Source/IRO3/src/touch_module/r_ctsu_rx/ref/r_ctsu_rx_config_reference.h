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
* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_ctsu_rx_config.h
* Description  : Configures the CTSU module.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 28.07.2016 1.00    First Release
***********************************************************************************************************************/
#ifndef CTSU_RX_CONFIG_HEADER_FILE
#define CTSU_RX_CONFIG_HEADER_FILE
/**
 * Perform parameter checking on function call. (0 := Disabled)
 */
#define CTSU_CFG_PARAM_CHECKING_ENABLE	(1)

/**
 * Define maximum control blocks the user is allowed to open. (1 := Minimum).
 */
#define CTSU_CFG_MAX_CONTROL_BLOCK_COUNT    (1)

/**
 * Set up the interrupt priority level for the CTSU interrupts CTSURD, CTSUWR,
 * and CTSUFN. (0 := disabled; 1 := lowest priority; 15 := highest priority)
 * */
#define CTSU_CFG_INTERRUPT_PRIORITY_LEVEL			(8)

/**
 * Apply correction factor to measured data.
 */
#define CTSU_CFG_ENABLE_CORRECTION	(0)

/**
 * Include code for R_CTSU_Calibrate. Allow fine tuning of CTSU count readings.
 */
#define CTSU_CFG_CALIBRATION_ENABLE				(0)

/**
 * Use the DTC for transferring data from RAM to SFRs and SFRs to RAM during
 * CTSUWR and CTSURD interrupts.
 * Atleast 16 bytes of additional memory is allocated to the control block.
 */
#define CTSU_CFG_DTC_TRANSFER_ENABLE	(0)

/**
 * When enabled, DTC chain transfers are used to reset transfer control blocks.
 * An additional 32 bytes of memory is allocated to each control block.
 */
#define CTSU_CFG_DTC_CHAIN_TRANSFERS	(0)

#endif /* CTSU_RX_CONFIG_HEADER_FILE */
