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
 * File Name : r_ctsu_rx.c
 * Version : 1.00: [2016-06-13] Released as Version 1.00.
 *            1.10: [2017-04-24] Added algorithms for calculating correction factors.
 * Description : This module provides the interface to the R_CTSU peripheral.
 ***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @file
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Includes
 ***********************************************************************************************************************/

#include <string.h>
#include "r_ctsu_rx_config.h"
#include "r_ctsu_rx_if.h"
#include "r_ctsu_rx_private.h"
#include "hw/r_ctsu_sfr_access_rx.h"

#if !defined(UNITY_TESTING) \
        && defined(BSP_CFG_IO_LIB_ENABLE) \
        && (BSP_CFG_IO_LIB_ENABLE == 1)
/** @note Use assertion on the Renesas Virtual Console.
 * Instructions: (Alt+Shift+Q, C) >
 *         (Button)Display Selected Console >
 *             Renesas Virtual Console >
 *                 (Button) Pin Console.
 */
#include "assert.h"
#endif

/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
/**
 * Unique number to identify if handle is open.
 */
#define OPEN    (0x4F50454e)
#define CTSUERRS_ENABLE_CALIBRATION_MODE    (0x0082)

#define TUNING_UPPER_LIMIT    (150)
#define TUNING_LOWER_LIMIT    (100)
#define OFFSET_UPPER_LIMIT    (200)
#define OFFSET_LOWER_LIMIT    (150)

#define CTSU_CORRECTION_SCAN_COUNT  (32)

#if defined(BSP_MCU_RX113)
#define LAST_TS  (12)
#elif defined(BSP_MCU_RX130)
#define LAST_TS  (36)
#elif defined(BSP_MCU_RX230)
#define LAST_TS  (36)
#elif defined(BSP_MCU_RX231)
#define LAST_TS  (36)
#else
#error "Unsupported MCU"
#endif

#if defined(assert)
#define ASSERT(test)    (assert(test))
#else
#define ASSERT(test)    if(false==(test)) {return CTSU_ERR_INVALID_PARAM;}
#endif

#define CTSU_CFG_CTSU_CORRECTION_READINGS     (5)

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Private variables
 ***********************************************************************************************************************/
#if (CTSU_CFG_DTC_TRANSFER_ENABLE == true)
static const dtc_internal_registers_t ctsu_wr_init[DTC_TCB_COUNT] =
{
[0] = { /* Moves from data from RAM to CTSU SFRs */
        .FIRST_LWORD.REG.MRA.BIT.MD = 2,    //2 = Block transfer mode
        .FIRST_LWORD.REG.MRA.BIT.SZ = 1,    //1 = 1-word transfer
        .FIRST_LWORD.REG.MRA.BIT.SM = 2,    //2 = SAR value is incremented after data transfer
        .FIRST_LWORD.REG.MRB.BIT.DM = 2,    //2 = Address in DAR is incremented .
        .FIRST_LWORD.REG.MRB.BIT.CHNS = 1,  //1 = Perform chain transfer after this transfer is complete.
        .FIRST_LWORD.REG.MRB.BIT.CHNE = 1,  //1 = Enable chain transfer
        .SECOND_LWORD.SAR = NULL,           // Filled in after all initialization
        .THIRD_LWORD.DAR = (uint32_t)&CTSU.CTSUSSC.WORD,
        .FOURTH_LWORD.REG.CRA.BYTE.CRA_H = 3,   // Block size is 3 units
        .FOURTH_LWORD.REG.CRA.BYTE.CRA_L = 3,   // Initialize counter to CRAH.
        .FOURTH_LWORD.REG.CRB.WORD = 0,         // Block transfer count is 65536.
    },
#if (CTSU_CFG_DTC_CHAIN_TRANSFERS == true)
[1] = {/* Reload SAR register of 0th node in chain. */
       .FIRST_LWORD.REG.MRA.BIT.MD = 1,     //1 = Repeat transfer mode
       .FIRST_LWORD.REG.MRA.BIT.SZ = 2,     //2 = 32 bit transfer
       .FIRST_LWORD.REG.MRA.BIT.SM = 0,     //0 = SAR value is constant after data transfer
       .FIRST_LWORD.REG.MRB.BIT.DM = 0,     //0 = DAR value is constant after data transfer
       .FIRST_LWORD.REG.MRB.BIT.DTS = 1,    //1 = Source is the repeat/block area
       .FIRST_LWORD.REG.MRB.BIT.CHNS = 1,   //1 = Perform chain transfer after this transfer is complete.
       .FIRST_LWORD.REG.MRB.BIT.CHNE = 1,   //1 = Enable chain transfer
       .SECOND_LWORD.SAR = 0,               // Filled in after all initialization
       .THIRD_LWORD.DAR = 0,                // Filled in after all initialization
       .FOURTH_LWORD.REG.CRA.BYTE.CRA_H = 1,    // Reload Block size is 1 units
       .FOURTH_LWORD.REG.CRA.BYTE.CRA_L = 1,    // Initialize counter size to 1 unit
       .FOURTH_LWORD.REG.CRB.WORD = 0,          // Ignored in repeat transfer mode
    },
[2] = { /* Fixes the block transfer count of the 0th node in chain. */
        .FIRST_LWORD.REG.MRA.BIT.MD = 1,    //1 = Repeat transfer mode
        .FIRST_LWORD.REG.MRA.BIT.SZ = 1,    //2 = 16 bit transfer
        .FIRST_LWORD.REG.MRA.BIT.SM = 0,    //0 = SAR value is constant after data transfer
        .FIRST_LWORD.REG.MRB.BIT.DM = 0,    //0 = DAR value is constant after data transfer
        .FIRST_LWORD.REG.MRB.BIT.DTS = 1,   //1 = Source is the repeat/block area
        .FIRST_LWORD.REG.MRB.BIT.CHNS = 0,  //1 = Perform chain transfer after this transfer is complete.
        .FIRST_LWORD.REG.MRB.BIT.CHNE = 0,  //1 = Enable chain transfer
        .SECOND_LWORD.SAR = 0,              // Filled in after all initialization
        .FOURTH_LWORD.REG.CRA.BYTE.CRA_H = 1,   // Reload Block size is 1 units
        .FOURTH_LWORD.REG.CRA.BYTE.CRA_L = 1,   // Initialize counter size to 1 unit
        .FOURTH_LWORD.REG.CRB.WORD = 0,         // Ignored in repeat transfer mode
    },
#endif /*(CTSU_CFG_DTC_CHAIN_TRANSFERS == true) */
};

static const dtc_internal_registers_t ctsu_rd_init[DTC_TCB_COUNT] = {
[0] = { /* Moves data from the CTSU SFRs to RAM */
        .FIRST_LWORD.REG.MRA.BIT.MD = 2,        //2 = Block transfer mode
        .FIRST_LWORD.REG.MRA.BIT.SZ = 1,        //1 = 1-word transfer
        .FIRST_LWORD.REG.MRA.BIT.SM = 2,        //2 = SAR value is incremented after data transfer
        .FIRST_LWORD.REG.MRB.BIT.DM = 2,        //2 = Address in DAR is incremented.
        .FIRST_LWORD.REG.MRB.BIT.DTS = 1,       //1 = Source is the repeat/block area
        .FIRST_LWORD.REG.MRB.BIT.CHNS = 1,      //1 = Perform chain transfer after this transfer is complete.
        .FIRST_LWORD.REG.MRB.BIT.CHNE = 1,      //1 = Enable chain transfer
        .SECOND_LWORD.SAR = (uint32_t)&CTSU.CTSUSC.WORD,
        .THIRD_LWORD.DAR = 0,                   // Filled in after all initialization
        .FOURTH_LWORD.REG.CRA.BYTE.CRA_H = 2,   // Block size is 2 units
        .FOURTH_LWORD.REG.CRA.BYTE.CRA_L = 2,   // Initialize counter to CRAH.
        .FOURTH_LWORD.REG.CRB.WORD = 0,         // First initialized through R_Touch_Open and re-init by node [2].
    },
#if (CTSU_CFG_DTC_CHAIN_TRANSFERS == true)
[1] = { /* Fixes DAR register of 0th node in chain. */
        .FIRST_LWORD.REG.MRA.BIT.MD = 1,    //1 = Repeat transfer mode
        .FIRST_LWORD.REG.MRA.BIT.SZ = 2,    //2 = 32 bit transfer
        .FIRST_LWORD.REG.MRA.BIT.SM = 0,    //0 = SAR value is constant after data transfer
        .FIRST_LWORD.REG.MRB.BIT.DM = 0,    //0 = DAR value is constant after data transfer
        .FIRST_LWORD.REG.MRB.BIT.DTS = 1,   //1 = Source is the repeat/block area
        .FIRST_LWORD.REG.MRB.BIT.CHNS = 1,  //1 = Perform chain transfer after this transfer is complete.
        .FIRST_LWORD.REG.MRB.BIT.CHNE = 1,  //1 = Enable chain transfer
        .SECOND_LWORD.SAR = 0,              // Filled in after all initialization
        .FOURTH_LWORD.REG.CRA.BYTE.CRA_H = 1,   // Reload Block size is 1 units
        .FOURTH_LWORD.REG.CRA.BYTE.CRA_L = 1,   // Initialize counter size to 1 unit
        .FOURTH_LWORD.REG.CRB.WORD = 0,         // Ignored in repeat transfer mode
    },
[2] = { /* Fixes the block transfer count of the 0th node in chain. */
        .FIRST_LWORD.REG.MRA.BIT.MD = 1,    //1 = Repeat transfer mode
        .FIRST_LWORD.REG.MRA.BIT.SZ = 1,    //1 = 16 bit transfer
        .FIRST_LWORD.REG.MRA.BIT.SM = 0,    //0 = SAR value is constant after data transfer
        .FIRST_LWORD.REG.MRB.BIT.DM = 0,    //0 = DAR value is constant after data transfer
        .FIRST_LWORD.REG.MRB.BIT.DTS = 1,   //1 = Source is the repeat/block area
        .FIRST_LWORD.REG.MRB.BIT.CHNS = 0,  //0 = Perform chain transfer after this transfer is complete.
        .FIRST_LWORD.REG.MRB.BIT.CHNE = 0,  //0 = Disable chain transfer
        .SECOND_LWORD.SAR = 0,              // Filled in after all initialization
        .FOURTH_LWORD.REG.CRA.BYTE.CRA_H = 1,   // Reload Block size is 1 units
        .FOURTH_LWORD.REG.CRA.BYTE.CRA_L = 1,   // Initialize counter size to 1 unit
        .FOURTH_LWORD.REG.CRB.WORD = 0,         // Ignored in repeat transfer mode
    },
#endif /*(CTSU_CFG_DTC_CHAIN_TRANSFERS == true) */
};
#endif

static ctsu_ctrl_blk_t pvt_ctsu_ctrl_blk[CTSU_CFG_MAX_CONTROL_BLOCK_COUNT];    ///< Local control blocks.
static uint32_t pvt_current_scan_handle_index = UINT32_MAX;                    ///< Identifies which handle in the array last used for scanning with the CTSU.
#if (CTSU_CFG_DTC_TRANSFER_ENABLE == false) && (CTSU_CFG_DTC_CHAIN_TRANSFERS == false)
static uint32_t sensor_wr_irq_itr = 0;        ///< CTSU Write Interrupt iterator
static uint32_t sensor_rd_irq_itr = 0;        ///< CTSU Read Interrupt iterator
#endif

/***********************************************************************************************************************
 Global Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 Private function prototypes
 ***********************************************************************************************************************/
