/*
 * Led.c
 *
 *  Created on: 13 апр. 2020 г.
 *      Author: ok
 */

#include "Led.h"


#define TOGGLE_LED()		LL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin)
#define TIME_OUT_FOR_1S		500u


static bool bLedInit = false;
static uint16_t u16_TimeOut = TIME_OUT_FOR_1S;

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vLed_Init(const void* configPtr)
{

	bLedInit = true;
	(void)configPtr;
}

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vLed_DeInit(void)
{
	bLedInit = false;

}

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vLed_MainFunction(void)
{

	if (false == bLedInit)
	{
		return;
	}
	else
	{
		if (0 < u16_TimeOut--)
		{
			return;
		}
		else
		{
			TOGGLE_LED();
			u16_TimeOut = TIME_OUT_FOR_1S;
		}
	}

}
