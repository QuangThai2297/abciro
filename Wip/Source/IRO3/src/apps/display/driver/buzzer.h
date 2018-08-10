
/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         buzzer.h
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

#ifndef APPS_DISPLAY_BUZZER_H_
#define APPS_DISPLAY_BUZZER_H_


/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/



/******************************************************************************
* Types
******************************************************************************/





/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
void Buzzer_process();

void Buzzer_onInMs(uint16_t msTime);
void Buzzer_blink(uint8_t time);
void Buzzer_blinkError();


/******************************************************************************
* Inline functions
******************************************************************************/






#endif /* APPS_DISPLAY_BUZZER_H_ */