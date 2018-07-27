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
void flash_app_init()
{
    uint32_t    addr;
    flash_err_t err;
    uint8_t data;

    /* Open driver */
    err = R_FLASH_Open();
    if (err != FLASH_SUCCESS)
        while(1);


    /* DATA FLASH */
    /* Erase highest data block */


    /* Write to all of block 0 */
    addr = FLASH_DF_BLOCK_0;
    data = *((uint8_t *)(addr + 0));
    data++;
    flash_app_writeBlock(&data, FLASH_DF_BLOCK_0, 1);



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