#pragma inline (hw_ctsu_poweron)
static void hw_ctsu_poweron (void);
#pragma inline (hw_ctsu_poweroff)
static void hw_ctsu_poweroff (void);
static uint16_t calculate_sensor_count (ctsu_mode_t mode, ctsu_const_sfrs_t * p_ctsu_settings);
static ctsu_err_t setup_dtc_tcb_chain(ctsu_ctrl_blk_t *const p_ctrl_blk);
static void ctsu_read_output(ctsu_mode_t mode, uint16_t num_sensors, uint16_t const * const offsets,void* p_dest, void* p_src);
static void ctsu_read_counter(ctsu_mode_t mode, uint16_t num_sensors, uint16_t const * const offsets,void* p_dest, void* p_src);
static void ctsu_handle_register_value_get(ctsu_ctrl_blk_t* p_ctrl_blk, ctsu_cmd_t cmd, void* p_dest);
static void ctsu_handle_register_value_set(ctsu_ctrl_blk_t* p_ctrl_blk, ctsu_cmd_t cmd, void* value);
static void ctsu_handle_register_16bit_get(ctsu_ctrl_blk_t* p_ctrl_blk, ctsu_cmd_t cmd, uint16_t offset, void* value);
static void ctsu_handle_register_16bit_set(ctsu_ctrl_blk_t* p_ctrl_blk, ctsu_cmd_t cmd, uint16_t offset, uint16_t value);
#if (CTSU_CFG_CALIBRATION_ENABLE > 0)
static ctsu_err_t ctsu_correction_factor(uint32_t usr_hdl);
#if (CTSU_CFG_ENABLE_CORRECTION > 0)
static ctsu_err_t ctsu_correction_get_avg_sensor_ico_value(uint32_t tune_hdl, uint16_t * const p_result);
static uint8_t ctsu_get_sdpa_setting(uint32_t target_freq, uint32_t pclkb, uint8_t ctsuclk);
static uint16_t ctsu_corrected_value(uint16_t , int8_t, uint16_t const * const, uint16_t const * const );
#endif /* (CTSU_CFG_ENABLE_CORRECTION > 0) */
#endif /* (CTSU_CFG_CALIBRATION_ENABLE > 0) */

#if (CTSU_CFG_DTC_TRANSFER_ENABLE == true) && (CTSU_CFG_DTC_CHAIN_TRANSFERS == false)
static void _setup_dtc_tcb0(ctsu_ctrl_blk_t * );
#endif

#if defined(__RENESAS__)
/* Renesas Compiler */
#pragma interrupt (excep_ctsu_ctsufn(vect=VECT(CTSU,CTSUFN)))
#endif /* #if defined(__RENESAS__) */
#if defined(__IAR_SYSTEMS_ICC__)
/* IAR Compiler */
#pragma vector=VECT(CTSU,CTSUFN)
__interrupt static void excep_ctsu_ctsufn(void)
#endif /* #if defined(__IAR_SYSTEMS_ICC__) */
static void excep_ctsu_ctsufn(void);

#if defined(__RENESAS__)
/* Renesas Compiler */
#pragma interrupt (excep_ctsu_ctsurd(vect=VECT(CTSU,CTSURD)))
#endif /* #if defined(__RENESAS__) */
#if defined(__IAR_SYSTEMS_ICC__)
/* IAR Compiler */
#pragma vector=VECT(CTSU,CTSURD)
__interrupt static void excep_ctsu_ctsurd(void)
#endif /* #if defined(__IAR_SYSTEMS_ICC__) */
static void excep_ctsu_ctsurd(void);

#if defined(__RENESAS__)
/* Renesas Compiler */
#pragma interrupt (excep_ctsu_ctsuwr(vect=VECT(CTSU,CTSUWR)))
#endif /* #if defined(__RENESAS__) */
#if defined(__IAR_SYSTEMS_ICC__)
/* IAR Compiler */
#pragma vector=VECT(CTSU,CTSUWR)
__interrupt static void excep_ctsu_ctsuwr(void)
#endif /* #if defined(__IAR_SYSTEMS_ICC__) */
static void excep_ctsu_ctsuwr(void);


/***********************************************************************************************************************
 Functions
 **********************************************************************************************************************/
