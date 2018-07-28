/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        UIControl.c
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
#include "UIControl.h"


/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define HOLD_TIME1 2000
#define HOLD_TIME2 3000
#define HOLD_TIME3 7000
/******************************************************************************
* Local types
******************************************************************************/
typedef enum UI_State
{
	UI_STATE_LOCK = 0,
	UI_STATE_TDS_OUT,
	UI_STATE_TDS_IN,
	UI_STATE_FILTER_1,
	UI_STATE_FILTER_2,
	UI_STATE_FILTER_3,
	UI_STATE_FILTER_4,
	UI_STATE_FILTER_5,
	UI_STATE_FILTER_6,
	UI_STATE_FILTER_7,
	UI_STATE_FILTER_8,
	UI_STATE_FILTER_9,
	UI_STATE_COUNT
}UI_State_t;

typedef enum
{
	BTN_STATE_RELEASE = 0,
	BTN_STATE_PRESS,
	BTN_STATE_HOLD1,
	BTN_STATE_HOLD2,
	BTN_STATE_HOLD3,
}BtnState_t;
typedef struct
{
	BtnState_t state;
	uint32_t timePress;
}BtnControl_t;
/******************************************************************************
* Local function prototypes
******************************************************************************/
void UIControl_btnProcess();
void UIControl_btnHold_cb(ButtonId_t btn,uint32_t holdingTime);


/******************************************************************************
* Local variables
******************************************************************************/
UI_State_t s_UIState = UI_STATE_LOCK;
BtnControl_t btnControls[BUTTON_NUM];
void UIControl_switchUiStateTo(UI_State_t newState);

/******************************************************************************
* Local functions
******************************************************************************/
void UIControl_btnHold_cb(ButtonId_t btn,uint32_t holdingTime)
{
	if((s_UIState == UI_STATE_LOCK) && (btn == BUTTON_ID_SET) && (holdingTime == HOLD_TIME2))
	{
		UIControl_switchUiStateTo(UI_STATE_TDS_OUT);
	}
}

void UIControl_switchUiStateTo(UI_State_t newState)
{
	if(s_UIState == UI_STATE_LOCK)
	{
		Display_turnOnLedKey();
	}
	switch (newState) {
		case UI_STATE_LOCK:
			Display_turnOffLedKey();
			break;
		default:
			break;
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
void UIControl_btnProcess()
{
	for(uint8_t btnId = 0; btnId< BUTTON_NUM; btnId++)
	{
		if(btnControls[btnId].state != BTN_STATE_RELEASE)
		{
			if((g_sysTime - btnControls[btnId].timePress) > HOLD_TIME1)
			{
				if(btnControls[btnId].state == BTN_STATE_PRESS)
				{
					btnControls[btnId].state = BTN_STATE_HOLD1;
					UIControl_btnHold_cb(btnId,HOLD_TIME1);
				}
				if((g_sysTime - btnControls[btnId].timePress) > HOLD_TIME2)
				{
					if(btnControls[btnId].state == BTN_STATE_HOLD1)
					{
						btnControls[btnId].state = BTN_STATE_HOLD2;
						UIControl_btnHold_cb(btnId,HOLD_TIME2);
					}
					if((g_sysTime - btnControls[btnId].timePress) > HOLD_TIME3)
					{
						if(btnControls[btnId].state == BTN_STATE_HOLD2)
						{
							btnControls[btnId].state = BTN_STATE_HOLD3;
							UIControl_btnHold_cb(btnId,HOLD_TIME3);
						}
					}
				}

			}
		}
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
bool UIControl_stateIsLock()
{
	return (s_UIState == UI_STATE_LOCK?true:false);
}

void UIControl_process()
{
	UIControl_btnProcess();
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


void TouchBtnPressed_cb(ButtonId_t btn)
{
	btnControls[btn].state = BTN_STATE_PRESS;
	btnControls[btn].timePress = g_sysTime;
}


void TouchBtnHoldRelease_cb(ButtonId_t btn)
{
	btnControls[btn].state = BTN_STATE_RELEASE;
}


