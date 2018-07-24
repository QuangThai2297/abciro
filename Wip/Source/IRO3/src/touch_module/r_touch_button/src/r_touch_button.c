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
 * Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @file
 ***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @addtogroup Touch_Button
 * @brief Driver for operating TS Pins as buttons. Provides the ability to generate events per button.
 *
 * It implements the following interface:
 *  - @ref Touch_Button interface defined in r_touch_button_if.h
 * @{
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 Includes
 ***********************************************************************************************************************/
#if !defined(UNITY_TESTING) \
        && defined(BSP_CFG_IO_LIB_ENABLE) \
        && (BSP_CFG_IO_LIB_ENABLE == 1)
/** @note Use assertion on the Renesas Virtual Console.
 * Instructions: (Alt+Shift+Q, C) >
 *      (Button)Display Selected Console >
 *          Renesas Virtual Console >
 *              (Button) Pin Console.
 */
#include <assert.h>
#endif
#include <string.h>
#include "r_touch_button_config.h"
#include "r_touch_button_if.h"
/***********************************************************************************************************************
 Macro definitions
 ***********************************************************************************************************************/
#if defined(assert)
#define ASSERT(test)    assert(test)
#else
#define ASSERT(test)    if((test)==false){return TOUCH_BUTTON_ERR_INVALID_PARAM;}
#endif

#define OPEN            (0x4F50454e)

#ifndef LAST_TS
#if (BSP_MCU_RX113==1)
#define LAST_TS     (11)
#elif (BSP_MCU_RX130==1)
#define LAST_TS     (35)
#elif (BSP_MCU_RX230==1)
#define LAST_TS     (35)
#elif (BSP_MCU_RX231==1)
#define LAST_TS     (35)
#endif
#endif  /* LAST_TS */

#define NUM_CTRL_BLOCKS (sizeof(pvt_button_ctrl_blk)/sizeof(pvt_button_ctrl_blk[0]))

/***********************************************************************************************************************
 Typedef definitions
 ***********************************************************************************************************************/
typedef struct st_touch_button_ctrl_t
{
    uint32_t open;

    /** Define the channel pairs which make up a button. */
    touch_sensor_t button;
    union
    {
        struct{
            /** enable release events */
            uint8_t  release    : 1; ///< Enable release events.
            uint8_t  press      : 1; ///< Enable press events.
            uint8_t  hold       : 1; ///< Enable hold events.
        };
        uint8_t byte; ///< Byte representation of events enabled.
    } enable;
    uint16_t const debounce;    ///< Number of consecutive times a button is determined as touched before state changes from touched to not touched.
    uint16_t const hold_max;    ///< Maximum duration of a valid touch.
    fit_callback_t p_callback;  ///< Function to call when an event occurs.

    /***************************************************************************/
    /* Private values. Configured and manipulated by R_Touch_Button code only. */
    /***************************************************************************/
    bsp_lock_t lock;
    uint32_t const touch_hdl;       ///< Represents the identifier of the lower level.
    touch_button_event_t  state;    ///< Represents the current state of the button.
    uint16_t debounce_counter;      ///< Variable value for debouncing purposes.
    uint16_t hold_counter;          ///< Variable value for noting the amount of time for which button is spending between press and release.
} touch_button_ctrl_t;

static touch_button_ctrl_t pvt_button_ctrl_blk[BUTTON_CFG_MAX_CONTROL_BLOCK_COUNT];

/***********************************************************************************************************************
 Private function prototypes
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Private global variables
 ***********************************************************************************************************************/
#if (MULTI_TOUCH_BUTTON_WAIT_FOR_RELEASE > 0)
uint16_t num_buttons_active = 0;
#endif

/***********************************************************************************************************************
 Global Variables
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Functions
 ***********************************************************************************************************************/
static void touch_button_callback(void * p_arg);
static uint8_t already_open(touch_button_ctrl_t const*const p_cfg);
static bool debounce(uint16_t * p_current, uint16_t const threshold);
static touch_button_err_t get_control_block_parameter(touch_button_ctrl_t * p_ctrl, touch_button_control_arg_t * const p_arg);
static touch_button_err_t set_control_block_parameter(touch_button_ctrl_t * p_ctrl, touch_button_control_arg_t * const p_arg);
static touch_button_callback_arg_t touch_button_update(touch_button_ctrl_t * const p_ctrl);

#pragma inline(R_TOUCH_ButtonGetVersion)

/***********************************************************************************************************************
* Function Name: R_TOUCH_ButtonGetVersion
* Description  : Get the version number for the driver.
* Arguments    : None
* Return Value : Version number of driver in 32-bits.
***********************************************************************************************************************/
uint32_t R_TOUCH_ButtonGetVersion(void)
{
    return ((((uint32_t)TOUCH_BUTTON_VERSION_MAJOR) << 16) | (uint32_t)TOUCH_BUTTON_VERSION_MINOR);
}
/***********************************************************************************************************************
End of function R_TOUCH_ButtonGetVersion
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: R_TOUCH_ButtonOpen
* Description  : Open a TOUCH Sensor Button with the configuration provided.
* Arguments    : handle -
*                    Pointer to location where user wants the index of the Button Control block to be returned.
*                p_cfg -
*                    Pointer to button configuration information.
* Return Value : TOUCH_BUTTON_SUCCESS, TOUCH_BUTTON_ERR_INVALID_PARAM, TOUCH_BUTTON_ERR_INSUFFICIENT_MEMORY
***********************************************************************************************************************/
touch_button_err_t R_TOUCH_ButtonOpen(uint32_t * const handle, touch_button_cfg_t const * const p_cfg)
{
    touch_button_err_t button_err;
    touch_err_t touch_err;
    uint8_t dummy_read_byte;
#if (BUTTON_CFG_PARAM_CHECKING_ENABLE == true)
    ASSERT(NULL != handle);
    ASSERT(LAST_TS >= p_cfg->button.rx);
    ASSERT(NULL != p_cfg->p_touch_cfg);
#endif

    touch_cfg_t touch_cfg =
    {
        /* Initializing constants to get rid of warnings */
        .p_ctsu_cfg = p_cfg->p_touch_cfg->p_ctsu_cfg,
        .p_common = p_cfg->p_touch_cfg->p_common,
        .p_sensor = p_cfg->p_touch_cfg->p_sensor,
        .p_ignored = p_cfg->p_touch_cfg->p_ignored,
    };

    touch_button_ctrl_t ctrl =
    {
        .open = OPEN,
        .lock = false,
        .button =
        {
            .rx = p_cfg->button.rx,
            .tx = p_cfg->button.tx,
        },
        .enable.byte = p_cfg->enable.byte,
        .state = TOUCH_BUTTON_EVENT_RELEASED,
        .debounce = p_cfg->debounce,
        .hold_max = p_cfg->hold_max,
        .p_callback = p_cfg->p_callback,

        .touch_hdl = UINT32_MAX,
        .debounce_counter = 0,
        .hold_counter = 0,
    };

    uint32_t * p_hdl = (uint32_t *)&ctrl.touch_hdl;

    memcpy(&touch_cfg, p_cfg->p_touch_cfg, sizeof(touch_cfg));

    /* Change the callback function to use the button update callback */
    touch_cfg.p_callback = touch_button_callback;

    /* Open lower layers */
    touch_err = TOUCH_SUCCESS;
    touch_err |= R_TOUCH_Open(p_hdl, &touch_cfg);

    touch_read_t args =
    {
        .read_cmd = TOUCH_DATA_BINARY,
        .sensor = &ctrl.button,
        .sensor_count = 1,
        .p_dest = &dummy_read_byte,
        .size = sizeof(dummy_read_byte),
    };

    touch_err |= R_TOUCH_Read(*p_hdl, &args);

#if (BUTTON_CFG_PARAM_CHECKING_ENABLE == true)
    /* Ensure lower layers opened successfully */
    ASSERT(TOUCH_SUCCESS==touch_err);
#endif

    if(TOUCH_SUCCESS==touch_err)
    {
        /* Find an open control block and add to it. */
        uint32_t itr = already_open(&ctrl);
        if(BUTTON_CFG_MAX_CONTROL_BLOCK_COUNT > itr)
        {
            /* Found a matching control block */
            *handle = itr;
            button_err = TOUCH_BUTTON_SUCCESS;
        }
        else
        {   /* Get the first open control block and write to it. */
            for( itr = 0; itr < NUM_CTRL_BLOCKS; itr++)
            {
                touch_button_ctrl_t * p_ctrl = &pvt_button_ctrl_blk[itr];
                if (OPEN != p_ctrl->open)
                {
                    memcpy(p_ctrl, &ctrl, sizeof(ctrl));
                    *handle = itr;
                    button_err = TOUCH_BUTTON_SUCCESS;
                    return button_err;
                }
            }

            if(BUTTON_CFG_MAX_CONTROL_BLOCK_COUNT <= itr)
            {
                button_err = TOUCH_BUTTON_ERR_INSUFFICIENT_MEMORY;
            }
        }
    }

    return button_err;
}
/***********************************************************************************************************************
End of function R_TOUCH_ButtonOpen
***********************************************************************************************************************/



/***********************************************************************************************************************
* Function Name: R_TOUCH_ButtonClose
* Description  : Close a Touch Button Control block.
* Arguments    : hdl - Control block identifier for the button. (Returned by successful call to R_TOUCH_ButtonOpen)
* Return Value : TOUCH_BUTTON_SUCCESS, TOUCH_BUTTON_ERR_INVALID_PARAM, or TOUCH_BUTTON_ERR_LOCKED
***********************************************************************************************************************/
touch_button_err_t R_TOUCH_ButtonClose(uint32_t hdl)
{
    touch_button_err_t err;
#if (BUTTON_CFG_PARAM_CHECKING_ENABLE == true)
    ASSERT(BUTTON_CFG_MAX_CONTROL_BLOCK_COUNT > hdl);
#endif
    touch_button_ctrl_t * p_ctrl = &pvt_button_ctrl_blk[hdl];
#if (BUTTON_CFG_PARAM_CHECKING_ENABLE == true)
    ASSERT(OPEN==p_ctrl->open);
#endif

    if(true==R_BSP_SoftwareLock(&p_ctrl->lock))
    {
        /* Not being used at the moment */
        touch_err_t touch_err = R_TOUCH_Close(p_ctrl->touch_hdl);
        if(TOUCH_SUCCESS == touch_err)
        {
            memset(p_ctrl, 0, sizeof(touch_button_ctrl_t));
            err = TOUCH_BUTTON_SUCCESS;
        }
        else
        {
            err = TOUCH_BUTTON_ERR_LOCKED;
        }
    }
    else
    {
        err = TOUCH_BUTTON_ERR_LOCKED;
    }
    return err;
}
/***********************************************************************************************************************
End of function R_TOUCH_ButtonClose
***********************************************************************************************************************/


/***********************************************************************************************************************
* Function Name: R_TOUCH_ButtonControl
* Description  : Control parameters associated with the operation of a button.
* Arguments    : hdl - Button Control Block Identifier (returned by a successfull call to R_Touch_Button_Open)
*                p_arg - Control arguments defining which parameter to get/set.
* Return Value : TOUCH_BUTTON_SUCCESS, TOUCH_BUTTON_ERR_INVALID_PARAM
***********************************************************************************************************************/
touch_button_err_t R_TOUCH_ButtonControl(uint32_t hdl, touch_button_control_arg_t * const p_arg)
{
    touch_button_err_t button_err;
#if (BUTTON_CFG_PARAM_CHECKING_ENABLE == true)
    ASSERT(BUTTON_CFG_MAX_CONTROL_BLOCK_COUNT > hdl);
    ASSERT(TOUCH_BUTTON_CMD_LAST > p_arg->cmd);
    ASSERT(NULL != p_arg->p_dest);
    ASSERT(0 < p_arg->size)
#endif
    touch_button_ctrl_t * p_ctrl = &pvt_button_ctrl_blk[hdl];
#if (BUTTON_CFG_PARAM_CHECKING_ENABLE == true)
    ASSERT(OPEN==p_ctrl->open);
#endif

    if(TOUCH_BUTTON_GET_LAST > p_arg->cmd)
    {
        button_err = get_control_block_parameter(p_ctrl, p_arg);
    }
    else
    {
        button_err = set_control_block_parameter(p_ctrl, p_arg);
    }

#if (BUTTON_CFG_PARAM_CHECKING_ENABLE == true)
    ASSERT(button_err == TOUCH_BUTTON_SUCCESS);
#endif

    return button_err;
}
/***********************************************************************************************************************
End of function R_TOUCH_ButtonControl
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: touch_button_update
* Description  : Update parameters controlling the state of the button.
* Arguments    : p_ctrl - Pointer to button control block
*                p_output -
*                    Pointer of where to put the output data
* Return Value : Button callback arguments identifying the state of the button.
***********************************************************************************************************************/
static touch_button_callback_arg_t touch_button_update(touch_button_ctrl_t * const p_ctrl)
{
    uint8_t binary;
    touch_err_t touch_err;
    touch_button_callback_arg_t button_arg;
    touch_read_t read_arg = {
        .read_cmd = TOUCH_DATA_BINARY,
        .sensor = &p_ctrl->button,
        .sensor_count = 1,
        .p_dest = &binary,
        .size = sizeof(binary),
    };
    binary = 0;
    button_arg.event = 0;
    button_arg.p_context = NULL;
    touch_err = R_TOUCH_Read(p_ctrl->touch_hdl, &read_arg);

    if (TOUCH_SUCCESS == touch_err)
    {
        if((binary & 0x01)==1)
        {
            /* Binary says 'touched' */
            if(TOUCH_BUTTON_EVENT_RELEASED==p_ctrl->state)
            {
                /* Current State := released */
                if(true==debounce(&p_ctrl->debounce_counter, p_ctrl->debounce))
                {
                    /* Button has been debounced */
                    p_ctrl->state = TOUCH_BUTTON_EVENT_PRESSED;
                    button_arg.event = (true == p_ctrl->enable.press) ? TOUCH_BUTTON_EVENT_PRESSED : button_arg.event;
                }
            }
            else if (TOUCH_BUTTON_EVENT_PRESSED == p_ctrl->state)
            {
                debounce (&p_ctrl->hold_counter, p_ctrl->hold_max);
            }
            else
            {
                ; /* GSCE CODAN Error Elimination */
            }
        }
        else
        {   /* Binary says 'not touched' */
            if(TOUCH_BUTTON_EVENT_PRESSED==p_ctrl->state)
            {
                /* Button was touched; release it! */
                p_ctrl->state = TOUCH_BUTTON_EVENT_RELEASED;
                button_arg.event = (true == p_ctrl->enable.release) ? TOUCH_BUTTON_EVENT_RELEASED : button_arg.event ;
                if(p_ctrl->hold_counter < p_ctrl->hold_max)
                {
                    /* Release within defined hold timing */
                    button_arg.event = (true == p_ctrl->enable.hold) ? TOUCH_BUTTON_EVENT_HOLD : button_arg.event;
                }
                p_ctrl->debounce_counter = 0;
                p_ctrl->hold_counter = 0;
            }
        }
    }
    return button_arg;
}
/***********************************************************************************************************************
End of function touch_button_update
***********************************************************************************************************************/

/**
 *
 *  p_current Pointer to current value of the debounce.
 * @param threshold Pointer to the maximum value beyond which button is considered to be debounced.
 * @return true - Button is debounced;  false - Button is not debounced.
 */
/***********************************************************************************************************************
* Function Name: debounce
* Description  : Verify if button has been debounced.
* Arguments    : @param p_current - Pointer to current value of the debounce.
*                @param threshold - Pointer to the maximum value beyond which button is considered to be debounced.
* Return Value : @return true - Button is debounced;  false - Button is not debounced.
***********************************************************************************************************************/
static bool debounce(uint16_t * p_current, uint16_t const threshold)
{
    uint16_t current = *p_current;

    if(current >= threshold)
    {
        return true;
    }
    else
    {
        current += 1;
        *p_current = current;
        return false;
    }
    return false;
}
/***********************************************************************************************************************
End of function debounce
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: already_open
* Description  : Verify if a matching control block is found.
* Arguments    : @param ctrl - Pointer to control block to be used for comparison.
* Return Value : @return Index of a matching control block (if any).
***********************************************************************************************************************/
static uint8_t already_open(touch_button_ctrl_t const*const ctrl)
{
    /* Run through all the open control blocks to check for a match */
    uint8_t itr;
    for( itr = 0; itr < NUM_CTRL_BLOCKS; itr++)
    {
        touch_button_ctrl_t * p_ctrl = &pvt_button_ctrl_blk[itr];
        if (OPEN == p_ctrl->open)
        {
            if(0==memcmp(p_ctrl, &ctrl, offsetof(touch_button_ctrl_t, touch_hdl)))
            {
                /* Matches a previously opened button! */
                return itr;
            }
        }
    }
    return itr;
}
/***********************************************************************************************************************
End of function already_open
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: get_control_block_parameter
* Description  : Get value of button control parameter from the control block.
* Arguments    : @param p_ctrl - Control block from which parameter must be extracted.
*                @param p_arg - Control arguments passed by the application.
* Return Value : @return TOUCH_BUTTON_SUCCESS, TOUCH_BUTTON_ERR_INVALID_PARAM
***********************************************************************************************************************/
static touch_button_err_t get_control_block_parameter(touch_button_ctrl_t * p_ctrl, touch_button_control_arg_t * const p_arg)
{
    touch_button_cmd_t cmd = p_arg->cmd;
    size_t offset = 0;
    size_t size = 0;
    uint8_t * base_addr = (uint8_t *)p_ctrl;

    switch(cmd)
    {
        case TOUCH_BUTTON_GET_CHANNELS:
            offset = offsetof(touch_button_ctrl_t, button);
            size = sizeof(((touch_button_ctrl_t *)0)->button);
            break;

        case TOUCH_BUTTON_GET_ENABLE_MASK:
            offset = offsetof(touch_button_ctrl_t, enable);
            size = sizeof(((touch_button_ctrl_t *)0)->enable);
            break;

        case TOUCH_BUTTON_GET_CALLBACK:
            offset = offsetof(touch_button_ctrl_t, p_callback);
            size = sizeof(((touch_button_ctrl_t *)0)->p_callback);
            break;

        case TOUCH_BUTTON_GET_TOUCH_HANDLE:
            offset = offsetof(touch_button_ctrl_t, touch_hdl);
            size = sizeof(((touch_button_ctrl_t *)0)->touch_hdl);
            break;


        case TOUCH_BUTTON_GET_STATE:
            offset = offsetof(touch_button_ctrl_t, state);
            size = sizeof(((touch_button_ctrl_t *)0)->state);
            break;

        case TOUCH_BUTTON_GET_DEBOUNCE_COUNTER:
            offset = offsetof(touch_button_ctrl_t, debounce_counter);
            size = sizeof(((touch_button_ctrl_t *)0)->debounce_counter);
            break;

        case TOUCH_BUTTON_GET_HOLD_COUNTER:
            offset = offsetof(touch_button_ctrl_t, hold_counter);
            size = sizeof(((touch_button_ctrl_t *)0)->hold_counter);
            break;
        default:
            return TOUCH_BUTTON_ERR_INVALID_PARAM;
            break;
    };

#if (BUTTON_CFG_PARAM_CHECKING_ENABLE == true)
    ASSERT(size <= p_arg->size);
#endif
    memcpy(p_arg->p_dest, (base_addr + offset), size);

    return TOUCH_BUTTON_SUCCESS;
}
/***********************************************************************************************************************
End of function get_control_block_parameter
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: set_control_block_parameter
* Description  : Set value of a button control parameter in the control block.
* Arguments    : @param p_ctrl Control block for which parameter must be changed.
*                @param p_arg Control arguments passed by the application.
* Return Value : @return TOUCH_BUTTON_SUCCESS, TOUCH_BUTTON_ERR_INVALID_PARAM
***********************************************************************************************************************/
static touch_button_err_t set_control_block_parameter(touch_button_ctrl_t * p_ctrl, touch_button_control_arg_t * const p_arg)
{
    touch_button_cmd_t cmd = p_arg->cmd;
    size_t offset = 0;
    size_t size = 0;
    uint8_t * base_addr = (uint8_t *)p_ctrl;

    switch(cmd)
    {
        case TOUCH_BUTTON_SET_CHANNELS:
            offset = offsetof(touch_button_ctrl_t, button);
            size = sizeof(((touch_button_ctrl_t *)0)->button);
            break;

        case TOUCH_BUTTON_SET_ENABLE_MASK:
            offset = offsetof(touch_button_ctrl_t, enable);
            size = sizeof(((touch_button_ctrl_t *)0)->enable);
            break;
        case TOUCH_BUTTON_SET_CALLBACK:
            offset = offsetof(touch_button_ctrl_t, p_callback);
            size = sizeof(((touch_button_ctrl_t *)0)->p_callback);
            break;
        case TOUCH_BUTTON_SET_TOUCH_HANDLE:
            offset = offsetof(touch_button_ctrl_t, touch_hdl);
            size = sizeof(((touch_button_ctrl_t *)0)->touch_hdl);
            break;
        case TOUCH_BUTTON_SET_STATE:
            offset = offsetof(touch_button_ctrl_t, state);
            size = sizeof(((touch_button_ctrl_t *)0)->state);
            break;
        case TOUCH_BUTTON_SET_DEBOUNCE_COUNTER:
            offset = offsetof(touch_button_ctrl_t, debounce_counter);
            size = sizeof(((touch_button_ctrl_t *)0)->debounce_counter);
            break;
        case TOUCH_BUTTON_SET_HOLD_COUNTER:
            offset = offsetof(touch_button_ctrl_t, hold_counter);
            size = sizeof(((touch_button_ctrl_t *)0)->hold_counter);
            break;

        default:
            return TOUCH_BUTTON_ERR_INVALID_PARAM;
            break;
    }

#if (BUTTON_CFG_PARAM_CHECKING_ENABLE == true)
    ASSERT(size <= p_arg->size);
#endif
    memcpy((base_addr + offset), p_arg->p_dest, size);

    return TOUCH_BUTTON_SUCCESS;
}
/***********************************************************************************************************************
End of function set_control_block_parameter
***********************************************************************************************************************/

/***********************************************************************************************************************
* Function Name: touch_button_callback
* Description  : Common callback for all buttons invoked by the lower layers.
* Arguments    : @param arg - Argument passed by the lower layer (of type touch_callback_arg_t)
* Return Value : None
***********************************************************************************************************************/
static void touch_button_callback(void * arg)
{
    touch_button_callback_arg_t button_arg;
    touch_callback_arg_t const * const p_arg = arg;
    touch_event_t event = (touch_event_t)p_arg->event;

    if ((event & TOUCH_EVENT_REQUEST_DELAY) == TOUCH_EVENT_REQUEST_DELAY)
    {
        /* Lower layer requesting a delay */
        button_arg.event = TOUCH_BUTTON_EVENT_REQUEST_DELAY;
        button_arg.id = UINT32_MAX;
        button_arg.p_context = NULL;
        for(uint32_t itr = 0; itr < NUM_CTRL_BLOCKS; itr++)
        {
            if(pvt_button_ctrl_blk[itr].p_callback!=NULL)
            {
                /* Found a valid callback to use */
                pvt_button_ctrl_blk[itr].p_callback(&button_arg);
                return;
            }
        }
    }
    else if ((event & TOUCH_EVENT_PARAMETERS_UPDATED) == TOUCH_EVENT_PARAMETERS_UPDATED)
    {
        for (uint32_t itr = 0; itr < NUM_CTRL_BLOCKS; itr++)
        {
            if ((0x4F50454e) == pvt_button_ctrl_blk[itr].open)
            {
                if (pvt_button_ctrl_blk[itr].touch_hdl == p_arg->handle_num)
                {
                    button_arg = touch_button_update (&pvt_button_ctrl_blk[itr]);
                    button_arg.id = itr;
                }
                if ((p_arg->info > 1) && (TOUCH_BUTTON_EVENT_PRESSED == button_arg.event))
                {
                    button_arg.event |= TOUCH_BUTTON_EVENT_MULTI_TOUCH;
                }
                if ((button_arg.event > 0) && (0 != pvt_button_ctrl_blk[itr].p_callback))
                {
                        pvt_button_ctrl_blk[itr].p_callback (&button_arg);
                }
            }
        }
    }
    else
    {
        ; /* GSCE CODAN Error Elimination */
    }
}
/***********************************************************************************************************************
End of function touch_button_callback
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @} (end defgroup R_Touch)
 ***********************************************************************************************************************/
