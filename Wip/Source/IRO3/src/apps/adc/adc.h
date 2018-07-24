
/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         Adc.h
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

#ifndef ADC_H_
#define ADC_H_

/******************************************************************************
* Includes
******************************************************************************/


#include <config.h>
#include "r_cg_s12ad.h"
#include <queue.h>


/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros 
******************************************************************************/

#define  ADC_SAMPLE_QUEUE_SIZE			(50)

/******************************************************************************
* Types
******************************************************************************/

/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *
 * Detailed explanation.
 */
	
typedef enum
{
	TDS_OUT = ADCHANNEL0,
	TDS_IN = ADCHANNEL1,
	ADC_MAX
	
}TDS_ID_E;


/******************************************************************************
* Global variables
******************************************************************************/
  

/******************************************************************************
* Global functions
******************************************************************************/
PUBLIC void	ADC_Init(void);

PUBLIC ERR_E  ADC_Read(ad_channel_t channel,uint16_t* adc_result);

PUBLIC ERR_E  ADC_ReadTds(ad_channel_t channel);

PUBLIC uint16_t  ADC_GetAdcLowValue();

PUBLIC uint16_t  ADC_GetAdcTdsValue();

//PUBLIC uint16_t  ADC_GetTdsValue(uint16_t adc0_value);
PUBLIC uint16_t  ADC_GetTdsValue();
/******************************************************************************
* Inline functions
******************************************************************************/



#endif 