#if defined(__RENESAS__)
/* Renesas Compiler */
#pragma inline(R_CTSU_GetVersion)
#endif
#if defined(__IAR_SYSTEMS_ICC__)
/* IAR Compiler */
#pragma inline
#endif
/***********************************************************************************************************************
* Function Name: R_CTSU_GetVersion
* Description  : Get the version number for the driver.
* Arguments    : None
* Return Value :
* @return Version number of driver in 32-bits.
***********************************************************************************************************************/
uint32_t R_CTSU_GetVersion (void)
{
    /* These version macros are defined in r_ctsu_rx_if.h. */
    return ((((uint32_t)CTSU_VERSION_MAJOR) << 16) | (uint32_t)CTSU_VERSION_MINOR);
}
/***********************************************************************************************************************
End of function R_CTSU_GetVersion
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: R_CTSU_Open
* Description  : Check parameters being used, Power up CTSU, and initialize an unused CTSU operation handle.
* Arguments    :
* @param p_usr_hdl    Location where the handle number is returned.
* @param p_ctsu_cfg Parameters that should be used for this handle.
* Return Value :
* @return @ref CTSU_SUCCESS, @ref CTSU_ERR_INVALID_PARAM, @ref CTSU_ERR_INSUFFICIENT_MEMORY, @ref CTSU_ERR_LOCKED
***********************************************************************************************************************/
ctsu_err_t R_CTSU_Open(uint32_t* p_usr_hdl, ctsu_cfg_t const * const p_ctsu_cfg)
{
#if (CTSU_CFG_PARAM_CHECKING_ENABLE > 0)
    /** a. (Optional) Perform error checking. */
    ASSERT(p_usr_hdl != NULL);
    ASSERT(p_ctsu_cfg != NULL);
    ASSERT(p_ctsu_cfg->pclkb_hz >= 4000000);
    ASSERT(p_ctsu_cfg->p_ctsu_settings != NULL);
    ASSERT(p_ctsu_cfg->p_sensor_data != NULL);
    ASSERT(p_ctsu_cfg->p_sensor_settings != NULL);
    ASSERT(p_ctsu_cfg->p_ctsu_settings->ctsucr1.byte != 0);
#if (CTSU_CFG_ENABLE_MEASUREMENT_CORRECTION > 0)
    ASSERT(p_ctsu_cfg->p_correction_ideal_sensor_ico > 0);
    ASSERT(p_ctsu_cfg->correction_ctsuso_delta != 0);
#endif
#endif /*(CTSU_CFG_PARAM_CHECKING_ENABLE > 0) */
    ctsu_ctrl_blk_t ctrl_blk =
    {
            .p_ctsu_settings    = p_ctsu_cfg->p_ctsu_settings,
            .p_sensor_settings  = p_ctsu_cfg->p_sensor_settings,
            .p_sensor_data      = p_ctsu_cfg->p_sensor_data,
            .pclkb_hz           = p_ctsu_cfg->pclkb_hz,
            .p_callback         = p_ctsu_cfg->p_callback,
            .mode               = (ctsu_mode_t)(p_ctsu_cfg->p_ctsu_settings->ctsucr1.byte >> 6),
            .num_sensors        = calculate_sensor_count(ctrl_blk.mode, p_ctsu_cfg->p_ctsu_settings),
            .num_wr_irq_requests = ctrl_blk.num_sensors,
            .num_rd_irq_requests = ctrl_blk.num_sensors * ((CTSU_MODE_MUTUAL_CAPACITANCE == ctrl_blk.mode) ? 2 : 1),
            .open = OPEN,
            .upper_level_count = 0,
            .p_correction_ideal_sensor_ico = p_ctsu_cfg->p_correction_ideal_sensor_ico,
            .p_linear_interpolation_table = &p_ctsu_cfg->p_correction_buffer[0],
            .p_linear_interpolation_coeff = &p_ctsu_cfg->p_correction_buffer[61],
            .p_magnification_sensor = (int8_t* )(&p_ctsu_cfg->p_correction_buffer[61+61]),
            .correction_ctsuso_delta = 0,
            .correction_factor_sec_cur = 0,
            .tscal_pin = p_ctsu_cfg->tscal_pin,
    };
    R_BSP_SoftwareUnlock(&ctrl_blk.lock);
#if (CTSU_CFG_PARAM_CHECKING_ENABLE > 0)
    /** d. Check if mode and number of sensors are valid. */
    ASSERT((ctrl_blk.num_sensors > 0) && (ctrl_blk.num_sensors <= 64));
#endif
    for(uint32_t ctrl_blk_itr = 0; ctrl_blk_itr < CTSU_CFG_MAX_CONTROL_BLOCK_COUNT; ctrl_blk_itr++)
    {
        if (OPEN == pvt_ctsu_ctrl_blk[ctrl_blk_itr].open)
        {
            /* CTSU configuration is already open*/
            if(0==memcmp(&ctrl_blk, &pvt_ctsu_ctrl_blk[ctrl_blk_itr], offsetof(ctsu_ctrl_blk_t, open)))
            {
                /* Matching control block */
                pvt_ctsu_ctrl_blk[ctrl_blk_itr].upper_level_count += 1U;
                *p_usr_hdl = ctrl_blk_itr;
                return CTSU_SUCCESS;
            }
        }
    }

    for(uint32_t itr = 0; itr < CTSU_CFG_MAX_CONTROL_BLOCK_COUNT; itr++)
    {
        if (OPEN != pvt_ctsu_ctrl_blk[itr].open)
        {
            /* First unused handle */

            ctsu_ctrl_blk_t * p_ctrl_blk = &pvt_ctsu_ctrl_blk[itr];

            /* Lock the handle */
            if(R_BSP_SoftwareLock(&p_ctrl_blk->lock) == false)
            {
                /* Couldn't get the lock. */
                return CTSU_ERR_LOCKED;
            }
            memcpy(p_ctrl_blk, &ctrl_blk, sizeof(ctrl_blk));
            setup_dtc_tcb_chain(p_ctrl_blk);

            /* ********************* */
            /** j. Power ON the CTSU */
            /* ********************* */
            hw_ctsu_poweron();

            /* ************************************ */
            /** k. Enable CTSU interrupt operation. */
            /* ************************************ */
            IR(CTSU,CTSUWR)= 0;
            IR(CTSU,CTSURD)= 0;
            IR(CTSU,CTSUFN)= 0;
            IPR(CTSU,CTSUWR)= CTSU_CFG_INTERRUPT_PRIORITY_LEVEL;
            IPR(CTSU,CTSURD)= CTSU_CFG_INTERRUPT_PRIORITY_LEVEL;
            IPR(CTSU,CTSUFN)= CTSU_CFG_INTERRUPT_PRIORITY_LEVEL;
            IEN(CTSU,CTSUWR)= 1;
            IEN(CTSU,CTSURD)= 1;
            IEN(CTSU,CTSUFN)= 1;

            /** l. Indicate handle has been successfully opened and return the handle number to user. */
            p_ctrl_blk->upper_level_count = 1;
            *p_usr_hdl = itr;

            R_BSP_SoftwareUnlock(&p_ctrl_blk->lock);
            return CTSU_SUCCESS;
        }
    }
    return CTSU_ERR_INSUFFICIENT_MEMORY;
}
/***********************************************************************************************************************
End of function R_CTSU_Open
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_CTSU_Close
* Description  : Close a previously successfully opened handle.
* Arguments    :
* @param usr_hdl Handle identifier provided to user by the @ref R_CTSU_Open function.
* Return Value :
* @return @ref CTSU_SUCCESS, @ref CTSU_ERR_LOCKED.
***********************************************************************************************************************/
ctsu_err_t R_CTSU_Close(uint32_t usr_hdl)
{
#if (CTSU_CFG_PARAM_CHECKING_ENABLE > 0)
    /** (Optional) Perform error checking. */
    ASSERT( CTSU_CFG_MAX_CONTROL_BLOCK_COUNT > usr_hdl);
    ASSERT( OPEN == pvt_ctsu_ctrl_blk[usr_hdl].open);
#endif
    /** a. Check if hardware is scanning with this handle */
    if( (false == R_BSP_SoftwareLock(&pvt_ctsu_ctrl_blk[usr_hdl].lock))
            && (pvt_current_scan_handle_index == usr_hdl) )
    {
        /* Hardware is busy scanning with this handle.
                    Buffer should not be read at this time. */
        return CTSU_ERR_LOCKED;
    }

    /** b. Clean the handle information using memset. */
#if (CTSU_CFG_CHECK_UPPER_LEVEL_COUNT==true)
    if (0 == (--pvt_ctsu_ctrl_blk[usr_hdl].upper_level_count))
#endif
    {
        /* No more upper levels are using this control block */
        memset(&pvt_ctsu_ctrl_blk[usr_hdl], 0, sizeof(ctsu_ctrl_blk_t));
    }
#if (CTSU_CFG_CHECK_UPPER_LEVEL_COUNT==true)
    else
#endif
    {
        /* Unlock handle because we locked it in step a. */
        R_BSP_SoftwareUnlock(&pvt_ctsu_ctrl_blk[usr_hdl].lock);
    }

    /** c. Exit if any other handles are open. */
    for(uint32_t ctrl_blk_itr = 0; ctrl_blk_itr < CTSU_CFG_MAX_CONTROL_BLOCK_COUNT; ctrl_blk_itr++)
    {
        /* Check for open handles */
        if (OPEN == pvt_ctsu_ctrl_blk[ctrl_blk_itr].open)
        {
            return CTSU_SUCCESS;
        }
    }

    /** d. If no more handles are open */
    pvt_current_scan_handle_index = UINT32_MAX;

    /* ********************************************** */
    /* Turn-off CTSU if all handles have been closed. */
    /* ********************************************** */
    /* Stop all conversions */
    HW_CTSU_CTSUCR0SetBitCTSUSTRT(0);

    /** 1. Re-initialize CTSU SFRs*/
    HW_CTSU_CTSUCR0SetBitCTSUINIT(1);

    /* Reset the SFRs to un-used state */
    HW_CTSU_CTSUCR0Set(0);
    HW_CTSU_CTSUCR1Set(0);
    HW_CTSU_CTSUSDPRSSet(0);
    HW_CTSU_CTSUDCLKCSet(0);
    HW_CTSU_CTSUSSTSet(0);
    HW_CTSU_CTSUCHAC0Set(0);
    HW_CTSU_CTSUCHAC1Set(0);
#if !defined(BSP_MCU_RX113)
    HW_CTSU_CTSUCHAC2Set(0);
    HW_CTSU_CTSUCHAC3Set(0);
    HW_CTSU_CTSUCHAC4Set(0);
#endif
    HW_CTSU_CTSUCHTRC0Set(0);
    HW_CTSU_CTSUCHTRC1Set(0);
#if !defined(BSP_MCU_RX113)
    HW_CTSU_CTSUCHTRC2Set(0);
    HW_CTSU_CTSUCHTRC3Set(0);
    HW_CTSU_CTSUCHTRC4Set(0);
#endif
#if (CTSU_CFG_DTC_TRANSFER_ENABLE == true)
    /** 2. Disable use of the DTC to move data when CTSUWR interrupt occurs */
    DTCE(CTSU,CTSUWR) = 0;
    /** 3. Disable use of the DTC to move data when CTSURD interrupt occurs */
    DTCE(CTSU,CTSURD) = 0;
#endif
    /* ************************************* */
    /** 4. Disable CTSU interrupt operation. */
    /* ************************************* */
    IR(CTSU,CTSUWR)= 0;
    IR(CTSU,CTSURD)= 0;
    IR(CTSU,CTSUFN)= 0;
    IPR(CTSU,CTSUWR)= 0;
    IPR(CTSU,CTSURD)= 0;
    IPR(CTSU,CTSUFN)= 0;
    IEN(CTSU,CTSUWR)= 0;
    IEN(CTSU,CTSURD)= 0;
    IEN(CTSU,CTSUFN)= 0;

    /* ******************************************************* */
    /** 5. Unlock Hardware (if it was left as locked somehow). */
    /* ******************************************************* */
    R_BSP_HardwareUnlock(BSP_LOCK_CTSU);

    /* ********************** */
    /** 6. Power OFF the CTSU */
    /* ********************** */
    hw_ctsu_poweroff();

    return CTSU_SUCCESS;
}
/***********************************************************************************************************************
End of function R_CTSU_Close
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_CTSU_Scan
* Description  : Enable scanning of a successfully opened handle.
* Arguments    :
* @param usr_hdl Handle number provided to user by the @ref R_CTSU_Open function.
* Return Value :
* @return @ref CTSU_SUCCESS, @ref CTSU_ERR_CTSU_OPERATIONAL, @ref CTSU_ERR_LOCKED.
***********************************************************************************************************************/
ctsu_err_t R_CTSU_Scan(uint32_t usr_hdl)
{
    bool lock_acquire;
    ctsu_callback_arg_t args;
#if (CTSU_CFG_PARAM_CHECKING_ENABLE > 0)
    /** a. (Optional) Perform error checking. */
    ASSERT( CTSU_CFG_MAX_CONTROL_BLOCK_COUNT > usr_hdl);
    ASSERT( OPEN == pvt_ctsu_ctrl_blk[usr_hdl].open);
    ASSERT( BSP_PCLKB_HZ == pvt_ctsu_ctrl_blk[usr_hdl].pclkb_hz );
#endif
    /** b. Get the handle */
    ctsu_ctrl_blk_t *const p_ctrl_blk = &pvt_ctsu_ctrl_blk[usr_hdl];

    args.handle_num = usr_hdl;
    args.info = 0;
    args.event_mask = 0;

    /** c. Lock the hardware. */
    if(false == R_BSP_HardwareLock(BSP_LOCK_CTSU))
    {
        /* Hardware is busy scanning */
        return CTSU_ERR_CTSU_OPERATIONAL;
    }

    /** d. Lock the handle as the @ref st_ctsu_ctrl_blk::p_sensor_data buffer is about to be updated. */
    lock_acquire = R_BSP_SoftwareLock(&p_ctrl_blk->lock);
    if (false == lock_acquire)
    {
        return CTSU_ERR_LOCKED;
    }

    if ((pvt_current_scan_handle_index != usr_hdl) || (true == p_ctrl_blk->reload))
    {
        /** e. If handle is different from previous */
        /** 1. Load new CTSU SFR settings. */
        if(HW_CTSU_CTSUCR1GetBitCTSUMD() != p_ctrl_blk->mode)
        {
            /* Mode will be changed. Notify the user */
            args.event_mask |= CTSU_EVENT_SFRS_CHANGED;
        }
        HW_CTSU_CTSUCR1Set(p_ctrl_blk->p_ctsu_settings->ctsucr1.byte);
        HW_CTSU_CTSUSDPRSSet(p_ctrl_blk->p_ctsu_settings->ctsusdprs.byte);
        HW_CTSU_CTSUDCLKCSet(p_ctrl_blk->p_ctsu_settings->ctsudclkc.byte);
        HW_CTSU_CTSUSSTSet(p_ctrl_blk->p_ctsu_settings->ctsusst.byte);
        HW_CTSU_CTSUCHAC0Set(p_ctrl_blk->p_ctsu_settings->ctsuchac0.byte);
        HW_CTSU_CTSUCHAC1Set(p_ctrl_blk->p_ctsu_settings->ctsuchac1.byte);
#if !defined(BSP_MCU_RX113)
        /* RX113 does not have the following registers. Skip over them. */
        HW_CTSU_CTSUCHAC2Set(p_ctrl_blk->p_ctsu_settings->ctsuchac2.byte);
        HW_CTSU_CTSUCHAC3Set(p_ctrl_blk->p_ctsu_settings->ctsuchac3.byte);
        HW_CTSU_CTSUCHAC4Set(p_ctrl_blk->p_ctsu_settings->ctsuchac4.byte);
#endif
        HW_CTSU_CTSUCHTRC0Set(p_ctrl_blk->p_ctsu_settings->ctsuchtrc0.byte);
        HW_CTSU_CTSUCHTRC1Set(p_ctrl_blk->p_ctsu_settings->ctsuchtrc1.byte);
#if !defined(BSP_MCU_RX113)
        /* RX113 does not have the following registers. Skip over them. */
        HW_CTSU_CTSUCHTRC2Set(p_ctrl_blk->p_ctsu_settings->ctsuchtrc2.byte);
        HW_CTSU_CTSUCHTRC3Set(p_ctrl_blk->p_ctsu_settings->ctsuchtrc3.byte);
        HW_CTSU_CTSUCHTRC4Set(p_ctrl_blk->p_ctsu_settings->ctsuchtrc4.byte);
#endif
        HW_CTSU_CTSUCR0Set((p_ctrl_blk->p_ctsu_settings->ctsucr0.byte & (uint8_t)0xFE));
#if (CTSU_CFG_DTC_TRANSFER_ENABLE == true)
        /* Get the DTC address */
        uint32_t* dtc_base_address = (uint32_t*)DTC.DTCVBR;
        /** 2. Set the DTC vectors to point to the right locations */
        dtc_base_address[IR_CTSU_CTSUWR] = (uint32_t)&p_ctrl_blk->ctsu_wr[0];
        dtc_base_address[IR_CTSU_CTSURD] = (uint32_t)&p_ctrl_blk->ctsu_rd[0];
#endif
        /** 3. Set the scan handle index (which is used in the ISRs) */
        pvt_current_scan_handle_index = usr_hdl;
        if (true == p_ctrl_blk->reload)
        {
            p_ctrl_blk->reload = false;
        }
    }
#if (CTSU_CFG_DTC_TRANSFER_ENABLE == true) && (CTSU_CFG_DTC_CHAIN_TRANSFERS == false)
    _setup_dtc_tcb0(p_ctrl_blk);
#endif

    if(1 == HW_CTSU_CTSUCR0GetBitCTSUSNZ())
    {
        /* Refer note on CTSUSNZ Bit (CTSU Wait State Power-Saving Enable) about 16uS delay */
        HW_CTSU_CTSUCR0SetBitCTSUSNZ(0);
        args.event_mask |= CTSU_EVENT_SFRS_CHANGED;
    }

    args.info = p_ctrl_blk->num_sensors;
    args.event_mask |= CTSU_EVENT_STARTING_SCAN;
    if (NULL != p_ctrl_blk->p_callback)
    {
        /** f. If User has provided a callback, Notify User. */
        p_ctrl_blk->p_callback(&args);
    }

    /** g. Start CTSU scan operation. */
    HW_CTSU_CTSUCR0SetBitCTSUINIT(1);
    HW_CTSU_CTSUCR0SetBitCTSUSTRT(1);

    return CTSU_SUCCESS;
}
/***********************************************************************************************************************
End of function R_CTSU_Scan
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_CTSU_Read
* Description  : Read the results generated by a CTSU scan operation.
* Arguments    :
* @param usr_hdl Handle number provided to user by the @ref R_CTSU_Open function.
* @param p_arg Argument providing information about information to be read from latest scan.
* Return Value :
* @return @ref CTSU_SUCCESS, @ref CTSU_ERR_INVALID_CMD, @ref CTSU_ERR_INVALID_PARAM, @ref CTSU_ERR_LOCKED.
***********************************************************************************************************************/
ctsu_err_t R_CTSU_Read(uint32_t usr_hdl, ctsu_read_t * p_arg)
{
    uint16_t* p_src;
    ctsu_err_t ctsu_err = CTSU_SUCCESS;
    ctsu_read_cmd_t cmd = p_arg->ctsu_result;
    size_t required_size = 0;
#if (CTSU_CFG_PARAM_CHECKING_ENABLE > 0)
    ASSERT(CTSU_CFG_MAX_CONTROL_BLOCK_COUNT > usr_hdl);
    ASSERT(OPEN == pvt_ctsu_ctrl_blk[usr_hdl].open);
    ASSERT(NULL != p_arg->p_dest);
    ASSERT(0 < p_arg->size);
    ASSERT(CTSU_READ_LAST > p_arg->ctsu_result);
    ASSERT((NULL != p_arg->sensor_offset));
    ASSERT(0 < p_arg->count);
#endif
    uint16_t count = p_arg->count;
    uint16_t* p_dest = p_arg->p_dest;
    uint16_t const * const p_offsets = p_arg->sensor_offset;

    /** a. Get the handle */
    ctsu_ctrl_blk_t* p_ctrl_blk = &pvt_ctsu_ctrl_blk[usr_hdl];
    if (CTSU_MODE_SELF_CAPACITANCE == p_ctrl_blk->mode)
    {
        if ((CTSU_READ_SENSOR_COUNT_SECONDARY == cmd) || (CTSU_READ_REFERENCE_COUNT_SECONDARY == cmd))
        {
            return CTSU_ERR_INVALID_CMD;
        }
    }

    /** b. Calculate memory required to copy */
    if (CTSU_READ_BUFFER_ALL == cmd)
    {
        required_size = ((p_ctrl_blk->num_sensors * ((CTSU_MODE_MUTUAL_CAPACITANCE == p_ctrl_blk->mode) ? (4) : (2)))
                * sizeof(uint16_t));
    }
    else if (cmd < CTSU_READ_LAST)
    {
        required_size = (count * sizeof(uint16_t));
    }
    else
    {
        return CTSU_ERR_INVALID_CMD;
    }
#if (CTSU_CFG_PARAM_CHECKING_ENABLE > 0)
    ASSERT(required_size <= p_arg->size);
#endif
    if (required_size > p_arg->size)
    {
        /** Insufficient memory */
        return CTSU_ERR_INVALID_PARAM;
    }

    /** c. Check if the handle is in use*/
    if ((false == R_BSP_SoftwareLock(&p_ctrl_blk->lock)) && (pvt_current_scan_handle_index == usr_hdl))
    {
        /* Hardware is busy scanning with this handle.
         Buffer should not be read at this time. */
        return CTSU_ERR_LOCKED;
    }

    /** d. Store the results into the user provided buffer location. */
    if (CTSU_READ_BUFFER_ALL == cmd)
    {
        /* Requesting everything (As is). */
        memcpy(p_arg->p_dest, p_ctrl_blk->p_sensor_data, required_size);
    }
    else if (CTSU_READ_SENSOR_OUTPUT == cmd)
    {
        p_src = (uint16_t*) p_ctrl_blk->p_sensor_data;
        ctsu_read_output(p_ctrl_blk->mode, count, p_offsets, p_dest, p_src);
    }
    else
    {
        switch (cmd)
        {
            case CTSU_READ_SENSOR_COUNT_PRIMARY :
                p_src = ((uint16_t*) p_ctrl_blk->p_sensor_data + OFFSET_PRIMARY_SENSOR_COUNT);
            break;
            case CTSU_READ_REFERENCE_COUNT_PRIMARY :
                p_src = (uint16_t*) p_ctrl_blk->p_sensor_data + OFFSET_PRIMARY_REFERENCE_COUNT;
            break;
            case CTSU_READ_SENSOR_COUNT_SECONDARY :
                p_src = (uint16_t*) p_ctrl_blk->p_sensor_data + OFFSET_SECONDARY_SENSOR_COUNT;
            break;
            case CTSU_READ_REFERENCE_COUNT_SECONDARY :
                p_src = (uint16_t*) p_ctrl_blk->p_sensor_data + OFFSET_SECONDARY_REFERENCE_COUNT;
            break;
            default :
                p_src = FIT_NO_PTR;
            break;
        }
        ctsu_read_counter(p_ctrl_blk->mode, count, p_offsets, p_dest, p_src);
    }

    /** e. Unlock the handle so it can be used by other CTSU functions.*/
    R_BSP_SoftwareUnlock(&p_ctrl_blk->lock);
    return ctsu_err;
}
/***********************************************************************************************************************
End of function R_CTSU_Read
***********************************************************************************************************************/

