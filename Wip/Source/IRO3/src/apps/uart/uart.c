/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        uart_drv.c
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


#include <uart.h>

/******************************************************************************
* External objects
******************************************************************************/

///* Global variable for changing CMT0 interval */
//volatile uint16_t interval_level = 1;
///* String used to print message at PC terminal */
//static char print_str[250];
///* Flag used to detect whether data is received from PC terminal */
//extern volatile uint8_t g_rx_flag;
///* Global variable used for storing data received from PC terminal */
//extern volatile uint8_t g_rx_char;
///* Sends SCI2 data and waits for transmit end flag. */


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
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
PUBLIC void UART_Init(void)
{
//	//init UART here
//	/* Set SCI2 receive buffer address and enable receive interrupt */
//	R_Config_SCI1_Serial_Receive((uint8_t *)&g_rx_char, 1);
//	/* Enable SCI2 operation */
//	R_Config_SCI1_Start();
//
//	sprintf(print_str, "UART init done\r\n");
//	R_SCI1_AsyncTransmit((uint8_t *)print_str, (uint16_t)strlen(print_str));
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
PUBLIC void UART_UartPuts (uint8_t *s)
{
//	sprintf(print_str, s);
//	R_SCI1_AsyncTransmit((uint8_t *)print_str, (uint16_t)strlen(print_str));
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
PUBLIC void UART_Debug(uint8_t * data)
{  
	#ifdef DEBUG_ENABLE
	 UART_UartPuts(data);
	#endif
}


