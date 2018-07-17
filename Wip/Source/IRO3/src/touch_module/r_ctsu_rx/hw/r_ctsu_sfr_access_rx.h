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
 *
 * NOTE: The contents of this file are confidential, and are covered under the SC32 non-disclosure agreement.
 ***********************************************************************************************************************/
/**********************************************************************************************************************
 * File Name    : r_ctsu_sfr_access_rx.h
 * Description  : This file implements the Hardware Abstraction Layer for the CTSU module. Provides functions to
 * 					manipulate SFRs.
 ***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *           12.11.2014 1.00    Initial Release.
 ***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @file
 ***********************************************************************************************************************/
/*******************************************************************************************************************//**
 * @ingroup HAL_Library
 * @defgroup r_ctsu_hw r_ctsu_hw
 * @brief Provides functions to manipulate Special Function Registers which operate the CTSU.
 *
 *  It implements the following interface:
 *  - @ref r_ctsu_sfr_access_rx.h
 * @{
 ***********************************************************************************************************************/

#ifndef R_CTSU_SFR_ACCESS_RX_H_
#define R_CTSU_SFR_ACCESS_RX_H_

#pragma inline (HW_CTSU_CTSUCR0Get)
static uint8_t HW_CTSU_CTSUCR0Get(void) {
	return CTSU.CTSUCR0.BYTE;
}
#pragma inline (HW_CTSU_CTSUCR1Get)
static uint8_t HW_CTSU_CTSUCR1Get(void) {
	return CTSU.CTSUCR1.BYTE;
}
#pragma inline (HW_CTSU_CTSUSDPRSGet)
static uint8_t HW_CTSU_CTSUSDPRSGet(void) {
	return CTSU.CTSUSDPRS.BYTE;
}
#pragma inline (HW_CTSU_CTSUSSTGet)
static uint8_t HW_CTSU_CTSUSSTGet(void) {
	return CTSU.CTSUSST.BYTE;
}
#pragma inline (HW_CTSU_CTSUMCH0Get)
static uint8_t HW_CTSU_CTSUMCH0Get(void) {
	return CTSU.CTSUMCH0.BYTE;
}
#pragma inline (HW_CTSU_CTSUMCH1Get)
static uint8_t HW_CTSU_CTSUMCH1Get(void) {
	return CTSU.CTSUMCH1.BYTE;
}
#pragma inline (HW_CTSU_CTSUCHAC0Get)
static uint8_t HW_CTSU_CTSUCHAC0Get(void) {
	return CTSU.CTSUCHAC0.BYTE;
}
#pragma inline (HW_CTSU_CTSUCHAC1Get)
static uint8_t HW_CTSU_CTSUCHAC1Get(void) {
	return CTSU.CTSUCHAC1.BYTE;
}
#if !defined(BSP_MCU_RX113)
#pragma inline (HW_CTSU_CTSUCHAC2Get)
static uint8_t HW_CTSU_CTSUCHAC2Get(void) {
	return CTSU.CTSUCHAC2.BYTE;
}
#pragma inline (HW_CTSU_CTSUCHAC3Get)
static uint8_t HW_CTSU_CTSUCHAC3Get(void) {
	return CTSU.CTSUCHAC3.BYTE;
}
#pragma inline (HW_CTSU_CTSUCHAC4Get)
static uint8_t HW_CTSU_CTSUCHAC4Get(void) {
	return CTSU.CTSUCHAC4.BYTE;
}
#endif
#pragma inline (HW_CTSU_CTSUCHTRC0Get)
static uint8_t HW_CTSU_CTSUCHTRC0Get(void) {
	return CTSU.CTSUCHTRC0.BYTE;
}
#pragma inline (HW_CTSU_CTSUCHTRC1Get)
static uint8_t HW_CTSU_CTSUCHTRC1Get(void) {
	return CTSU.CTSUCHTRC1.BYTE;
}
#if !defined(BSP_MCU_RX113)
#pragma inline (HW_CTSU_CTSUCHTRC2Get)
static uint8_t HW_CTSU_CTSUCHTRC2Get(void) {
	return CTSU.CTSUCHTRC2.BYTE;
}
#pragma inline (HW_CTSU_CTSUCHTRC3Get)
static uint8_t HW_CTSU_CTSUCHTRC3Get(void) {
	return CTSU.CTSUCHTRC3.BYTE;
}
#pragma inline (HW_CTSU_CTSUCHTRC4Get)
static uint8_t HW_CTSU_CTSUCHTRC4Get(void) {
	return CTSU.CTSUCHTRC4.BYTE;
}
#endif
#pragma inline (HW_CTSU_CTSUDCLKCGet)
static uint8_t HW_CTSU_CTSUDCLKCGet(void) {
	return CTSU.CTSUDCLKC.BYTE;
}
#pragma inline (HW_CTSU_CTSUSTGet)
static uint8_t HW_CTSU_CTSUSTGet(void) {
	return CTSU.CTSUST.BYTE;
}
#pragma inline (HW_CTSU_CTSUSCGet)
static uint16_t HW_CTSU_CTSUSCGet(void) {
	return CTSU.CTSUSC.WORD;
}
#pragma inline (HW_CTSU_CTSURCGet)
static uint16_t HW_CTSU_CTSURCGet(void) {
	return CTSU.CTSURC.WORD;
}
#pragma inline (HW_CTSU_CTSUERRSGet)
static uint16_t HW_CTSU_CTSUERRSGet(void) {
	return CTSU.CTSUERRS.WORD;
}
#pragma inline (HW_CTSU_CTSUTRMRGet)
static uint8_t HW_CTSU_CTSUTRMRGet(void) {
    uint8_t * const p_dest = (uint8_t*)(0x007FFFBE);
    return *p_dest;
}

