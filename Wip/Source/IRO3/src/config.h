/******************************************************************************
*
* M1 Communication Inc.
* (c) Copyright 2016 M1 Communication, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         Config.h
 *
 * @author    	trongkn
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

#ifndef CONFIG_H_
#define CONFIG_H_

/******************************************************************************
* Includes
******************************************************************************/

#include "platform.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros 
******************************************************************************/

#define PUBLIC 
#define LOCAL static


#define uint8  uint8_t
#define uint16 uint16_t
#define uint32 uint32_t
#define TRUE   1
#define FALSE  0
#define PNULL  0
//#define DEBUG_ENABLE
#define MAX_TRACE_LEN				(120)
#define  ARR_SIZE( _a )  ( sizeof( (_a) ) / sizeof( (_a[0]) ) )

#define VERSION_SOFTWARE ("V1.0")


// machine specification
#define FILTER_NUM 9


/******************************************************************************
* Types
******************************************************************************/

typedef uint8_t		BOOLEAN;

typedef void*		DPARAM;				/*!< param data pointer type */

typedef enum
{
	OK,
	ERR,
	ERR_PARAM,
	ERR_TIMEOUT,
	ERR_UNKNOWN,
	NOT_SUPPORT

}ERR_E;

/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *    PE43712ds_attenuation 
 * Detailed explanation.
 */




/******************************************************************************
* Global variables
******************************************************************************/
extern volatile uint32_t g_sysTime;
   

/******************************************************************************
* Global functions
******************************************************************************/


/******************************************************************************
* Inline functions
******************************************************************************/



#endif