/**
 *

 */
/***********************************************************************************************************************
* Function Name: R_CTSU_Control
* Description  : Change operating parameters of the specified handle.
* Arguments    :
* @param usr_hdl Handle number provided to user by the @ref R_CTSU_Open function.
* @param p_arg Arguments specifying details of things to change.
* Return Value : @return @ref CTSU_SUCCESS, @ref CTSU_ERR_INVALID_PARAM, @ref CTSU_ERR_INVALID_CMD.
***********************************************************************************************************************/
ctsu_err_t R_CTSU_Control(uint32_t usr_hdl, ctsu_control_arg_t* p_arg)
{
#if (CTSU_CFG_PARAM_CHECKING_ENABLE > 0)
    /** (Optional) Perform error checking. */
    ASSERT( CTSU_CFG_MAX_CONTROL_BLOCK_COUNT > usr_hdl );
    ASSERT( OPEN == pvt_ctsu_ctrl_blk[usr_hdl].open );
    ASSERT( p_arg != NULL);
    ASSERT( p_arg->p_context != NULL);
#endif
    ctsu_cmd_t cmd = p_arg->cmd;
#if (CTSU_CFG_PARAM_CHECKING_ENABLE > 0)
    ASSERT(CTSU_CMD_LAST > cmd);
#endif
    /** a. Get the handle */
    ctsu_ctrl_blk_t* p_ctrl_blk = &pvt_ctsu_ctrl_blk[usr_hdl];

    /** b. Read or write value depending upon GET/SET command respectively. */
    /* ************** */
    /* Write commands */
    /* ************** */
    if(CTSU_CMD_GET_ONE_ARG_LAST > cmd)
    {
        ctsu_handle_register_value_get(p_ctrl_blk, cmd, p_arg->p_context);
    }
    else if(CTSU_CMD_GET_TWO_ARG_LAST > cmd)
    {
        ctsu_control_arg_ext_t* p_context = (ctsu_control_arg_ext_t*)p_arg->p_context;
#if (CTSU_CFG_PARAM_CHECKING_ENABLE > 0)
        ASSERT(p_ctrl_blk->num_sensors > p_context->offset);
#endif
        ctsu_handle_register_16bit_get(p_ctrl_blk, cmd, p_context->offset, &p_context->value);
    }
    else if(CTSU_CMD_SET_ONE_ARG_LAST > cmd)
    {
        ctsu_handle_register_value_set(p_ctrl_blk, cmd, p_arg->p_context);
        p_ctrl_blk->reload = true;
    }
    else
    {
        /* (CTSU_CMD_SET_TWO_ARG_LAST > cmd)*/
        ctsu_control_arg_ext_t* p_context = (ctsu_control_arg_ext_t*)p_arg->p_context;
#if (CTSU_CFG_PARAM_CHECKING_ENABLE > 0)
        ASSERT(p_ctrl_blk->num_sensors > p_context->offset);
#endif
        ctsu_handle_register_16bit_set(p_ctrl_blk, cmd, p_context->offset, p_context->value);
        p_ctrl_blk->reload = true;
    }

    return CTSU_SUCCESS;
}
/***********************************************************************************************************************
End of function R_CTSU_Control
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: R_CTSU_Calibrate
* Description  : Calibrate the CTSU handle. I.e. Bring sensor count ~~ reference counts.
* Arguments    :
* @param usr_hdl Handle number provided to user by the @ref R_CTSU_Open function.
* Return Value :
* @return CTSU error status.
***********************************************************************************************************************/
ctsu_err_t R_CTSU_Calibrate(uint32_t usr_hdl)
{
    ctsu_err_t ctsu_err = CTSU_SUCCESS;
#if (CTSU_CFG_PARAM_CHECKING_ENABLE > 0)
    ASSERT( CTSU_CFG_MAX_CONTROL_BLOCK_COUNT > usr_hdl);
    ASSERT( OPEN == pvt_ctsu_ctrl_blk[usr_hdl].open);
    ASSERT( 64 >= pvt_ctsu_ctrl_blk[usr_hdl].num_sensors);
#endif

#if (CTSU_CFG_CALIBRATION_ENABLE > 0)
    uint16_t ctsuso;
    uint32_t sensor_itr;
    uint16_t sensor_count;
    uint16_t reference_count;
    uint16_t num_sensors;
    uint16_t num_sensors_tuned = 0;
    uint64_t tuned_sensor_mask = 0;

    /* Get the handle */
    ctsu_ctrl_blk_t *const p_ctrl_blk = &pvt_ctsu_ctrl_blk[usr_hdl];

    ctsu_err = ctsu_correction_factor(usr_hdl);

    num_sensors = p_ctrl_blk->num_sensors;

    while(num_sensors_tuned < num_sensors)
    {
        ctsu_err = R_CTSU_Scan(usr_hdl);

        if (CTSU_SUCCESS != ctsu_err)
        {
            /* Scan unable to start. */
            return ctsu_err;
        }

        /* Wait until scan is complete. Grab lock for manipulating buffers. */
        while(false == R_BSP_SoftwareLock(&p_ctrl_blk->lock))
        {
            ;
        }

        if(p_ctrl_blk->scan_errors & CTSU_ERR_CTSU_ICOMP)
        {
            /* Problem with TSCAP voltage. */
            ctsu_err = CTSU_ERR_CTSU_ICOMP;
        }

        num_sensors_tuned = 0;

        for(sensor_itr = 0; (sensor_itr < num_sensors) ; sensor_itr++)
        {
            if((((uint64_t)1<<sensor_itr) & tuned_sensor_mask) == ((uint64_t)1<<sensor_itr))
            {
                /* Sensor is already tuned */
                num_sensors_tuned += 1U;
                continue;
            }

            if (CTSU_MODE_SELF_CAPACITANCE == p_ctrl_blk->mode)
            {
                sensor_count = ((uint16_t*)(p_ctrl_blk->p_sensor_data))[(2*sensor_itr) + 0];
                reference_count = ((uint16_t*)(p_ctrl_blk->p_sensor_data))[(2*sensor_itr) + 1];
            }
            else if (CTSU_MODE_MUTUAL_CAPACITANCE == p_ctrl_blk->mode)
            {
                sensor_count = ((uint16_t*) (p_ctrl_blk->p_sensor_data))[(4 * sensor_itr) + 0];
                reference_count = ((uint16_t*) (p_ctrl_blk->p_sensor_data))[(4 * sensor_itr) + 1];
            }
            else
            {
                ;
            }

            ctsuso = p_ctrl_blk->p_sensor_settings[sensor_itr].ctsuso0 & 0x3FF;

            if((0x3FF <= ctsuso) || (ctsuso <= 0))
            {
                /* Offset limit reached. Can't make any changes. */
                num_sensors_tuned += 1U;
                tuned_sensor_mask |= ((uint64_t)1U << sensor_itr);
                continue;
            }

            if(reference_count < (sensor_count - TUNING_UPPER_LIMIT) )
            {
                /* Need to increase sensor offset current amount to bring sensor within range of operation. */
                ctsuso += 1U;
            }
            else if( reference_count > (sensor_count + TUNING_LOWER_LIMIT) )
            {
                /* Need to reduce sensor offset current amount to bring sensor within range of operation. */
                ctsuso -= 1U;
            }
            else
            {
                /* Sensor is within range of operation. */
                num_sensors_tuned += 1;
                tuned_sensor_mask |= ((uint64_t)1U << sensor_itr);
                continue;
            }

            p_ctrl_blk->p_sensor_settings[sensor_itr].ctsuso0 &= (~0x3FF);
            p_ctrl_blk->p_sensor_settings[sensor_itr].ctsuso0 |= ctsuso;

        }
        R_BSP_SoftwareUnlock(&p_ctrl_blk->lock);
    }
#endif    /*(CTSU_CFG_CALIBRATION_ENABLE > 0) */


    return ctsu_err;
}
/***********************************************************************************************************************
End of function R_CTSU_Calibrate
***********************************************************************************************************************/

