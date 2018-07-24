/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        temp.c
*
* @author    trongkn
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


#include <config.h>
#include <gpio.h>


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
PUBLIC void GPIO_Init(void)
{
	   R_GPIO_PinWrite(GPIO_PORT_C_PIN_6,GPIO_LEVEL_HIGH);
	   R_GPIO_PinWrite(GPIO_PORT_C_PIN_7,GPIO_LEVEL_LOW);
	   R_GPIO_PinWrite(GPIO_PORT_C_PIN_3,GPIO_LEVEL_HIGH);
	   R_GPIO_PinWrite(GPIO_PORT_C_PIN_5,GPIO_LEVEL_LOW);
	   R_GPIO_PinWrite(GPIO_PORT_5_PIN_4,GPIO_LEVEL_HIGH);
	   R_GPIO_PinWrite(GPIO_PORT_5_PIN_5,GPIO_LEVEL_LOW);
	   R_GPIO_PinWrite(GPIO_PORT_B_PIN_6,GPIO_LEVEL_HIGH);
	   R_GPIO_PinWrite(GPIO_PORT_C_PIN_2,GPIO_LEVEL_LOW);
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
