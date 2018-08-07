/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        display.c
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


#include <config.h>
#include <display.h>
#include "r_gpio_rx_if.h"
#include "adc.h"
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
#define PIN_KEY_NUM 4
#define LED_MACHINE_STATE_TDS_OUT GPIO_PORT_E_PIN_0
#define LED_MACHINE_STATE_TDS_IN  GPIO_PORT_4_PIN_7
#define LED_MACHINE_STATE_FILTER GPIO_PORT_E_PIN_1
#define LED_MACHINE_STATE_NUM 3

const uint8_t LED7_CODE[] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff};
const gpio_port_pin_t LED7_PIN[] = {GPIO_PORT_A_PIN_3,GPIO_PORT_A_PIN_0,GPIO_PORT_B_PIN_3,GPIO_PORT_A_PIN_6,GPIO_PORT_A_PIN_4,GPIO_PORT_A_PIN_1,GPIO_PORT_B_PIN_5,GPIO_PORT_B_PIN_1};
const gpio_port_pin_t LED7_DIGITS[] = {GPIO_PORT_B_PIN_7,GPIO_PORT_4_PIN_4,GPIO_PORT_4_PIN_3,GPIO_PORT_4_PIN_2,GPIO_PORT_E_PIN_2};
const gpio_port_pin_t LED_KEY_RED_PIN[] = {GPIO_PORT_B_PIN_6,GPIO_PORT_C_PIN_3,GPIO_PORT_C_PIN_6,GPIO_PORT_5_PIN_4};
const gpio_port_pin_t LED_KEY_GREEN_PIN[] = {GPIO_PORT_C_PIN_2,GPIO_PORT_C_PIN_5,GPIO_PORT_C_PIN_7,GPIO_PORT_5_PIN_5};
const gpio_port_pin_t LED_MACHINE_STATE_PIN[LED_MACHINE_STATE_NUM] = {LED_MACHINE_STATE_TDS_OUT,LED_MACHINE_STATE_TDS_IN,LED_MACHINE_STATE_FILTER};
#define BUZZER_PIN GPIO_PORT_1_PIN_7

/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/
void Display_showDigitAtIndex(uint8_t digit,uint8_t index);
void processBuzzer();
void encodeLed4();