/**********************************************************************************************************************/
/*                                      Local function definitions                                                    */
/**********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: hw_ctsu_poweron
* Description  : Enables CTSU operation by disabling low-power operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void hw_ctsu_poweron (void)
{
    /** a. Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable (BSP_REG_PROTECT_LPC_CGC_SWR);

    /** b. Bring module out of stop state. */
    MSTP(CTSU) = 0;

    /** c. Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable (BSP_REG_PROTECT_LPC_CGC_SWR);
}
/***********************************************************************************************************************
End of function hw_ctsu_poweron
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: hw_ctsu_poweroff
* Description  : Disables CTSU operation by enabling low-power operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void hw_ctsu_poweroff (void)
{
    /** a. Enable writing to MSTP registers. */
    R_BSP_RegisterProtectDisable (BSP_REG_PROTECT_LPC_CGC_SWR);

    /** b. Bring module in to stop state. */
    MSTP(CTSU) = 1;

    /** c. Disable writing to MSTP registers. */
    R_BSP_RegisterProtectEnable (BSP_REG_PROTECT_LPC_CGC_SWR);
}
/***********************************************************************************************************************
End of function hw_ctsu_poweroff
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: setup_dtc_tcb_chain
* Description  : Setup DTC Transfer Control Block Chain.
* Arguments    :
* @param p_ctrl_blk - Pointer to CTSU Control Block.
* Return Value :
* @return @ref CTSU_SUCCESS, @ref CTSU_ERR_INVALID_PARAM,
***********************************************************************************************************************/
static ctsu_err_t setup_dtc_tcb_chain(ctsu_ctrl_blk_t *const p_ctrl_blk)
{

#if (CTSU_CFG_DTC_TRANSFER_ENABLE == true)
#if (CTSU_CFG_PARAM_CHECKING_ENABLE == true)
    /** c. Check if Data transfer controller (DTC) is configured. */
    ASSERT(DTC.DTCVBR!=NULL);
#endif
    /* *************************************************** */
    /** f. Initialize the handle's DTC TCB for CTSUWR IRQ. */
    /* *************************************************** */
    memcpy(p_ctrl_blk->ctsu_wr, &ctsu_wr_init, sizeof(ctsu_wr_init));

    /* **************************************************** */
    /** g. Initialize the handle's DTC TCB for CTSU RD IRQ. */
    /* **************************************************** */
    memcpy(p_ctrl_blk->ctsu_rd, &ctsu_rd_init, sizeof(ctsu_rd_init));
#if (CTSU_CFG_DTC_CHAIN_TRANSFERS == true)
    /* Set up all the left over parameters */
    p_ctrl_blk->ctsu_wr[0].SECOND_LWORD.SAR = (uint32_t)p_ctrl_blk->p_sensor_settings;
    p_ctrl_blk->ctsu_wr[0].FOURTH_LWORD.REG.CRB.WORD = 0xFFFF;
    p_ctrl_blk->ctsu_wr[0].FOURTH_LWORD.REG.CRB.WORD = p_ctrl_blk->num_wr_irq_requests;
    p_ctrl_blk->ctsu_wr[1].SECOND_LWORD.SAR = (uint32_t)&p_ctrl_blk->p_sensor_settings;
    p_ctrl_blk->ctsu_wr[1].THIRD_LWORD.DAR = (uint32_t)&p_ctrl_blk->ctsu_wr[0].SECOND_LWORD.SAR;
    p_ctrl_blk->ctsu_wr[2].SECOND_LWORD.SAR = (uint32_t)&p_ctrl_blk->num_wr_irq_requests;
    p_ctrl_blk->ctsu_wr[2].THIRD_LWORD.DAR = (uint32_t)&p_ctrl_blk->ctsu_wr[0].FOURTH_LWORD.REG.CRB.WORD;

    p_ctrl_blk->ctsu_rd[0].THIRD_LWORD.DAR = (uint32_t)p_ctrl_blk->p_sensor_data;
    p_ctrl_blk->ctsu_rd[0].FOURTH_LWORD.REG.CRB.WORD = 0xFFFF;
    p_ctrl_blk->ctsu_rd[0].FOURTH_LWORD.REG.CRB.WORD = p_ctrl_blk->num_rd_irq_requests;
    p_ctrl_blk->ctsu_rd[1].SECOND_LWORD.SAR = (uint32_t)&p_ctrl_blk->p_sensor_data;
    p_ctrl_blk->ctsu_rd[1].THIRD_LWORD.DAR = (uint32_t)&p_ctrl_blk->ctsu_rd[0].THIRD_LWORD.DAR;
    p_ctrl_blk->ctsu_rd[2].SECOND_LWORD.SAR = (uint32_t)&p_ctrl_blk->num_rd_irq_requests;
    p_ctrl_blk->ctsu_rd[2].THIRD_LWORD.DAR = (uint32_t)&p_ctrl_blk->ctsu_rd[0].FOURTH_LWORD.REG.CRB.WORD;
#else
    _setup_dtc_tcb0(p_ctrl_blk);
#endif
    /** h. Use the DTC to move data when CTSUWR interrupt occurs */
    DTCE(CTSU,CTSUWR) = 1;
    /** i. Use the DTC to move data when CTSURD interrupt occurs */
    DTCE(CTSU,CTSURD) = 1;
#endif

    return CTSU_SUCCESS;
}
/***********************************************************************************************************************
End of function setup_dtc_tcb_chain
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: ctsu_read_output
* Description  : Read the counter value representing capacitance indexed by sensor offsets.
* Arguments    :
* @param mode - CTSU mode of operation. Refer @ref ctsu_mode_t.
* @param num_sensors - Number of sensors provided by argument offsets.
* @param offsets - Array of sensor offsets to read.
* @param p_dest - Location where copy of results should be made.
* @param p_src - Location where results are output by the CTSU.
* Return Value : None
***********************************************************************************************************************/
static void ctsu_read_output(ctsu_mode_t mode, uint16_t num_sensors, uint16_t const * const offsets,void* p_dest, void* p_src)
{
    uint32_t itr;
    uint16_t* p_result_dest = p_dest;
    uint16_t* p_result_src  = p_src;
    uint16_t sen_cnt_2 = 0;
    uint16_t sen_cnt_1 = 0;
    uint16_t offset = 0;

    for(itr = 0; itr < num_sensors; itr++)
    {
        offset = offsets[itr] * ((CTSU_MODE_MUTUAL_CAPACITANCE == mode) ? 4 : 2);
        sen_cnt_1 = p_result_src[offset+OFFSET_PRIMARY_SENSOR_COUNT];
        sen_cnt_2 = p_result_src[offset+OFFSET_SECONDARY_SENSOR_COUNT];

        p_result_dest[itr] =
                ((CTSU_MODE_MUTUAL_CAPACITANCE == mode) ?
                (uint16_t)(sen_cnt_2 - sen_cnt_1):(sen_cnt_1));
    }
}
/***********************************************************************************************************************
End of function ctsu_read_output
***********************************************************************************************************************/

/**
 * Read Counter values at for sensor.
 * @param mode CTSU mode of operation.
 * @param num_sensors Number of sensors provided by argument offsets.
 * @param offsets Array of sensor offsets to read.
 * @param p_dest Location where copy of results should be made.
 * @param p_src    Location where results are output by the CTSU.
 */
/***********************************************************************************************************************
* Function Name: ctsu_read_counter
* Description  : Counter values at for sensor.
* Arguments    :
* @param mode - CTSU mode of operation.
* @param num_sensors - Number of sensors provided by argument offsets.
* @param offsets - Array of sensor offsets to read.
* @param p_dest - Location where copy of results should be made.
* @param p_src - Location where results are output by the CTSU.
* Return Value : None
***********************************************************************************************************************/
static void ctsu_read_counter(ctsu_mode_t mode, uint16_t num_sensors, uint16_t const * const offsets,void* p_dest, void* p_src)
{
    uint16_t* p_result_dest = p_dest;
    uint16_t* p_result_src  = p_src;
    uint16_t offset = 0;
    uint32_t itr;
    for(itr = 0; itr < num_sensors; itr++)
    {
        offset = offsets[itr] * ((CTSU_MODE_MUTUAL_CAPACITANCE == mode) ? 4 : 2);
        p_result_dest[itr] = p_result_src[offset];
    }
}
/***********************************************************************************************************************
End of function ctsu_read_counter
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: ctsu_handle_register_value_get
* Description  : Get value stored for parameter in control block.
* Arguments    :
* @param p_ctrl_blk - Control block index to read
* @param cmd Control - command issued
* @param p_dest - Location where result must be copied
* Return Value : None
***********************************************************************************************************************/
static void ctsu_handle_register_value_get(ctsu_ctrl_blk_t* p_ctrl_blk, ctsu_cmd_t cmd, void* p_dest)
{
    uint64_t chac_mask;
    uint64_t chtrc_mask;
    switch (cmd)
    {
        case CTSU_CMD_GET_CTSUCR0 :
            *(uint8_t*) p_dest = p_ctrl_blk->p_ctsu_settings->ctsucr0.byte;
            break;
        case CTSU_CMD_GET_CTSUCR1 :
            *(uint8_t*) p_dest = p_ctrl_blk->p_ctsu_settings->ctsucr1.byte;
            break;
        case CTSU_CMD_GET_CTSUSDPRS :
            *(uint8_t*) p_dest = p_ctrl_blk->p_ctsu_settings->ctsusdprs.byte;
            break;
        case CTSU_CMD_GET_CTSUSST :
            *(uint8_t*) p_dest = p_ctrl_blk->p_ctsu_settings->ctsusst.byte;
            break;
        case CTSU_CMD_GET_CTSUCHAC :
            chac_mask = 0;
            chac_mask |= p_ctrl_blk->p_ctsu_settings->ctsuchac4.byte;
            chac_mask <<= 8;
            chac_mask |= p_ctrl_blk->p_ctsu_settings->ctsuchac3.byte;
            chac_mask <<= 8;
            chac_mask |= p_ctrl_blk->p_ctsu_settings->ctsuchac2.byte;
            chac_mask <<= 8;
            chac_mask |= p_ctrl_blk->p_ctsu_settings->ctsuchac1.byte;
            chac_mask <<= 8;
            chac_mask |= p_ctrl_blk->p_ctsu_settings->ctsuchac0.byte;

            *(uint64_t*) p_dest = chac_mask;
            break;
        case CTSU_CMD_GET_CTSUCHTRC :
            chtrc_mask = 0;
            chtrc_mask |= p_ctrl_blk->p_ctsu_settings->ctsuchtrc4.byte;
            chtrc_mask <<= 8;
            chtrc_mask |= p_ctrl_blk->p_ctsu_settings->ctsuchtrc3.byte;
            chtrc_mask <<= 8;
            chtrc_mask |= p_ctrl_blk->p_ctsu_settings->ctsuchtrc2.byte;
            chtrc_mask <<= 8;
            chtrc_mask |= p_ctrl_blk->p_ctsu_settings->ctsuchtrc1.byte;
            chtrc_mask <<= 8;
            chtrc_mask |= p_ctrl_blk->p_ctsu_settings->ctsuchtrc0.byte;

            *(uint64_t*) p_dest = chtrc_mask;
            break;

        case CTSU_CMD_GET_CTSUDCLKC :
            *(uint8_t*) p_dest = p_ctrl_blk->p_ctsu_settings->ctsudclkc.byte;
            break;

        case CTSU_CMD_GET_RESULT_OUTPUT_BUFFER :
            *(void**) p_dest = p_ctrl_blk->p_sensor_data;
            break;

        case CTSU_CMD_GET_MODE :
            *(ctsu_mode_t*) p_dest = p_ctrl_blk->mode;
            break;

        case CTSU_CMD_GET_PCLK :
            *(uint32_t*) p_dest = p_ctrl_blk->pclkb_hz;
            break;

        case CTSU_CMD_GET_SENSOR_COUNT :
            *(uint16_t*) p_dest = p_ctrl_blk->num_sensors;
            break;

        case CTSU_CMD_GET_ERROR_INFORMATION :
            *(uint32_t*) p_dest = p_ctrl_blk->scan_errors;
            break;

        case CTSU_CMD_GET_CALLBACK :
            *(fit_callback_t*) p_dest = p_ctrl_blk->p_callback;
            break;

        default :

            break;
    }
}
/***********************************************************************************************************************
End of function ctsu_handle_register_value_get
***********************************************************************************************************************/

/**
 * Set value for parameter in control block.

 */
