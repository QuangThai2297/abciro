/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        user_config.c
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


#include "user_config.h"
#include "flash_app.h"
#include <string.h>


/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/
UserConfig_t g_userConfig;
/******************************************************************************
* Constants and macros
******************************************************************************/
static const UserConfig_t USER_CONFIG_DEFAULD = {
		.filterLifeTime = {1080000,1080000,1080000,12960000,3600000,3600000,3600000,3600000,3600000},
		.tdsLimitIn = 1000,
		.tdsLimitOut = 100,
		.tdsToTimeMode = false
};

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


void user_config_updateToFlash()
{
	flash_app_writeBlock((uint8_t *)&g_userConfig, USER_CONFIG_BLOCK, sizeof(g_userConfig));
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
void user_config_init()
{
    bool readOk = flash_app_readData((uint8_t*)&g_userConfig,USER_CONFIG_BLOCK,sizeof(g_userConfig));

    if(!readOk)
    {
    	memcpy(&g_userConfig,&USER_CONFIG_DEFAULD,sizeof(g_userConfig));
    	user_config_updateToFlash();
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