#pragma inline (HW_CTSU_CTSUCR0Set)
static void HW_CTSU_CTSUCR0Set(uint8_t value) {
	CTSU.CTSUCR0.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUCR1Set)
static void HW_CTSU_CTSUCR1Set(uint8_t value) {
	CTSU.CTSUCR1.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUSDPRSSet)
static void HW_CTSU_CTSUSDPRSSet(uint8_t value) {
	CTSU.CTSUSDPRS.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUSSTSet)
static void HW_CTSU_CTSUSSTSet(uint8_t value) {
	CTSU.CTSUSST.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUMCH0Set)
static void HW_CTSU_CTSUMCH0Set(uint8_t value) {
	CTSU.CTSUMCH0.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUMCH1Set)
static void HW_CTSU_CTSUMCH1Set(uint8_t value) {
	CTSU.CTSUMCH1.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUCHAC0Set)
static void HW_CTSU_CTSUCHAC0Set(uint8_t value) {
	CTSU.CTSUCHAC0.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUCHAC1Set)
static void HW_CTSU_CTSUCHAC1Set(uint8_t value) {
	CTSU.CTSUCHAC1.BYTE = value;
}
#if !defined(BSP_MCU_RX113)
#pragma inline (HW_CTSU_CTSUCHAC2Set)
static void HW_CTSU_CTSUCHAC2Set(uint8_t value) {
	CTSU.CTSUCHAC2.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUCHAC3Set)
static void HW_CTSU_CTSUCHAC3Set(uint8_t value) {
	CTSU.CTSUCHAC3.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUCHAC4Set)
static void HW_CTSU_CTSUCHAC4Set(uint8_t value) {
	CTSU.CTSUCHAC4.BYTE = value;
}
#endif
#pragma inline (HW_CTSU_CTSUCHTRC0Set)
static void HW_CTSU_CTSUCHTRC0Set(uint8_t value) {
	CTSU.CTSUCHTRC0.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUCHTRC1Set)
static void HW_CTSU_CTSUCHTRC1Set(uint8_t value) {
	CTSU.CTSUCHTRC1.BYTE = value;
}
#if !defined(BSP_MCU_RX113)
#pragma inline (HW_CTSU_CTSUCHTRC2Set)
static void HW_CTSU_CTSUCHTRC2Set(uint8_t value) {
	CTSU.CTSUCHTRC2.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUCHTRC3Set)
static void HW_CTSU_CTSUCHTRC3Set(uint8_t value) {
	CTSU.CTSUCHTRC3.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUCHTRC4Set)
static void HW_CTSU_CTSUCHTRC4Set(uint8_t value) {
	CTSU.CTSUCHTRC4.BYTE = value;
}
#endif
#pragma inline (HW_CTSU_CTSUDCLKCSet)
static void HW_CTSU_CTSUDCLKCSet(uint8_t value) {
	CTSU.CTSUDCLKC.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUSTSet)