/***********************************************************************************************************************
* Function Name: ctsu_handle_register_value_set
* Description  : Does an example task. Making this longer just to see how it wraps. Making this long just to see how it
*                wraps.
* Arguments    :
* @param p_ctrl_blk - Control block index to read.
* @param cmd Control - command issued.
* @param value - Location where result must be copied.
* Return Value : count -
*                    How many entries were found
***********************************************************************************************************************/
static void ctsu_handle_register_value_set(ctsu_ctrl_blk_t* p_ctrl_blk, ctsu_cmd_t cmd, void* value)
{
    uint64_t chac = *(uint64_t*)value;
    uint64_t chtrc = *(uint64_t*)value;

    switch(cmd)
    {
        case CTSU_CMD_SET_CTSUCR0:
            p_ctrl_blk->p_ctsu_settings->ctsucr0.byte = *(uint8_t*)value;
            break;

        case CTSU_CMD_SET_CTSUCR1:
            p_ctrl_blk->p_ctsu_settings->ctsucr1.byte = *(uint8_t*)value;
            break;

        case CTSU_CMD_SET_CTSUSDPRS:
            p_ctrl_blk->p_ctsu_settings->ctsusdprs.byte = *(uint8_t*)value;
            break;

        case CTSU_CMD_SET_CTSUSST:
        p_ctrl_blk->p_ctsu_settings->ctsusst.byte = *(uint8_t*)value;
            break;

        case CTSU_CMD_SET_CTSUCHAC:
            p_ctrl_blk->p_ctsu_settings->ctsuchac0.byte = chac & 0xFF;
            chac >>= 8;
            p_ctrl_blk->p_ctsu_settings->ctsuchac1.byte = chac & 0xFF;
            chac >>= 8;
            p_ctrl_blk->p_ctsu_settings->ctsuchac2.byte = chac & 0xFF;
            chac >>= 8;
            p_ctrl_blk->p_ctsu_settings->ctsuchac3.byte = chac & 0xFF;
            chac >>= 8;
            p_ctrl_blk->p_ctsu_settings->ctsuchac4.byte = chac & 0xFF;
            break;

        case CTSU_CMD_SET_CTSUCHTRC:
            p_ctrl_blk->p_ctsu_settings->ctsuchtrc0.byte = chtrc & 0xFF;
            chtrc >>= 8;
            p_ctrl_blk->p_ctsu_settings->ctsuchtrc1.byte = chtrc & 0xFF;
            chtrc >>= 8;
            p_ctrl_blk->p_ctsu_settings->ctsuchtrc2.byte = chtrc & 0xFF;
            chtrc >>= 8;
            p_ctrl_blk->p_ctsu_settings->ctsuchtrc3.byte = chtrc & 0xFF;
            chtrc >>= 8;
            p_ctrl_blk->p_ctsu_settings->ctsuchtrc4.byte = chtrc & 0xFF;
            break;

        case CTSU_CMD_SET_CTSUDCLKC:
            p_ctrl_blk->p_ctsu_settings->ctsudclkc.byte = *(uint8_t*)value;
            break;

        case CTSU_CMD_SET_RESULT_OUTPUT_BUFFER:
            p_ctrl_blk->p_sensor_data = value;
#if (CTSU_CFG_DTC_TRANSFER_ENABLE == true) && (CTSU_CFG_DTC_CHAIN_TRANSFERS == false)
            _setup_dtc_tcb0(p_ctrl_blk);
#endif
            break;

        case CTSU_CMD_SET_CALLBACK:
            p_ctrl_blk->p_callback = (fit_callback_t)value;
            break;

        default:
            break;
    }
}
/***********************************************************************************************************************
End of function ctsu_handle_register_value_set
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: ctsu_handle_register_16bit_get
* Description  : Get 16-bit value stored for parameter in control block at given offset.
* Arguments    :
* @param p_ctrl_blk - Control block index to read.
* @param cmd - Control command issued.
* @param offset - Offset of sensor.
* @param value - Pointer to location where value must be stored.
* Return Value : None
***********************************************************************************************************************/
static void ctsu_handle_register_16bit_get(ctsu_ctrl_blk_t* p_ctrl_blk, ctsu_cmd_t cmd, uint16_t offset, void* value)
{
    if(CTSU_CMD_GET_CTSUSSC == cmd)
    {
        *(uint16_t*) value = p_ctrl_blk->p_sensor_settings[offset].ctsussc;
    }
    else if(CTSU_CMD_GET_CTSUSO0 == cmd)
    {
        *(uint16_t*) value = p_ctrl_blk->p_sensor_settings[offset].ctsuso0;
    }
    else if(CTSU_CMD_GET_CTSUSO1 == cmd)
    {
        *(uint16_t*) value =p_ctrl_blk->p_sensor_settings[offset].ctsuso1;
    }
    else
    {
        ;
    }
}
/***********************************************************************************************************************
End of function ctsu_handle_register_16bit_get
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: ctsu_handle_register_16bit_set
* Description  : Set 16-bit parameter value in control block.
* Arguments    :
* @param p_ctrl_blk - Control block index to read.
* @param cmd - Control command issued.
* @param offset - Offset of sensor.
* @param value - Location where result must be copied.
* Return Value : None
***********************************************************************************************************************/
static void ctsu_handle_register_16bit_set(ctsu_ctrl_blk_t* p_ctrl_blk, ctsu_cmd_t cmd, uint16_t offset, uint16_t value)
{
    if(CTSU_CMD_SET_CTSUSSC == cmd)
    {
        p_ctrl_blk->p_sensor_settings[offset].ctsussc = value;
    }
    else if(CTSU_CMD_SET_CTSUSO0 == cmd)
    {
        p_ctrl_blk->p_sensor_settings[offset].ctsuso0 = value;
    }
    else if(CTSU_CMD_SET_CTSUSO1 == cmd)
    {
        p_ctrl_blk->p_sensor_settings[offset].ctsuso1 = value;
    }
    else
    {
        ;
    }
}
/***********************************************************************************************************************
End of function ctsu_handle_register_16bit_set
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: calculate_sensor_count
* Description  : Calculate the sensor count for CTSU setting
* Arguments    :
* @param mode - Mode of CTSU operation
* @param p_ctsu_settings - Pointer to CTSU settings
* Return Value :
* @return Sensor count
***********************************************************************************************************************/
static uint16_t calculate_sensor_count (ctsu_mode_t mode, ctsu_const_sfrs_t * p_ctsu_settings)
{
    uint8_t sensor_itr;
    uint8_t num_tx = 0;
    uint8_t num_rx = 0;
    uint64_t ch_en_mask = 0;
    uint64_t ch_tx_mask = 0;

    ch_en_mask = p_ctsu_settings->ctsuchac4.byte;
    ch_en_mask <<= 8;
    ch_en_mask |= p_ctsu_settings->ctsuchac3.byte;
    ch_en_mask <<= 8;
    ch_en_mask |= p_ctsu_settings->ctsuchac2.byte;
    ch_en_mask <<= 8;
    ch_en_mask |= p_ctsu_settings->ctsuchac1.byte;
    ch_en_mask <<= 8;
    ch_en_mask |= p_ctsu_settings->ctsuchac0.byte;

    ch_tx_mask = p_ctsu_settings->ctsuchtrc4.byte;
    ch_tx_mask <<= 8;
    ch_tx_mask |= p_ctsu_settings->ctsuchtrc3.byte;
    ch_tx_mask <<= 8;
    ch_tx_mask |= p_ctsu_settings->ctsuchtrc2.byte;
    ch_tx_mask <<= 8;
    ch_tx_mask |= p_ctsu_settings->ctsuchtrc1.byte;
    ch_tx_mask <<= 8;
    ch_tx_mask |= p_ctsu_settings->ctsuchtrc0.byte;

    for(sensor_itr = 0; sensor_itr < LAST_TS; sensor_itr++)
    {
        /* Iterate through to the last sensor */
        if((ch_en_mask & ((uint64_t)1<<sensor_itr)) && (ch_tx_mask & ((uint64_t)1<<sensor_itr)))
        {
            /* CTSU will be operated in mutual mode. */
            num_tx++;
        }
        else if(ch_en_mask & ((uint64_t)1<<sensor_itr))
        {
            /* CTSU will be operated in self mode. */
            num_rx++;
        }
        else
        {
            /* Nothing to do */
            ;
        }
    }

    return (CTSU_MODE_MUTUAL_CAPACITANCE == mode) ? (num_rx * num_tx) : num_rx;
}
/***********************************************************************************************************************
End of function calculate_sensor_count
***********************************************************************************************************************/

#if (CTSU_CFG_ENABLE_CORRECTION > 0)
/***********************************************************************************************************************
* Function Name: ctsu_get_sdpa_setting
* Description  : Get the SDPA setting for achieving specified target frequency.
* Arguments    :   @param target_freq - Target Drive Frequency
*                  @param pclkb - PCLKB Frequency from CGC
*                  @param ctsuclk - CTSU divisor value in CTSU SFRs.
*                  @return
* Return Value : CTSUSDPA SFR value.
***********************************************************************************************************************/
static uint8_t ctsu_get_sdpa_setting(uint32_t target_freq, uint32_t pclkb, uint8_t ctsuclk)
{
    uint32_t pclkb_actual = pclkb/(1<<ctsuclk);
    uint8_t sdpa_setting = ((pclkb_actual / target_freq)>>1) - 1;

    return sdpa_setting;
}
/***********************************************************************************************************************
End of function ctsu_get_sdpa_setting
***********************************************************************************************************************/

#if (CTSU_CFG_ENABLE_CORRECTION==1)
/***********************************************************************************************************************
* Function Name: ctsu_correction_factor_first_coefficient
* Description  : Calculate first coefficient for the correction factor.
* Arguments    : sensor_ico_avg - Sensor ICO average value.
* Return Value : first coefficient calculated as:
*                                             (sensor_ico_avg * 1024)       sensor_ico_avg +20
*       correction_1st_coefficient  =  -------------------------------  =  --------------------------
*                                                        40960                            40
***********************************************************************************************************************/
uint16_t ctsu_correction_factor_first_coefficient(uint16_t sensor_ico_avg)
{
    uint16_t correction_1st_coefficient = (sensor_ico_avg + 20 ) / 40;
    return correction_1st_coefficient;
}
/***********************************************************************************************************************
End of function ctsu_correction_factor_first_coefficient
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: ctsu_correction_factor_second_std_value
* Description  : Calculate Standard deviation value
* Arguments    : ctsutrmr - Register value from CTSU Trimming register (set by factory).
* Return Value : Standard deviation value calculated as:
*                                  ctsutrmr + 273                             (ctsutrmr + 273) * 2560 *128
* g_correction_2nd_std_val  =  ----------------------- * 40960 *128  =  -------------------------------------
*                                        528                                               33
***********************************************************************************************************************/
uint16_t ctsu_correction_factor_second_std_value(uint16_t ctsutrmr)
{
    uint32_t temp = ((ctsutrmr + 273) * 9930) + 64;
    return (temp >> 7);
}
/***********************************************************************************************************************
End of function ctsu_correction_factor_second_std_value
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: ctsu_correction_factor_second_coefficient
* Description  : Calculates second coefficient for CTSU correction factor based on standard deviation value, and
*                   second sensor ico average reading.
* Arguments    : sensor_ico_avg - second sensor ico average reading
*                second_std_val - Standard deviation value
* Return Value : second coefficient calculated as:
*                                      g_correction_2nd_val
*  g_correction_2nd_coefficient  =  -------------------------- * 1024
*                                    g_correction_2nd_std_val
***********************************************************************************************************************/
uint16_t ctsu_correction_factor_second_coefficient(uint16_t sensor_ico_avg, uint16_t second_std_val)
{
    uint16_t retval = ((sensor_ico_avg << 10) + (second_std_val/2)) / second_std_val;
    return retval;
}
/***********************************************************************************************************************
End of function ctsu_correction_factor_second_coefficient
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: ctsu_correction_16pt_table_create
* Description  : Creates an interpolated 16-point table of sensor ICO values and coefficients.
* Arguments    : ideal_sensor_ico_values_16pt - ideal sensor ICO curve
*                coeff_1 - 1st correction coefficient
*                coeff_2 - 2nd correction coefficient
*                std_2 - Standard deviation value
*                table16pt - Sensor ICO values expected.
*                coeff16pt - Correction coefficients to be applied.
* Return Value : None, (Return from argument := table16pt and coeff16pt)
***********************************************************************************************************************/
void ctsu_correction_16pt_table_create(uint16_t const * const ideal_sensor_ico_values_16pt,
                                        uint16_t coeff_1,
                                        uint16_t coeff_2,
                                        uint16_t std_2,
                                        uint16_t * const table16pt,
                                        uint16_t * const coeff16pt)
{
    uint16_t flag;
    uint32_t diff;
    uint16_t correction_pt[16];

    if(coeff_2 > coeff_1)
    {
        flag = 0;
        diff = coeff_2 - coeff_1;
    }
    else
    {
        flag = 1;
        diff = coeff_1 - coeff_2;
    }

    /*         diff_coefficient * 40960                                                                               */
    /*     ----------------------------------                                                                         */
    /*      40960 - g_correction_2nd_std_val                                                                          */

    diff = diff * 40960;
    diff = diff/(40960 - std_2);

    /*                                                                 fill_diff * N                                  */
    /*     g_correction_16point  =  g_correction_1st_coefficient +(-) ---------------                                 */
    /*                                                                      16                                        */

    for(uint8_t itr = 1; itr < 16; itr++)
    {
        correction_pt[itr] = (0 == flag) ?
                                (coeff_1 + ((diff * itr) >> 4))
                              : (coeff_1 - ((diff * itr) >> 4));
    }

    correction_pt[0] = coeff_1;

    for(uint8_t itr = 0; itr < 16; itr++)
    {
        uint32_t temp = (correction_pt[itr] * ideal_sensor_ico_values_16pt[itr]);
        table16pt[itr] = (uint16_t)((temp + 512) >> 10);
        coeff16pt[itr] = 1000000/correction_pt[15-itr];
    }

}


