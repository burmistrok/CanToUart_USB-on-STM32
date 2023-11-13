/*
 * Init.c
 *
 *  Created on: 13 апр. 2020 г.
 *      Author: ok
 */

#include "Init.h"
#include "Sys.h"
#include "SchM.h"
#include "Led.h"
#include "HAL_USART.h"
#include "HAL_CAN.h"
#include "core_cm3.h"
#include "main.h"

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vInit(void)
{

#ifdef USE_SYS
	LL_RCC_ClocksTypeDef rcc_clocks;
	LL_RCC_GetSystemClocksFreq(&rcc_clocks);
	vSys_Init((void*)SysTick_Config(rcc_clocks.SYSCLK_Frequency/1000));
#endif

#ifdef USE_LED
	vLed_Init(NULL_PTR);
#else
	MX_GPIO_Init();
#endif

#ifdef USE_USART
	vUSART_Init((void*)MX_USART1_UART_Init);
#endif

#ifdef USE_CAN
	vCan_Init();
#endif

#ifdef USE_SCHM
	vSchM_Init(NULL_PTR);
	vSchM_MainFunction();
#elif
	while (1)
		;
#endif


}