static void HW_CTSU_CTSUSTSet(uint8_t value) {
	CTSU.CTSUST.BYTE = value;
}
#pragma inline (HW_CTSU_CTSUSSCSet)
static void HW_CTSU_CTSUSSCSet(uint16_t value) {
	CTSU.CTSUSSC.WORD = value;
}
#pragma inline (HW_CTSU_CTSUSO0Set)
static void HW_CTSU_CTSUSO0Set(uint16_t value) {
	CTSU.CTSUSO0.WORD = value;
}
#pragma inline (HW_CTSU_CTSUSO1Set)
static void HW_CTSU_CTSUSO1Set(uint16_t value) {
	CTSU.CTSUSO1.WORD = value;
}
#pragma inline (HW_CTSU_CTSUERRSSet)
static void HW_CTSU_CTSUERRSSet(uint16_t value) {
	CTSU.CTSUERRS.WORD = value;
}
#pragma inline (HW_CTSU_CTSUTRMRSet)
static void HW_CTSU_CTSUTRMRSet(uint8_t value) {
    uint8_t * const p_dest = (uint8_t*)(0x007FFFBE);
    *p_dest = value;
}

#pragma inline (HW_CTSU_CTSUCR0GetBitCTSUSTRT)
static uint32_t HW_CTSU_CTSUCR0GetBitCTSUSTRT(void) {
	return CTSU.CTSUCR0.BIT.CTSUSTRT;
}
#pragma inline (HW_CTSU_CTSUCR0GetBitCTSUCAP)
static uint32_t HW_CTSU_CTSUCR0GetBitCTSUCAP(void) {
	return CTSU.CTSUCR0.BIT.CTSUCAP;
}
#pragma inline (HW_CTSU_CTSUCR0GetBitCTSUSNZ)
static uint32_t HW_CTSU_CTSUCR0GetBitCTSUSNZ(void) {
	return CTSU.CTSUCR0.BIT.CTSUSNZ;
}
#pragma inline (HW_CTSU_CTSUCR0GetBitCTSUINIT)
static uint32_t HW_CTSU_CTSUCR0GetBitCTSUINIT(void) {
	return CTSU.CTSUCR0.BIT.CTSUINIT;
}
#pragma inline (HW_CTSU_CTSUCR1GetBitCTSUPON)
static uint32_t HW_CTSU_CTSUCR1GetBitCTSUPON(void) {
	return CTSU.CTSUCR1.BIT.CTSUPON;
}
#pragma inline (HW_CTSU_CTSUCR1GetBitCTSUCSW)
static uint32_t HW_CTSU_CTSUCR1GetBitCTSUCSW(void) {
	return CTSU.CTSUCR1.BIT.CTSUCSW;
}
#pragma inline (HW_CTSU_CTSUCR1GetBitCTSUATUNE0)
static uint32_t HW_CTSU_CTSUCR1GetBitCTSUATUNE0(void) {
	return CTSU.CTSUCR1.BIT.CTSUATUNE0;
}
#pragma inline (HW_CTSU_CTSUCR1GetBitCTSUATUNE1)
static uint32_t HW_CTSU_CTSUCR1GetBitCTSUATUNE1(void) {
	return CTSU.CTSUCR1.BIT.CTSUATUNE1;
}
#pragma inline (HW_CTSU_CTSUCR1GetBitCTSUCLK)
static uint32_t HW_CTSU_CTSUCR1GetBitCTSUCLK(void) {
	return CTSU.CTSUCR1.BIT.CTSUCLK;
}
#pragma inline (HW_CTSU_CTSUCR1GetBitCTSUMD)
static uint32_t HW_CTSU_CTSUCR1GetBitCTSUMD(void) {
	return CTSU.CTSUCR1.BIT.CTSUMD;
}
#pragma inline (HW_CTSU_CTSUSDPRSGetBitCTSUPRRATIO)
static uint32_t HW_CTSU_CTSUSDPRSGetBitCTSUPRRATIO(void) {
	return CTSU.CTSUSDPRS.BIT.CTSUPRRATIO;
}
#pragma inline (HW_CTSU_CTSUSDPRSGetBitCTSUPRMODE)
static uint32_t HW_CTSU_CTSUSDPRSGetBitCTSUPRMODE(void) {
	return CTSU.CTSUSDPRS.BIT.CTSUPRMODE;
}
#pragma inline (HW_CTSU_CTSUSDPRSGetBitCTSUSOFF)
static uint32_t HW_CTSU_CTSUSDPRSGetBitCTSUSOFF(void) {
	return CTSU.CTSUSDPRS.BIT.CTSUSOFF;
}
#pragma inline (HW_CTSU_CTSUSSTGetBitCTSUSST)
static uint32_t HW_CTSU_CTSUSSTGetBitCTSUSST(void) {
	return CTSU.CTSUSST.BIT.CTSUSST;
}
#pragma inline (HW_CTSU_CTSUMCH0GetBitCTSUMCH0)
static uint32_t HW_CTSU_CTSUMCH0GetBitCTSUMCH0(void) {
	return CTSU.CTSUMCH0.BIT.CTSUMCH0;
}
#pragma inline (HW_CTSU_CTSUMCH1GetBitCTSUMCH1)
static uint32_t HW_CTSU_CTSUMCH1GetBitCTSUMCH1(void) {
	return CTSU.CTSUMCH1.BIT.CTSUMCH1;
}
#pragma inline (HW_CTSU_CTSUCHAC0GetBitCTSUCHAC00)
static uint32_t HW_CTSU_CTSUCHAC0GetBitCTSUCHAC00(void) {
	return CTSU.CTSUCHAC0.BIT.CTSUCHAC00;
}
#pragma inline (HW_CTSU_CTSUCHAC0GetBitCTSUCHAC01)
static uint32_t HW_CTSU_CTSUCHAC0GetBitCTSUCHAC01(void) {
	return CTSU.CTSUCHAC0.BIT.CTSUCHAC01;
}
#pragma inline (HW_CTSU_CTSUCHAC0GetBitCTSUCHAC02)
static uint32_t HW_CTSU_CTSUCHAC0GetBitCTSUCHAC02(void) {
	return CTSU.CTSUCHAC0.BIT.CTSUCHAC02;
}
#pragma inline (HW_CTSU_CTSUCHAC0GetBitCTSUCHAC03)
static uint32_t HW_CTSU_CTSUCHAC0GetBitCTSUCHAC03(void) {
	return CTSU.CTSUCHAC0.BIT.CTSUCHAC03;
}
#pragma inline (HW_CTSU_CTSUCHAC0GetBitCTSUCHAC04)
static uint32_t HW_CTSU_CTSUCHAC0GetBitCTSUCHAC04(void) {
	return CTSU.CTSUCHAC0.BIT.CTSUCHAC04;
}
#pragma inline (HW_CTSU_CTSUCHAC0GetBitCTSUCHAC05)
static uint32_t HW_CTSU_CTSUCHAC0GetBitCTSUCHAC05(void) {
	return CTSU.CTSUCHAC0.BIT.CTSUCHAC05;
}
#pragma inline (HW_CTSU_CTSUCHAC0GetBitCTSUCHAC06)
static uint32_t HW_CTSU_CTSUCHAC0GetBitCTSUCHAC06(void) {
	return CTSU.CTSUCHAC0.BIT.CTSUCHAC06;
}
#pragma inline (HW_CTSU_CTSUCHAC0GetBitCTSUCHAC07)
static uint32_t HW_CTSU_CTSUCHAC0GetBitCTSUCHAC07(void) {
	return CTSU.CTSUCHAC0.BIT.CTSUCHAC07;
}
#pragma inline (HW_CTSU_CTSUCHAC1GetBitCTSUCHAC10)
static uint32_t HW_CTSU_CTSUCHAC1GetBitCTSUCHAC10(void) {
	return CTSU.CTSUCHAC1.BIT.CTSUCHAC10;
}
#pragma inline (HW_CTSU_CTSUCHAC1GetBitCTSUCHAC11)
static uint32_t HW_CTSU_CTSUCHAC1GetBitCTSUCHAC11(void) {
	return CTSU.CTSUCHAC1.BIT.CTSUCHAC11;
}
#pragma inline (HW_CTSU_CTSUCHAC1GetBitCTSUCHAC12)
static uint32_t HW_CTSU_CTSUCHAC1GetBitCTSUCHAC12(void) {
	return CTSU.CTSUCHAC1.BIT.CTSUCHAC12;
}
#pragma inline (HW_CTSU_CTSUCHAC1GetBitCTSUCHAC13)
static uint32_t HW_CTSU_CTSUCHAC1GetBitCTSUCHAC13(void) {
	return CTSU.CTSUCHAC1.BIT.CTSUCHAC13;
}
#pragma inline (HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC00)
static uint32_t HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC00(void) {
	return CTSU.CTSUCHTRC0.BIT.CTSUCHTRC00;
}
#pragma inline (HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC01)
static uint32_t HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC01(void) {
	return CTSU.CTSUCHTRC0.BIT.CTSUCHTRC01;
}
#pragma inline (HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC02)
static uint32_t HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC02(void) {
	return CTSU.CTSUCHTRC0.BIT.CTSUCHTRC02;
}
#pragma inline (HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC03)
static uint32_t HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC03(void) {
	return CTSU.CTSUCHTRC0.BIT.CTSUCHTRC03;
}
#pragma inline (HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC04)
static uint32_t HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC04(void) {
	return CTSU.CTSUCHTRC0.BIT.CTSUCHTRC04;
}
#pragma inline (HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC05)
static uint32_t HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC05(void) {
	return CTSU.CTSUCHTRC0.BIT.CTSUCHTRC05;
}
#pragma inline (HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC06)
static uint32_t HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC06(void) {
	return CTSU.CTSUCHTRC0.BIT.CTSUCHTRC06;
}
#pragma inline (HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC07)
static uint32_t HW_CTSU_CTSUCHTRC0GetBitCTSUCHTRC07(void) {
	return CTSU.CTSUCHTRC0.BIT.CTSUCHTRC07;
}
#pragma inline (HW_CTSU_CTSUCHTRC1GetBitCTSUCHTRC10)
static uint32_t HW_CTSU_CTSUCHTRC1GetBitCTSUCHTRC10(void) {
	return CTSU.CTSUCHTRC1.BIT.CTSUCHTRC10;
}
#pragma inline (HW_CTSU_CTSUCHTRC1GetBitCTSUCHTRC11)
static uint32_t HW_CTSU_CTSUCHTRC1GetBitCTSUCHTRC11(void) {
	return CTSU.CTSUCHTRC1.BIT.CTSUCHTRC11;
}
#pragma inline (HW_CTSU_CTSUCHTRC1GetBitCTSUCHTRC12)
static uint32_t HW_CTSU_CTSUCHTRC1GetBitCTSUCHTRC12(void) {
	return CTSU.CTSUCHTRC1.BIT.CTSUCHTRC12;
}
#pragma inline (HW_CTSU_CTSUCHTRC1GetBitCTSUCHTRC13)
static uint32_t HW_CTSU_CTSUCHTRC1GetBitCTSUCHTRC13(void) {
	return CTSU.CTSUCHTRC1.BIT.CTSUCHTRC13;
}
#pragma inline (HW_CTSU_CTSUDCLKCGetBitCTSUSSMOD)
static uint32_t HW_CTSU_CTSUDCLKCGetBitCTSUSSMOD(void) {
	return CTSU.CTSUDCLKC.BIT.CTSUSSMOD;
}
#pragma inline (HW_CTSU_CTSUDCLKCGetBitCTSUSSCNT)
static uint32_t HW_CTSU_CTSUDCLKCGetBitCTSUSSCNT(void) {
	return CTSU.CTSUDCLKC.BIT.CTSUSSCNT;
}
#pragma inline (HW_CTSU_CTSUSTGetBitCTSUSTC)
static uint32_t HW_CTSU_CTSUSTGetBitCTSUSTC(void) {
	return CTSU.CTSUST.BIT.CTSUSTC;
}
#pragma inline (HW_CTSU_CTSUSTGetBitCTSUDTSR)
static uint32_t HW_CTSU_CTSUSTGetBitCTSUDTSR(void) {
	return CTSU.CTSUST.BIT.CTSUDTSR;
}
#pragma inline (HW_CTSU_CTSUSTGetBitCTSUSOVF)
static uint32_t HW_CTSU_CTSUSTGetBitCTSUSOVF(void) {
	return CTSU.CTSUST.BIT.CTSUSOVF;
}
#pragma inline (HW_CTSU_CTSUSTGetBitCTSUROVF)
static uint32_t HW_CTSU_CTSUSTGetBitCTSUROVF(void) {
	return CTSU.CTSUST.BIT.CTSUROVF;
}
#pragma inline (HW_CTSU_CTSUSTGetBitCTSUPS)
static uint32_t HW_CTSU_CTSUSTGetBitCTSUPS(void) {
	return CTSU.CTSUST.BIT.CTSUPS;
}
#pragma inline (HW_CTSU_CTSUSCGetBitCTSUSC)
static uint32_t HW_CTSU_CTSUSCGetBitCTSUSC(void) {
	return CTSU.CTSUSC.BIT.CTSUSC;
}
#pragma inline (HW_CTSU_CTSURCGetBitCTSURC)
static uint32_t HW_CTSU_CTSURCGetBitCTSURC(void) {
	return CTSU.CTSURC.BIT.CTSURC;
}
#pragma inline (HW_CTSU_CTSUERRSGetBitCTSUICOMP)
static uint32_t HW_CTSU_CTSUERRSGetBitCTSUICOMP(void) {
	return CTSU.CTSUERRS.BIT.CTSUICOMP;
}


