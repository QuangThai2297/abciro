/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        flash_app.c
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


#include "flash_app.h"
#include "user_config.h"
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


/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/



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


/******************************************************************************
* Global functions
******************************************************************************/

void flash_app_eraseAll()
{
	flash_err_t err;
	flash_res_t result;
    /* Erase all of data flash */
    err = R_FLASH_Erase(FLASH_DF_BLOCK_0, FLASH_NUM_BLOCKS_DF);
    if (err != FLASH_SUCCESS)
    {
        while(1) ;
    }

    /* Verify erased */
    err = R_FLASH_BlankCheck(FLASH_DF_BLOCK_0, FLASH_DF_FULL_SIZE, &result);
    if ((err != FLASH_SUCCESS) || (result != FLASH_RES_BLANK))
    {
        while(1) ;
    }
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
void flash_app_init()
{
    flash_err_t err;

    /* Open driver */
    err = R_FLASH_Open();
    if (err != FLASH_SUCCESS)
        while(1);
//    flash_app_eraseAll();
    user_config_init();
    filter_time_init();


    //    filter_time_eraseBlock(DEFAULD_BLOCK);

}

void flash_app_eraseBlock(flash_block_address_t blockAdress)
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

void flash_app_writeBlock(uint8_t * data, flash_block_address_t blockAdress,uint16_t dataSize)
{
    uint32_t    addr, i;
    flash_err_t err;

    flash_app_eraseBlock(blockAdress);
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

bool flash_app_readData(uint8* dataRead, flash_block_address_t blockAdress,uint16_t dataSize)
{
    flash_err_t err;
    flash_res_t result;

    err = R_FLASH_BlankCheck(blockAdress, FLASH_DF_BLOCK_SIZE, &result);
    if (err != FLASH_SUCCESS)
        while(1) ;
    if(result == FLASH_RES_BLANK)
    	return false;

    for (int i=0; i < dataSize; i ++)
    {
        *(dataRead + i) = *((uint8_t *)(blockAdress + i));
    }
    return true;
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

