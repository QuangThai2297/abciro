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
* File Name : r_ctsu_rx_private.h
* Version : 1.00: [2016-07-29] Released as Version 1.00.
*			1.10: [TBD]
* Description : This module provides the interface to the R_CTSU peripheral.
***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @file
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 Includes
 ***********************************************************************************************************************/
#ifndef SRC_R_CTSU_RX_PRIVATE_HEADER
#define SRC_R_CTSU_RX_PRIVATE_HEADER

#if (CTSU_CFG_DTC_TRANSFER_ENABLE == true)
#if (CTSU_CFG_DTC_CHAIN_TRANSFERS == true)
#define DTC_TCB_COUNT    (3)
#else
#define DTC_TCB_COUNT    (1)
#endif  //(CTSU_CFG_DTC_CHAIN_TRANSFERS == true)
#endif

/** Definition of DTC Transfer Control Block. */
#if defined(__RENESAS__)
/* Identified Renesas Compiler */
#pragma bit_order left
#pragma unpack
#endif /* defined(__RENESAS__) */

#if defined(__IAR_SYSTEMS_ICC__)
/* IAR Compiler */
#pragma bitfields=reversed
#pragma pack()
#endif /* #if defined(__IAR_SYSTEMS_ICC__) */

typedef union dtc_mra {
    uint8_t BYTE;
    struct {
        uint8_t MD:2; /* b7,b6: DTC Transfer Mode Select */
        uint8_t SZ:2; /* DTC Data Transfer Size */
        uint8_t SM:2; /* Transfer Source Address Addressing Mode */
        uint8_t rs:2; /* reserved */
    } BIT;

} dtc_mra_t;

/* The DTC Mode Register B (MRB) structure */
typedef union dtc_mrb {
    uint8_t BYTE;
    struct {
        uint8_t CHNE :1;  /* b7: DTC Chain Transfer Enable */
        uint8_t CHNS :1;  /* DTC Chain Transfer Select */
        uint8_t DISEL:1;  /* DTC Interrupt Select */
        uint8_t DTS  :1;  /* DTC Transfer Mode Select */
        uint8_t DM   :2;  /* Transfer Destination Address Addressing Mode */
        uint8_t rs   :2;  /* reserved */
    } BIT;

} dtc_mrb_t;

/* The DTC Transfer Count Register A (CRA) structure */
typedef union dtc_cra {
    uint16_t WORD;
    struct {
#if (__BIG) ||(__RX_BIG_ENDIAN__) /* Big-Endian */
        uint8_t CRA_H;
        uint8_t CRA_L;
#else /* little endian */
        uint8_t CRA_L;
        uint8_t CRA_H;
#endif
    } BYTE;
} dtc_cra_t;

/* The DTC Transfer Count Register B (CRB) structure */
typedef union dtc_crb {
    uint16_t WORD;
} dtc_crb_t;

typedef struct st_dtc_full_transfer_data {
    union {
        uint32_t LWORD;
        struct {
#if (__BIG) ||(__RX_BIG_ENDIAN__) /* Big-Endian */
            dtc_mra_t MRA;
            dtc_mrb_t MRB;
            uint16_t  reserver; /* reserve area */
#else /* Little-Endian */
            uint16_t  reserver; /* reserve area */
            dtc_mrb_t MRB;
            dtc_mra_t MRA;
#endif /* (DTC_BIG_ENDIAN) */
        } REG;
    } FIRST_LWORD;
    union {
        uint32_t SAR;
    } SECOND_LWORD;
    union {
        uint32_t DAR;
    } THIRD_LWORD;
    union {
        uint32_t LWORD;
        struct {
#if (__BIG) ||(__RX_BIG_ENDIAN__) /* Big-Endian */
            dtc_cra_t CRA;
            dtc_crb_t CRB;
#else /* Little-Endian */
            dtc_crb_t CRB;
            dtc_cra_t CRA;
#endif /* (DTC_BIG_ENDIAN) */
        } REG;
    } FOURTH_LWORD;
} dtc_internal_registers_t;
#if defined(__RENESAS__)
/* Identified Renesas RX Compiler */
#pragma bit_order
#pragma packoption
#endif /* defined(__RENESAS__) */