/***********************************************************************************************************************
* Function Name: ctsu_corrected_value
* Description  : Returns the corrected value for the provided CTSU measurement.
* Arguments    : index -
*                    Where to start looking
*                p_output -
*                    Pointer of where to put the output data
* Return Value : count -
*                    How many entries were found
***********************************************************************************************************************/
uint16_t ctsu_corrected_value(uint16_t input,
                                int8_t magnification,
                                uint16_t const * const table61pt,
                                uint16_t const * const coeff61pt)
{
    uint8_t loop;
    uint8_t loop_coef;
    uint32_t cmp_val = 0;
    uint32_t correct_box;
    uint32_t correct_box2;
    uint16_t correction_sensor_cnt;

    if(0 > magnification)
    {
        cmp_val = ((-1)* magnification) * input ;
        cmp_val /= 10;
    }
    else
    {
        cmp_val = (input * 10) / magnification;
    }
    correct_box = input;

    if(table61pt[60]>= cmp_val)
    {
        correct_box2 = correct_box*coeff61pt[0];
        correction_sensor_cnt = (uint16_t)(correct_box2>>10);
    }
    else
    {
        if (table61pt[29] <= cmp_val)
        {
            if (table61pt[14] <= cmp_val)
            {
                for (loop = 0, loop_coef = 60; loop < 15; loop++)
                {
                    if (table61pt[loop] <= cmp_val)
                    {
                        correct_box2          = correct_box * coeff61pt[loop_coef];
                        correction_sensor_cnt = (uint16_t)(correct_box2 >> 10);
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
            else
            {
                for (loop = 15, loop_coef = 45; loop < 30; loop++)
                {
                    if (table61pt[loop] <= cmp_val)
                    {
                        correct_box2          = correct_box * coeff61pt[loop_coef];
                        correction_sensor_cnt = (uint16_t)(correct_box2 >> 10);
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
        }
        else
        {
            if (table61pt[44] <= cmp_val)
            {
                for (loop = 30, loop_coef = 30; loop < 45; loop++)
                {
                    if (table61pt[loop] <= cmp_val)
                    {
                        correct_box2          = correct_box * coeff61pt[loop_coef];
                        correction_sensor_cnt = (uint16_t)(correct_box2 >> 10);
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
            else
            {
                for (loop = 45, loop_coef = 15; loop < 60; loop++)
                {
                    if (table61pt[loop] <= cmp_val)
                    {
                        correct_box2          = correct_box * coeff61pt[loop_coef];
                        correction_sensor_cnt = (uint16_t)(correct_box2 >> 10);
                        break;
                    }
                    loop_coef = loop_coef - 1;
                }
            }
        }
    }

    return correction_sensor_cnt;
}
/***********************************************************************************************************************
End of function ctsu_corrected_value
***********************************************************************************************************************/
#endif

/***********************************************************************************************************************
* Function Name: ctsu_correction_get_avg_sensor_ico_value
* Description  : Does an example task. Making this longer just to see how it wraps. Making this long just to see how it
*                wraps.
* Arguments    : tune_hdl - CTSU Control Block to be used for Scanning.
*                p_result - Location where the result must be output.
* Return Value : See R_CTSU_Scan error return values.
***********************************************************************************************************************/
static ctsu_err_t ctsu_correction_get_avg_sensor_ico_value(uint32_t tune_hdl, uint16_t * const p_result)
{
    uint16_t scan_count = 0;
    uint32_t sensor_ico = 0;

    ctsu_err_t ctsu_err = CTSU_SUCCESS;

    ctsu_ctrl_blk_t *const p_ctrl_blk = &pvt_ctsu_ctrl_blk[tune_hdl];
    uint16_t const * const ctsu_result = p_ctrl_blk->p_sensor_data;

    while(scan_count < CTSU_CORRECTION_SCAN_COUNT)
    {
        ctsu_err = R_CTSU_Scan(tune_hdl);

        if (CTSU_SUCCESS != ctsu_err)
        {
            return ctsu_err;
        }

        /* Wait until scan is complete. Grab lock for manipulating buffers. */
        while(false == R_BSP_SoftwareLock(&p_ctrl_blk->lock))
        {
            /* GSCE CODAN Error Elimination: Do Nothing. */;
        }

        if (0 == (p_ctrl_blk->scan_errors & CTSU_ERR_CTSU_ICOMP))
        {
            sensor_ico += ctsu_result[0];
            scan_count += 1;
        }

        R_BSP_SoftwareUnlock(&p_ctrl_blk->lock);
    }

    sensor_ico /= scan_count;

    p_result[0] = (uint16_t)(sensor_ico & UINT16_MAX);

    return CTSU_SUCCESS;
}
/***********************************************************************************************************************
End of function ctsu_correction_get_avg_sensor_ico_value
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: ctsu_correction_create_bins
* Description  : Create interpolated data over 61 points.
* Arguments    : table16pt - Table of 16 sensor ICO values
*                coeff16pt - Table of 16 correction coefficients
*                table61pt - Table of 61 sensor ICO values
*                coeff61pt - Table of 61 correction coefficients
* Return Value : table61pt, and coeff61pt.
***********************************************************************************************************************/
static void ctsu_correction_create_bins(uint16_t const * const table16pt,
                                        uint16_t const * const coeff16pt,
                                        uint16_t * const table61pt,
                                        uint16_t * const coeff61pt)
{
    for (uint16_t itr = 0; itr < 15; itr++)
    {
        uint16_t delta;
        delta = ((table16pt[itr] - table16pt[itr+1]) + 2) / 4;

        uint16_t offset = (itr*4);
        table61pt[offset + 0] = table16pt[itr];
        table61pt[offset + 1] = table16pt[itr] - delta;
        table61pt[offset + 2] = table16pt[itr] - (delta*2);
        table61pt[offset + 3] = table16pt[itr] - (delta*3);

        if(coeff16pt[1] > coeff16pt[0])
        {
            delta = ((coeff16pt[itr+1] - coeff16pt[itr]) +2) / 4;
            coeff61pt[offset + 0] = coeff16pt[itr];
            coeff61pt[offset + 1] = coeff16pt[itr] + delta;
            coeff61pt[offset + 2] = coeff16pt[itr] + (delta * 2);
            coeff61pt[offset + 3] = coeff16pt[itr] + (delta * 3);
        }
        else
        {
            delta = ((coeff16pt[itr] - coeff16pt[itr+1]) +2) / 4;
            coeff61pt[offset + 0] = coeff16pt[itr];
            coeff61pt[offset + 1] = coeff16pt[itr] - delta;
            coeff61pt[offset + 2] = coeff16pt[itr] - (delta * 2);
            coeff61pt[offset + 3] = coeff16pt[itr] - (delta * 3);
        }
    }

    table61pt[60] = table16pt[15];
    coeff61pt[60] = coeff16pt[15];
}

/***********************************************************************************************************************
* Function Name: ctsu_correction_set_magnification
* Description  : Does an example task. Making this longer just to see how it wraps. Making this long just to see how it
*                wraps.
* Arguments    : index -
*                    Where to start looking
*                p_output -
*                    Pointer of where to put the output data
* Return Value : count -
*                    How many entries were found
***********************************************************************************************************************/
void ctsu_correction_set_magnification(int8_t * const p_buffer,
                                                ctsu_sensor_setting_t const * const p_setting,
                                                uint16_t const sensor_count,
                                                uint32_t const pclkb_hz)
{
    for(uint16_t itr = 0; itr < sensor_count; itr++)
    {
        uint16_t   measurement_period;
        int8_t snum = (p_setting[itr].ctsuso0 & 0xFC00)>>10;
        int8_t sdpa = (p_setting[itr].ctsuso1 & 0x1F00)>>8;
        uint8_t mea_freq = ((pclkb_hz/1000000) * 10) /(2*(sdpa+1));

        switch (mea_freq)
        {
            case 40:                                                            /* 4.0MHz    32MHz / 8     24MHz / 6  */
                measurement_period =  625 * (snum + 1);
                break;
            case 33:                                                            /* 3.3MHz    27MHz / 8  */
                measurement_period =  750 * (snum + 1);
                break;
            case 20:                                                            /* 2.0MHz    32MHz / 16    24MHz / 12  */
                measurement_period = 1250 * (snum + 1);
                break;
            case 16:                                                            /* 1.6MHz    27MHz / 16 */
                measurement_period = 1500 * (snum + 1);
                break;
            case 10:                                                            /* 1.0MHz    32MHz / 32    24MHz / 24  */
                measurement_period = 2500 * (snum + 1);
                break;
            case 8:                                                             /* 0.8MHz    27MHz / 32 */
                measurement_period = 3000 * (snum + 1);
                break;
            case 5:                                                             /* 0.5MHz    32MHz / 64    24MHz / 48  */
                measurement_period = 5000 * (snum + 1);
                break;
            case 4:                                                             /* 0.4MHz    27MHz / 64 */
                measurement_period = 6000 * (snum + 1);
                break;
            default:
                break;
        }

        p_buffer[itr] = (5000 <= measurement_period) ? (int8_t)(measurement_period/500):(int8_t)((-1)*(int8_t)(50000 / measurement_period));
    }
}
#endif  /* (CTSU_CFG_ENABLE_CORRECTION > 0) */

#if (CTSU_CFG_CALIBRATION_ENABLE > 0)
/***********************************************************************************************************************
* Function Name: ctsu_correction_factor
* Description  : Calculate Correction Factor for Control Block.
* Arguments    :
* @param usr_hdl Index to Control Block in array.
*
* Return Value :
* @return @ref CTSU_SUCCESS, CTSU_ERR_CORRECTION_FACTOR
* @note When CTSU_CFG_CALIBRATION_ENABLE is set to 1, algorithm specified in R01AN3610EJ0100 is implemented.
***********************************************************************************************************************/
static ctsu_err_t ctsu_correction_factor(uint32_t usr_hdl)
{
    ctsu_err_t ctsu_err = CTSU_SUCCESS;
#if (CTSU_CFG_PARAM_CHECKING_ENABLE > 0)
    ASSERT( CTSU_CFG_MAX_CONTROL_BLOCK_COUNT > usr_hdl);
    ASSERT( OPEN == pvt_ctsu_ctrl_blk[usr_hdl].open);
    ASSERT( 64 >= pvt_ctsu_ctrl_blk[usr_hdl].num_sensors);
    ASSERT( CTSU_CFG_MAX_CONTROL_BLOCK_COUNT > 1);
#endif

    ctsu_ctrl_blk_t * const p_ctrl_blk = &pvt_ctsu_ctrl_blk[usr_hdl];

    /* Set the magnification factor */
    ctsu_correction_set_magnification(p_ctrl_blk->p_magnification_sensor,
            p_ctrl_blk->p_sensor_settings,
            p_ctrl_blk->num_sensors,
            p_ctrl_blk->pclkb_hz);

#if (CTSU_CFG_ENABLE_CORRECTION > 0)
    /**
     * CTSU Sensor ICO Correction using CTSUTRMR register.
     * Refer: R01AN3610EJ0100
     */


    uint8_t ctsutrmr = HW_CTSU_CTSUTRMRGet();

    typedef union
    {
       uint8_t byte[5];
       uint64_t dlong;
    }ctsu_chxx_t;

    ctsu_chxx_t chac;
    chac.dlong = 0;

    if (CTSU_MODE_SELF_CAPACITANCE == p_ctrl_blk->mode)
    {
        chac.byte[0] = p_ctrl_blk->p_ctsu_settings->ctsuchac0.byte;
        chac.byte[1] = p_ctrl_blk->p_ctsu_settings->ctsuchac1.byte;
        chac.byte[2] = p_ctrl_blk->p_ctsu_settings->ctsuchac2.byte;
        chac.byte[3] = p_ctrl_blk->p_ctsu_settings->ctsuchac3.byte;
        chac.byte[4] = p_ctrl_blk->p_ctsu_settings->ctsuchac4.byte;
    }
    else if(CTSU_MODE_MUTUAL_CAPACITANCE == p_ctrl_blk->mode)
    {
        chac.byte[0] = p_ctrl_blk->p_ctsu_settings->ctsuchtrc0.byte;
        chac.byte[1] = p_ctrl_blk->p_ctsu_settings->ctsuchtrc1.byte;
        chac.byte[2] = p_ctrl_blk->p_ctsu_settings->ctsuchtrc2.byte;
        chac.byte[3] = p_ctrl_blk->p_ctsu_settings->ctsuchtrc3.byte;
        chac.byte[4] = p_ctrl_blk->p_ctsu_settings->ctsuchtrc4.byte;
    }
    else
    {
        ;
    }

    /* Enable only one sensor */
    for(uint64_t itr = 1; itr > 0 ;itr <<= 1)
    {
        if((chac.dlong & itr) > 0)
        {
            chac.dlong &= itr;
            break;
        }
    }

    ASSERT(chac.dlong > 0);

    const uint8_t ctsuclk = (p_ctrl_blk->p_ctsu_settings->ctsucr1.byte & 0x30);

    ctsu_const_sfrs_t ctsu_const_sfrs_correction =
    {
        .ctsucr0.byte   = 0,
        .ctsucr1.byte   = (0x43) | ctsuclk,
        .ctsusdprs.byte = 0x63,
        .ctsusst.byte   = 0x10,
        .ctsuchac0.byte = chac.byte[0],
        .ctsuchac1.byte = chac.byte[1],
        .ctsuchac2.byte = chac.byte[2],
        .ctsuchac3.byte = chac.byte[3],
        .ctsuchac4.byte = chac.byte[4],
        .ctsuchtrc0.byte = 0,
        .ctsuchtrc1.byte = 0,
        .ctsuchtrc2.byte = 0,
        .ctsuchtrc3.byte = 0,
        .ctsuchtrc4.byte = 0,
        .ctsudclkc.byte = 0x30,
    };

    uint16_t ctsu_result[4];

    ctsu_sensor_setting_t sensor_drive_setting_correction[1] =
    {
        [0] = {.ctsussc = 0x0700, .ctsuso0 = 0x0000, .ctsuso1 = 0x200F},
    };

    const uint16_t ctsusdpa = ctsu_get_sdpa_setting(500000, p_ctrl_blk->pclkb_hz, ctsuclk);

    sensor_drive_setting_correction[0].ctsuso1 |=  (ctsusdpa << 8);
    memset(ctsu_result, 0, sizeof(ctsu_result));

    const ctsu_cfg_t ctsu_correction_cfg =
    {
         .p_ctsu_settings = &ctsu_const_sfrs_correction,
         .p_sensor_settings = &sensor_drive_setting_correction[0],
         .p_sensor_data = &ctsu_result[0],
         .pclkb_hz = p_ctrl_blk->pclkb_hz,
         .p_callback = NULL,
    };

    const uint32_t tune_hdl = UINT32_MAX;

    ctsu_err = R_CTSU_Open((uint32_t*)&tune_hdl, &ctsu_correction_cfg);

    /* Following code enables CTSUERRS.CTSUSPMD=2, CTSUTSOC=1*/
    {
        uint16_t ctsuerrs = HW_CTSU_CTSUERRSGet();
        const uint16_t mask = CTSUERRS_ENABLE_CALIBRATION_MODE;
        ctsuerrs |= mask;
        HW_CTSU_CTSUERRSSet(ctsuerrs);
    }

    uint16_t avg_sensor_ico[2] = {0,0};

    /* Get average Primary CTSUSC value */
    ctsu_err = ctsu_correction_get_avg_sensor_ico_value(tune_hdl, &avg_sensor_ico[0]);

    /* Change CTSUTRMR Value to 0xFF*/
    HW_CTSU_CTSUTRMRSet(0xFF);

    /* Get average Primary CTSUSC value */
    ctsu_err = ctsu_correction_get_avg_sensor_ico_value(tune_hdl, &avg_sensor_ico[1]);

    /* Restore CTSUTRMR Value */
    HW_CTSU_CTSUTRMRSet(ctsutrmr);

    /* Following code sets CTSUERRS.CTSUSPMD=0, CTSUTSOC=0*/
    {
        uint16_t ctsuerrs = HW_CTSU_CTSUERRSGet();
        const uint16_t mask = ~CTSUERRS_ENABLE_CALIBRATION_MODE;
        ctsuerrs &= mask;
        HW_CTSU_CTSUERRSSet(ctsuerrs);
    }

    /* Create 1st Coefficient */
    uint16_t correction_coefficient[2];
    correction_coefficient[0] = ctsu_correction_factor_first_coefficient(avg_sensor_ico[0]);

    /* Create 2nd standard value */
    uint16_t correction_std_val_2nd = ctsu_correction_factor_second_std_value(ctsutrmr);

    /* Create 2nd Coefficient value */
    correction_coefficient[1] = ctsu_correction_factor_second_coefficient(avg_sensor_ico[1], correction_std_val_2nd);

    uint16_t table16pt[16];
    uint16_t coeff16pt[16];

    memset(&table16pt, 0, sizeof(table16pt));
    memset(&coeff16pt, 0, sizeof(coeff16pt));

    /* Create 16 point table */
    ctsu_correction_16pt_table_create(p_ctrl_blk->p_correction_ideal_sensor_ico,
                                        correction_coefficient[0],
                                        correction_coefficient[1],
                                        correction_std_val_2nd,
                                        &table16pt[0],
                                        &coeff16pt[0]);

    /* Create Interpolated table and coefficients */
    ctsu_correction_create_bins(&table16pt[0],
                                &coeff16pt[0],
                                p_ctrl_blk->p_linear_interpolation_table,
                                p_ctrl_blk->p_linear_interpolation_coeff);

    /* Close the configuration */
    ctsu_err = R_CTSU_Close(tune_hdl);
#endif /* (CTSU_CFG_ENABLE_MEASUREMENT_CORRECTION > 0) */

    return ctsu_err;
}
#endif /* (CTSU_CFG_CALIBRATION_ENABLE > 0) */
/***********************************************************************************************************************
End of function ctsu_correction_factor
***********************************************************************************************************************/

#if (CTSU_CFG_DTC_TRANSFER_ENABLE == true) && (CTSU_CFG_DTC_CHAIN_TRANSFERS == false)
/**
 * Setup DTC TCB0 for control block.
 * @param p_hdl Pointer to control block to setup.
 */
static void _setup_dtc_tcb0(ctsu_ctrl_blk_t * p_hdl)
{
    p_hdl->ctsu_wr[0].SECOND_LWORD.SAR = (uint32_t)p_hdl->p_sensor_settings;
    p_hdl->ctsu_wr[0].FOURTH_LWORD.REG.CRB.WORD = 0xFFFF;
    p_hdl->ctsu_rd[0].THIRD_LWORD.DAR = (uint32_t)p_hdl->p_sensor_data;
    p_hdl->ctsu_rd[0].FOURTH_LWORD.REG.CRB.WORD = 0xFFFF;
}
#endif /*(CTSU_CFG_DTC_TRANSFER_ENABLE == true) && (CTSU_CFG_DTC_CHAIN_TRANSFERS == false) */

/**********************************************************************************************************************/
/*                                                 Interrupt service routines                                               */
/**********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: excep_ctsu_ctsuwr
* Description  : Interrupt service routine definition for the CTSUWR interrupt.
* Arguments    : None
* Return Value : count -
*                    How many entries were found
***********************************************************************************************************************/
static void excep_ctsu_ctsuwr(void)
{
#if (CTSU_CFG_DTC_TRANSFER_ENABLE == false)
    if(pvt_current_scan_handle_index < CTSU_CFG_MAX_CONTROL_BLOCK_COUNT)
    {
        /** a. Grab the a handle being used by the CTSU. */
        ctsu_ctrl_blk_t* p_ctrl_blk = &pvt_ctsu_ctrl_blk[pvt_current_scan_handle_index];

        /** b. Write CTSU settings from RAM to SFR */
        HW_CTSU_CTSUSSCSet(p_ctrl_blk->p_sensor_settings[sensor_wr_irq_itr].ctsussc);
        HW_CTSU_CTSUSO0Set(p_ctrl_blk->p_sensor_settings[sensor_wr_irq_itr].ctsuso0);
        HW_CTSU_CTSUSO1Set(p_ctrl_blk->p_sensor_settings[sensor_wr_irq_itr].ctsuso1);

        /** c. Increment write IRQ iterator */
        sensor_wr_irq_itr++;
    }
#endif
}
/***********************************************************************************************************************
End of function excep_ctsu_ctsuwr
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: excep_ctsu_ctsurd
* Description  : Interrupt service routine definition for CTSURD interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void excep_ctsu_ctsurd(void)
{
#if (CTSU_CFG_DTC_TRANSFER_ENABLE == false)
    if(pvt_current_scan_handle_index < CTSU_CFG_MAX_CONTROL_BLOCK_COUNT)
    {
        /** a. Grab the a handle being used by the CTSU. */
        ctsu_ctrl_blk_t* p_ctrl_blk = &pvt_ctsu_ctrl_blk[pvt_current_scan_handle_index];

        /** b. Get location in RAM where results should be stored*/
        uint16_t* p_result = (uint16_t*)p_ctrl_blk->p_sensor_data;

        /** c. Copy CTSU scan results from SFR to RAM */
        p_result[sensor_rd_irq_itr + 0] = HW_CTSU_CTSUSCGet();
        p_result[sensor_rd_irq_itr + 1] = HW_CTSU_CTSURCGet();

        /** d. Increment read IRQ iterator */
        sensor_rd_irq_itr+=2;
    }
#endif
}
/***********************************************************************************************************************
End of function excep_ctsu_ctsurd
***********************************************************************************************************************/


/**
 *
 */
/***********************************************************************************************************************
* Function Name: excep_ctsu_ctsufn
* Description  : Interrupt service routine definition for CTSUFN interrupt.
* Arguments    : None.
* Return Value : None
***********************************************************************************************************************/
static void excep_ctsu_ctsufn(void)
{
    ctsu_callback_arg_t args;
    args.event_mask = CTSU_EVENT_SCAN_COMPLETED;
    args.handle_num = UINT32_MAX;
    args.info = 0;

    /** a. Read the CTSU SFRs which indicate errors encountered from the latest scan.
     * Record and Clear if any errors occurred during scan. */
    if(HW_CTSU_CTSUSTGetBitCTSUROVF() == 1)
    {
        args.info |= CTSU_ERR_CTSU_RC_OVF;
        HW_CTSU_CTSUCR0SetBitCTSUINIT(1);
    }

    if(HW_CTSU_CTSUSTGetBitCTSUSOVF() == 1)
    {
        args.info |= CTSU_ERR_CTSU_SC_OVF;
        HW_CTSU_CTSUCR0SetBitCTSUINIT(1);
    }

    if(HW_CTSU_CTSUERRSGetBitCTSUICOMP() == 1)
    {
        args.info |= CTSU_ERR_CTSU_ICOMP;
        uint8_t ctsucr1 = HW_CTSU_CTSUCR1Get();

        /* Hardware note on CTSU LPF Capacitance Charging Control/TSCAP Voltage Error Monitor */
        HW_CTSU_CTSUCR1Set(ctsucr1 & (~0x03));
        HW_CTSU_CTSUCR1Set(ctsucr1);
    }

#if (CTSU_CFG_WAIT_STATE_POWER_SAVING_ENABLE==true)
    /* Refer CTSUSNZ Bit (CTSU Wait State Power-Saving Enable) */
    HW_CTSU_CTSUCR0SetBitCTSUSNZ(1);
#endif

    if(pvt_current_scan_handle_index < CTSU_CFG_MAX_CONTROL_BLOCK_COUNT)
    {
        /** b. Grab the a handle being used by the CTSU. */
        ctsu_ctrl_blk_t* p_ctrl_blk = &pvt_ctsu_ctrl_blk[pvt_current_scan_handle_index];
#if (CTSU_CFG_ENABLE_CORRECTION > 0)
        if ((NULL != p_ctrl_blk->p_correction_ideal_sensor_ico)
                && (p_ctrl_blk->p_linear_interpolation_table[61] > 0))
        {
            /* Ready to perform corrections */
            uint16_t multiplier = (CTSU_MODE_SELF_CAPACITANCE == p_ctrl_blk->mode) ? 2 : 4;
            uint16_t * const input = p_ctrl_blk->p_sensor_data;
            for(uint16_t itr = 0; itr < p_ctrl_blk->num_sensors; itr+=1)
            {
                /* Primary Sensor Counter */
                int8_t magnifier = p_ctrl_blk->p_magnification_sensor[itr];
                input[(itr*multiplier) + 0] = ctsu_corrected_value(input[(itr*multiplier) + 0],
                                                    magnifier,
                                                    p_ctrl_blk->p_linear_interpolation_table,
                                                    p_ctrl_blk->p_linear_interpolation_coeff);
                input[(itr*multiplier) + 1] = (CTSU_MODE_SELF_CAPACITANCE == p_ctrl_blk->mode) ?
                        (p_ctrl_blk->p_correction_ideal_sensor_ico[_07_2UA]):
                        (p_ctrl_blk->p_correction_ideal_sensor_ico[_04_8UA]);

                if (CTSU_MODE_MUTUAL_CAPACITANCE == p_ctrl_blk->mode)
                {
                    /* Secondary Sensor Counter */
                    input[(itr*multiplier) + 2] = ctsu_corrected_value(input[(itr*multiplier) + 2],
                                                                        magnifier,
                                                                        p_ctrl_blk->p_linear_interpolation_table,
                                                                        p_ctrl_blk->p_linear_interpolation_coeff);
                    input[(itr*multiplier) + 3] = p_ctrl_blk->p_correction_ideal_sensor_ico[_04_8UA];
                }
            }

        }
#endif
        /** d. Unlock the handle so it can be used by other CTSU functions.*/
        R_BSP_SoftwareUnlock(&p_ctrl_blk->lock);
        
        p_ctrl_blk->scan_errors = args.info;
        if (NULL != p_ctrl_blk->p_callback)
        {
            /** c. Perform callback to the upper layer. */
            args.handle_num = pvt_current_scan_handle_index;
            p_ctrl_blk->p_callback(&args);
        }

    }
#if (CTSU_CFG_DTC_TRANSFER_ENABLE == false) && (CTSU_CFG_DTC_CHAIN_TRANSFERS == false)
    /** e. Reset the ISR iterators */
    sensor_wr_irq_itr = 0;
    sensor_rd_irq_itr = 0;
#endif
    /** f. Unlock the CTSU hardware (locked when @ref R_CTSU_Scan was invoked). */
    R_BSP_HardwareUnlock(BSP_LOCK_CTSU);
}
/***********************************************************************************************************************
End of function excep_ctsu_ctsufn
***********************************************************************************************************************/
