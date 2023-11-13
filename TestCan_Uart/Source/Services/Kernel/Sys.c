/*
 * Sys.c
 *
 *  Created on: 13 апр. 2020 г.
 *      Author: ok
 */


#include "Sys.h"

static bool bSysInit = false;
volatile static uint32_t u32_RunTime;


/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vSys_Init(const void* configPtr)
{

	bSysInit = true;
	(void)configPtr;
}

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vSys_DeInit(void)
{

	bSysInit = false;
}

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
uint32_t u32Sys_GetRunTime(void)
{
	return u32_RunTime;
}

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
volatile void vSysRunModeTick(void)
{

	u32_RunTime++;

}
