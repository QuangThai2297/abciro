/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        filter_time.c
*
* @author    quanvu
*
* @version   1.0
*
* @date
*
* @brief
*
*******************************************************************************
*
* Detailed Description of the file. If not used, remove the separator above.
*
******************************************************************************/


#include "filter_time.h"


/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define FILTER_NUM 10 // 9 filter and 1 is special sign
#define DEFAULD_BLOCK FLASH_DF_BLOCK_0

static const uint32_t FILTER_TIME_DEFAULD[FILTER_NUM] = {26121803,1080000,1080000,1080000,12960000,3600000,3600000,3600000,3600000,3600000};
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
static uint32_t s_time_filter[FILTER_NUM];



/******************************************************************************
* Local functions
******************************************************************************/
/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
void filter_time_eraseBlock(flash_block_address_t blockAdress)
{
	flash_err_t err;
	flash_res_t result;
    err = R_FLASH_Erase(blockAdress, 1);
    if (err != FLASH_SUCCESS)
    {
        while(1) ;
    }

    /* Verify erased */
    err = R_FLASH_BlankCheck(blockAdress, FLASH_DF_BLOCK_SIZE, &result);
    if ((err != FLASH_SUCCESS) || (result != FLASH_RES_BLANK))
    {
        while(1) ;
    }
}

void filter_time_writeBlock(uint8_t * data, flash_block_address_t blockAdress,uint16_t dataSize)
{
    uint32_t    addr, i;
    flash_err_t err;

	filter_time_eraseBlock(blockAdress);
    addr = blockAdress;
    err = R_FLASH_Write((uint32_t)data, addr, FLASH_DF_BLOCK_SIZE);
    if(err != FLASH_SUCCESS)
    {
        while(1) ;
    }

    /* Verify data write */
    for (i=0; i < dataSize; i++)
    {
        if (*(data +i) != *((uint8_t *)(addr + i)))
            while(1);
    }
}

void filter_time_updateToFlash()
{
	filter_time_writeBlock((uint8_t *)s_time_filter, DEFAULD_BLOCK, sizeof(s_time_filter));
}

/******************************************************************************
* Global functions
******************************************************************************/

/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
void filter_time_init()
{
    uint32_t    addr,i;
    flash_err_t err;

    /* Open driver */
    err = R_FLASH_Open();
    if (err != FLASH_SUCCESS)
        while(1);

    addr = DEFAULD_BLOCK;
    for (i=0; i < (sizeof(s_time_filter) >> 2); i ++)
    {
        *(s_time_filter + i) = *((uint32_t *)(addr + (i << 2)));
    }
    if(s_time_filter[0] != FILTER_TIME_DEFAULD[0])
    {
    	filter_time_writeBlock((uint8_t *)FILTER_TIME_DEFAULD, DEFAULD_BLOCK, sizeof(FILTER_TIME_DEFAULD));
    }
//    filter_time_eraseBlock(DEFAULD_BLOCK);

}

void filter_time_minusTime(uint32_t second)
{
	for(uint8_t i = 1; i< FILTER_NUM; i++)
	{
		s_time_filter[i] -= second;
	}
	filter_time_updateToFlash();
}

uint16_t filter_time_getFilterHour(uint8_t filIndex)
{
	return (uint16_t)(s_time_filter[filIndex]/3600);
}
/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