#pragma inline (HW_CTSU_CTSUCR0SetBitCTSUSTRT)
static void HW_CTSU_CTSUCR0SetBitCTSUSTRT(uint32_t value) {
	CTSU.CTSUCR0.BIT.CTSUSTRT =
			value;
}
#pragma inline (HW_CTSU_CTSUCR0SetBitCTSUCAP)
static void HW_CTSU_CTSUCR0SetBitCTSUCAP(uint32_t value) {
	CTSU.CTSUCR0.BIT.CTSUCAP =
			value;
}
#pragma inline (HW_CTSU_CTSUCR0SetBitCTSUSNZ)
static void HW_CTSU_CTSUCR0SetBitCTSUSNZ(uint32_t value) {
	CTSU.CTSUCR0.BIT.CTSUSNZ =
			value;
}
#pragma inline (HW_CTSU_CTSUCR0SetBitCTSUINIT)
static void HW_CTSU_CTSUCR0SetBitCTSUINIT(uint32_t value) {
	CTSU.CTSUCR0.BIT.CTSUINIT =
			value;
}
#pragma inline (HW_CTSU_CTSUCR1SetBitCTSUPON)
static void HW_CTSU_CTSUCR1SetBitCTSUPON(uint32_t value) {
	CTSU.CTSUCR1.BIT.CTSUPON =
			value;
}
#pragma inline (HW_CTSU_CTSUCR1SetBitCTSUCSW)
static void HW_CTSU_CTSUCR1SetBitCTSUCSW(uint32_t value) {
	CTSU.CTSUCR1.BIT.CTSUCSW =
			value;
}
#pragma inline (HW_CTSU_CTSUCR1SetBitCTSUATUNE0)
static void HW_CTSU_CTSUCR1SetBitCTSUATUNE0(uint32_t value) {
	CTSU.CTSUCR1.BIT.CTSUATUNE0 =
			value;
}
#pragma inline (HW_CTSU_CTSUCR1SetBitCTSUATUNE1)
static void HW_CTSU_CTSUCR1SetBitCTSUATUNE1(uint32_t value) {
	CTSU.CTSUCR1.BIT.CTSUATUNE1 =
			value;
}
#pragma inline (HW_CTSU_CTSUCR1SetBitCTSUCLK)
static void HW_CTSU_CTSUCR1SetBitCTSUCLK(uint32_t value) {
	CTSU.CTSUCR1.BIT.CTSUCLK =
			value;
}
#pragma inline (HW_CTSU_CTSUCR1SetBitCTSUMD)
static void HW_CTSU_CTSUCR1SetBitCTSUMD(uint32_t value) {
	CTSU.CTSUCR1.BIT.CTSUMD =
			value;
}
#pragma inline (HW_CTSU_CTSUSDPRSSetBitCTSUPRRATIO)
static void HW_CTSU_CTSUSDPRSSetBitCTSUPRRATIO(uint32_t value) {
	CTSU.CTSUSDPRS.BIT.CTSUPRRATIO =
			value;
}
#pragma inline (HW_CTSU_CTSUSDPRSSetBitCTSUPRMODE)
static void HW_CTSU_CTSUSDPRSSetBitCTSUPRMODE(uint32_t value) {
	CTSU.CTSUSDPRS.BIT.CTSUPRMODE =
			value;
}
#pragma inline (HW_CTSU_CTSUSDPRSSetBitCTSUSOFF)
static void HW_CTSU_CTSUSDPRSSetBitCTSUSOFF(uint32_t value) {
	CTSU.CTSUSDPRS.BIT.CTSUSOFF =
			value;
}
#pragma inline (HW_CTSU_CTSUSSTSetBitCTSUSST)
static void HW_CTSU_CTSUSSTSetBitCTSUSST(uint32_t value) {
	CTSU.CTSUSST.BIT.CTSUSST =
			value;
}
#pragma inline (HW_CTSU_CTSUMCH0SetBitCTSUMCH0)
static void HW_CTSU_CTSUMCH0SetBitCTSUMCH0(uint32_t value) {
	CTSU.CTSUMCH0.BIT.CTSUMCH0 =
			value;
}
#pragma inline (HW_CTSU_CTSUMCH1SetBitCTSUMCH1)
static void HW_CTSU_CTSUMCH1SetBitCTSUMCH1(uint32_t value) {
	CTSU.CTSUMCH1.BIT.CTSUMCH1 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHAC0SetBitCTSUCHAC00)
