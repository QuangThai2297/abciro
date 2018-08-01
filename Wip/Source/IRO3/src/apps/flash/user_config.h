#ifndef APPS_FLASH_USER_CONFIG_H_
#define APPS_FLASH_USER_CONFIG_H_


/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         user_config.h
 *
 * @author    	quanvu
 *
 * @version   1.0
 *
 * @date
 *
 * @brief     Brief description of the file
 *
 * Detailed Description of the file. If not used, remove the separator above.
 *
 */


/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>
#include "r_flash_rx_if.h"
#include "r_flash_rx_config.h"
/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/
#define FILTER_NUM 9



/******************************************************************************
* Types
******************************************************************************/
typedef struct UserConfigs {
	uint32_t filterLifeTime[9];	// tuổi thọ lõi lọc tính theo giây
	uint16_t tdsLimitIn;		// giới hạn tds đầu vào
	uint16_t tdsLimitOut;		// giới hạn tds đầu ra
	bool tdsToTimeMode;		// chế độ thời gian lõi lọc theo tds
} UserConfig_t;

/******************************************************************************
* Global variables
******************************************************************************/
extern UserConfig_t g_userConfig;

/******************************************************************************
* Global functions
******************************************************************************/
void user_config_init();
void UserConfig_setFilterLifeTime(uint32_t lifeTime,uint8_t filterIndex);


/******************************************************************************
* Inline functions
******************************************************************************/



#endif /* APPS_FLASH_USER_CONFIG_H_ */