/******************************************************************************
* Local variables
******************************************************************************/
uint8_t led4Digits[4] = {0xff,0xff,0xff,0xff};
uint16_t s_led4Number;
uint8_t led1Code = 0;
uint8_t ledIndex;
uint32_t timeOffBuzzer;
bool buzzerIsOn = false;

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
void processBuzzer()
{
	if(buzzerIsOn && ((g_sysTime - timeOffBuzzer) < 5000) )
	{
		R_GPIO_PinWrite(BUZZER_PIN, GPIO_LEVEL_LOW);
		buzzerIsOn = false;
	}
}
void encodeLed4()
{
	uint16_t number = s_led4Number;
	for(int i = 0; i<4; i++)
	{
		led4Digits[i] = LED7_CODE[number%10];
		number /= 10;
	}
	if(led4Digits[3] == LED7_CODE[0])
	{
		led4Digits[3] = LED7_CODE[LED_7SEG_OFF];
		if(led4Digits[2] == LED7_CODE[0])
		{
			led4Digits[2] = LED7_CODE[LED_7SEG_OFF];
			if(led4Digits[1] == LED7_CODE[0])
			{
				led4Digits[1] = LED7_CODE[LED_7SEG_OFF];
			}
		}
	}
}
/******************************************************************************
* Global functions
******************************************************************************/
void Display_process()
{
	processBuzzer();
	Display_scanLed();
}
void Display_onBuzzerInMs(uint16_t msTime)
{
	timeOffBuzzer = g_sysTime + msTime;
	R_GPIO_PinWrite(BUZZER_PIN, GPIO_LEVEL_HIGH);
	buzzerIsOn = true;
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
void Display_showDigitAtIndex(uint8_t digit,uint8_t index)
{
	for(int i = 0; i<5; i++)
	{
		R_GPIO_PinWrite(LED7_DIGITS[i],GPIO_LEVEL_HIGH);
	}
	for(int i = 0; i<7;i++)
	{
		R_GPIO_PinWrite(LED7_PIN[i],(digit & (1<<i)));
	}
	R_GPIO_PinWrite(LED7_DIGITS[index],GPIO_LEVEL_LOW);
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
void Display_scanLed(void)
{
	uint8_t digit = 0;
	if(ledIndex < 4)
	{
		Display_showDigitAtIndex(led4Digits[ledIndex],ledIndex);
	}else{
		digit = led1Code;
		Display_showDigitAtIndex(digit,ledIndex);
	}

	if(ledIndex++ == 4)
	{
		ledIndex = 0;
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
void Display_SetNumberInLed1(int8_t number)
{
	led1Code = LED7_CODE[number];
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
void Display_SetNumberInLed4(uint16_t number)
{
	s_led4Number = number;
	encodeLed4();
}

uint16_t Display_getNumberInLed4()
{
	return s_led4Number;
}

void Display_reduceNumberInLed4(uint16_t reduce)
{
	s_led4Number -= reduce;
	encodeLed4();
}

void Display_increaseNumberInLed4(uint16_t reduce)
{
	s_led4Number += reduce;
	encodeLed4();
}

void Display_SetLedKeyState(LedKeyName_t ledName,LedKeyColor_t color, LedState_t state)
{
	if(color == LED_KEY_COLLOR_GREEN)
	{
		R_GPIO_PinWrite(LED_KEY_GREEN_PIN[ledName],state == LED_STATE_ON? GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH);
	}else
	{
		R_GPIO_PinWrite(LED_KEY_RED_PIN[ledName],state == LED_STATE_ON? GPIO_LEVEL_LOW:GPIO_LEVEL_HIGH);
	}
}

void Display_turnOnLedKey()
{
	for(uint8_t i = 0; i< PIN_KEY_NUM; i++)
	{
		R_GPIO_PinWrite(LED_KEY_GREEN_PIN[i],GPIO_LEVEL_LOW);
	}
}
void Display_turnOffLedKey()
{
	for(uint8_t i = 0; i< PIN_KEY_NUM; i++)
	{
		R_GPIO_PinWrite(LED_KEY_GREEN_PIN[i],GPIO_LEVEL_HIGH);
	}
}

void Display_switchMachineStateLed(MachineStateLed_t machineState)
{
	uint8_t i;
	for ( i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		R_GPIO_PinWrite(LED_MACHINE_STATE_PIN[i], GPIO_LEVEL_HIGH);
	}
	R_GPIO_PinWrite(LED_MACHINE_STATE_PIN[machineState], GPIO_LEVEL_LOW);
}
void Display_showFilterTime(uint8_t filter)
{
	Display_switchMachineStateLed(MACHINE_STATE_LED_FILTER);
	Display_SetNumberInLed4(filter_time_getFilterHour(filter));
	Display_SetNumberInLed1(filter +1);
}
extern volatile uint8_t g_run200usFlag;

void Display_turnOnAllIn1s()
{
	Display_turnOnLedKey();
	for (uint8_t i = 0;  i < LED_MACHINE_STATE_NUM; ++ i) {
		R_GPIO_PinWrite(LED_MACHINE_STATE_PIN[i], GPIO_LEVEL_LOW);
	}
	Display_SetNumberInLed1(8);
	Display_SetNumberInLed4(8888);
	R_GPIO_PinWrite(BUZZER_PIN, GPIO_LEVEL_HIGH);
	uint32_t timeStart = g_sysTime;
	while((g_sysTime - timeStart) < 1500)
	{
    	if(g_run200usFlag == 1)
    	{
    		Display_scanLed();
    		g_run200usFlag= 0;
    	}
	}
	R_GPIO_PinWrite(BUZZER_PIN, GPIO_LEVEL_LOW);

}

void Display_showTdsOut()
{
	Display_SetNumberInLed1(LED_7SEG_OFF);
	Display_SetNumberInLed4(ADC_GetTdsValueDisplay(TDS_OUT_VALUE));
	Display_switchMachineStateLed(MACHINE_STATE_LED_TDS_OUT);
}
void Display_showTdsIn()
{
	Display_SetNumberInLed1(LED_7SEG_OFF);
	Display_SetNumberInLed4(ADC_GetTdsValueDisplay(TDS_IN_VALUE));
	Display_switchMachineStateLed(MACHINE_STATE_LED_TDS_IN);
}