static void HW_CTSU_CTSUCHAC0SetBitCTSUCHAC00(uint32_t value) {
	CTSU.CTSUCHAC0.BIT.CTSUCHAC00 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHAC0SetBitCTSUCHAC01)
static void HW_CTSU_CTSUCHAC0SetBitCTSUCHAC01(uint32_t value) {
	CTSU.CTSUCHAC0.BIT.CTSUCHAC01 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHAC0SetBitCTSUCHAC02)
static void HW_CTSU_CTSUCHAC0SetBitCTSUCHAC02(uint32_t value) {
	CTSU.CTSUCHAC0.BIT.CTSUCHAC02 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHAC0SetBitCTSUCHAC03)
static void HW_CTSU_CTSUCHAC0SetBitCTSUCHAC03(uint32_t value) {
	CTSU.CTSUCHAC0.BIT.CTSUCHAC03 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHAC0SetBitCTSUCHAC04)
static void HW_CTSU_CTSUCHAC0SetBitCTSUCHAC04(uint32_t value) {
	CTSU.CTSUCHAC0.BIT.CTSUCHAC04 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHAC0SetBitCTSUCHAC05)
static void HW_CTSU_CTSUCHAC0SetBitCTSUCHAC05(uint32_t value) {
	CTSU.CTSUCHAC0.BIT.CTSUCHAC05 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHAC0SetBitCTSUCHAC06)