#if defined(__IAR_SYSTEMS_ICC__)
/* IAR Compiler */
#pragma bitfields=default
#endif /* #if defined(__IAR_SYSTEMS_ICC__) */

/**
 * Types and order of counter values generated by the CTSU when measuring capacitance.
 */
typedef enum e_sensor_count_t
{
    OFFSET_PRIMARY_SENSOR_COUNT = 0,     ///< Index of primary sensor ICO in CTSU sensor measurement
    OFFSET_PRIMARY_REFERENCE_COUNT = 1,  ///< Index of primary reference ICO in CTSU sensor measurement
    OFFSET_SECONDARY_SENSOR_COUNT = 2,   ///< Index of secondary sensor ICO in CTSU sensor measurement
    OFFSET_SECONDARY_REFERENCE_COUNT = 3,///< Index of secondary reference ICO in CTSU sensor measurement
}sensor_count_t;

typedef enum
{
    _19_2UA = 0,
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
    _01_2UA,
    _00_0UA
}sensor_current_t;

/**
 * Definition of a CTSU operation handle (used to copy information from the configuration, and operate CTSU in specified mode).
 */
typedef struct st_ctsu_ctrl_blk
{
        ctsu_const_sfrs_t * p_ctsu_settings;    ///< CTSU SFRs associated with CTSU operation with this handle (Common to all sensors).
        ctsu_sensor_setting_t* p_sensor_settings;   ///< CTSU SFRs associated with each sensor in this handle (Unique to each sensor).
        void * p_sensor_data;                       ///< Location in memory where CTSU measurement data is output.
        uint32_t pclkb_hz;                  ///< Holds the frequency with which PCLKB should be running to scan this handle.
        ctsu_mode_t mode;                   ///< Indicates the operating mode of the CTSU.
        uint16_t num_sensors;               ///< Holds the number of sensors being operated with this channel.
        uint16_t num_wr_irq_requests;       ///< Expected number of write requests that will occur during a complete scan. Should equal to @ref num_sensors.
        uint16_t num_rd_irq_requests;       ///< Expected number of read requests that will occur during a complete scan. Should equal -OR- 2x  @ref num_sensors.


        uint32_t open;                      ///< Value 0x4F50454e indicates handle was opened by a call to @ref R_CTSU_Open.
        bsp_lock_t lock;                    ///< Binary lock (using xchg instruction) to prevent multiple functions from simultaneous access to same handle.
        fit_callback_t p_callback;          ///< Pointer to upper layer function (used for event notifications).
        uint16_t upper_level_count;         ///< Value indicating how many upper layers are using this configuration.
        uint8_t const tscal_pin;
        uint16_t const * const p_correction_ideal_sensor_ico;       ///< CTSU correction factor for primary sensor count readings measured from MCU used for calibration.
        uint16_t * const p_linear_interpolation_table;              ///< TODO: Documentation
        uint16_t * const p_linear_interpolation_coeff;              ///< TODO: Documentation
        int8_t * const p_magnification_sensor;                      ///< TODO: Documentation
        uint16_t correction_factor_sec_cur;     ///< CTSU correction factor for secondary sensor count measured from MCU other than one used for calibration.
        uint8_t correction_ctsuso_delta;        ///< Change in ctsuso for measuring correction factor.
        uint32_t scan_errors;                       ///< Holds the information about any errors that may have occured during last scan.
        bool reload;
#if (CTSU_CFG_DTC_TRANSFER_ENABLE == true)
        dtc_internal_registers_t ctsu_wr[DTC_TCB_COUNT];    ///< DTC TCB chain to use for the CTSUWR interrupt.
        dtc_internal_registers_t ctsu_rd[DTC_TCB_COUNT];    ///< DTC TCB chain to use for the CTSURD interrupt.
#endif
}ctsu_ctrl_blk_t;                                   ///< Refer: @ref st_ctsu_ctrl_blk
#endif /* SRC_R_CTSU_RX_PRIVATE_HEADER */