static void HW_CTSU_CTSUCHAC0SetBitCTSUCHAC06(uint32_t value) {
	CTSU.CTSUCHAC0.BIT.CTSUCHAC06 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHAC0SetBitCTSUCHAC07)
static void HW_CTSU_CTSUCHAC0SetBitCTSUCHAC07(uint32_t value) {
	CTSU.CTSUCHAC0.BIT.CTSUCHAC07 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHAC1SetBitCTSUCHAC10)
static void HW_CTSU_CTSUCHAC1SetBitCTSUCHAC10(uint32_t value) {
	CTSU.CTSUCHAC1.BIT.CTSUCHAC10 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHAC1SetBitCTSUCHAC11)
static void HW_CTSU_CTSUCHAC1SetBitCTSUCHAC11(uint32_t value) {
	CTSU.CTSUCHAC1.BIT.CTSUCHAC11 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHAC1SetBitCTSUCHAC12)
static void HW_CTSU_CTSUCHAC1SetBitCTSUCHAC12(uint32_t value) {
	CTSU.CTSUCHAC1.BIT.CTSUCHAC12 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHAC1SetBitCTSUCHAC13)
static void HW_CTSU_CTSUCHAC1SetBitCTSUCHAC13(uint32_t value) {
	CTSU.CTSUCHAC1.BIT.CTSUCHAC13 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC00)
static void HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC00(uint32_t value) {
	CTSU.CTSUCHTRC0.BIT.CTSUCHTRC00 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC01)
static void HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC01(uint32_t value) {
	CTSU.CTSUCHTRC0.BIT.CTSUCHTRC01 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC02)
static void HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC02(uint32_t value) {
	CTSU.CTSUCHTRC0.BIT.CTSUCHTRC02 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC03)
static void HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC03(uint32_t value) {
	CTSU.CTSUCHTRC0.BIT.CTSUCHTRC03 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC04)
static void HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC04(uint32_t value) {
	CTSU.CTSUCHTRC0.BIT.CTSUCHTRC04 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC05)
static void HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC05(uint32_t value) {
	CTSU.CTSUCHTRC0.BIT.CTSUCHTRC05 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC06)
static void HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC06(uint32_t value) {
	CTSU.CTSUCHTRC0.BIT.CTSUCHTRC06 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC07)
static void HW_CTSU_CTSUCHTRC0SetBitCTSUCHTRC07(uint32_t value) {
	CTSU.CTSUCHTRC0.BIT.CTSUCHTRC07 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHTRC1SetBitCTSUCHTRC10)
static void HW_CTSU_CTSUCHTRC1SetBitCTSUCHTRC10(uint32_t value) {
	CTSU.CTSUCHTRC1.BIT.CTSUCHTRC10 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHTRC1SetBitCTSUCHTRC11)
static void HW_CTSU_CTSUCHTRC1SetBitCTSUCHTRC11(uint32_t value) {
	CTSU.CTSUCHTRC1.BIT.CTSUCHTRC11 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHTRC1SetBitCTSUCHTRC12)
static void HW_CTSU_CTSUCHTRC1SetBitCTSUCHTRC12(uint32_t value) {
	CTSU.CTSUCHTRC1.BIT.CTSUCHTRC12 =
			value;
}
#pragma inline (HW_CTSU_CTSUCHTRC1SetBitCTSUCHTRC13)
static void HW_CTSU_CTSUCHTRC1SetBitCTSUCHTRC13(uint32_t value) {
	CTSU.CTSUCHTRC1.BIT.CTSUCHTRC13 =
			value;
}
#pragma inline (HW_CTSU_CTSUDCLKCSetBitCTSUSSMOD)
static void HW_CTSU_CTSUDCLKCSetBitCTSUSSMOD(uint32_t value) {
	CTSU.CTSUDCLKC.BIT.CTSUSSMOD =
			value;
}
#pragma inline (HW_CTSU_CTSUDCLKCSetBitCTSUSSCNT)
static void HW_CTSU_CTSUDCLKCSetBitCTSUSSCNT(uint32_t value) {
	CTSU.CTSUDCLKC.BIT.CTSUSSCNT =
			value;
}
#pragma inline (HW_CTSU_CTSUSTSetBitCTSUSTC)
static void HW_CTSU_CTSUSTSetBitCTSUSTC(uint32_t value) {
	CTSU.CTSUST.BIT.CTSUSTC =
			value;
}
#pragma inline (HW_CTSU_CTSUSTSetBitCTSUDTSR)
static void HW_CTSU_CTSUSTSetBitCTSUDTSR(uint32_t value) {
	CTSU.CTSUST.BIT.CTSUDTSR =
			value;
}
#pragma inline (HW_CTSU_CTSUSTSetBitCTSUSOVF)
static void HW_CTSU_CTSUSTSetBitCTSUSOVF(uint32_t value) {
	CTSU.CTSUST.BIT.CTSUSOVF =
			value;
}
#pragma inline (HW_CTSU_CTSUSTSetBitCTSUROVF)
static void HW_CTSU_CTSUSTSetBitCTSUROVF(uint32_t value) {
	CTSU.CTSUST.BIT.CTSUROVF =
			value;
}
#pragma inline (HW_CTSU_CTSUSTSetBitCTSUPS)
static void HW_CTSU_CTSUSTSetBitCTSUPS(uint32_t value) {
	CTSU.CTSUST.BIT.CTSUPS =
			value;
}
#pragma inline (HW_CTSU_CTSUSSCSetBitCTSUSSDIV)
static void HW_CTSU_CTSUSSCSetBitCTSUSSDIV(uint32_t value) {
	CTSU.CTSUSSC.BIT.CTSUSSDIV =
			value;
}
#pragma inline (HW_CTSU_CTSUSO0SetBitCTSUSO)
static void HW_CTSU_CTSUSO0SetBitCTSUSO(uint32_t value) {
	CTSU.CTSUSO0.BIT.CTSUSO =
			value;
}
#pragma inline (HW_CTSU_CTSUSO0SetBitCTSUSNUM)
static void HW_CTSU_CTSUSO0SetBitCTSUSNUM(uint32_t value) {
	CTSU.CTSUSO0.BIT.CTSUSNUM =
			value;
}
#pragma inline (HW_CTSU_CTSUSO1SetBitCTSURICOA)
static void HW_CTSU_CTSUSO1SetBitCTSURICOA(uint32_t value) {
	CTSU.CTSUSO1.BIT.CTSURICOA =
			value;
}
#pragma inline (HW_CTSU_CTSUSO1SetBitCTSUSDPA)
static void HW_CTSU_CTSUSO1SetBitCTSUSDPA(uint32_t value) {
	CTSU.CTSUSO1.BIT.CTSUSDPA =
			value;
}
#pragma inline (HW_CTSU_CTSUSO1SetBitCTSUICOG)
static void HW_CTSU_CTSUSO1SetBitCTSUICOG(uint32_t value) {
	CTSU.CTSUSO1.BIT.CTSUICOG =
			value;
}
#pragma inline (HW_CTSU_CTSUERRSSetBitCTSUICOMP)
static void HW_CTSU_CTSUERRSSetBitCTSUICOMP(uint32_t value) {
	CTSU.CTSUERRS.BIT.CTSUICOMP =
			value;
}
#endif /* R_CTSU_SFR_ACCESS_RX_H_ */
/*******************************************************************************************************************//**
 * @} (end defgroup r_ctsu)
 ***********************************************************************************************************************/
