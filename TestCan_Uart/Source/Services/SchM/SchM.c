/*
 * SchM.c
 *
 *  Created on: Apr 13, 2020
 *      Author: ok
 */

#include "SchM.h"
#include "Sys.h"

#ifdef USE_SYS
#define Sys_GetRunTime()	u32Sys_GetRunTime()
#else
#define	Sys_GetRunTime() 0u
#endif

static bool bSchmInit = false;
static uint32_t u32_ScheduleCycle_Sample;

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vSchM_Init(const void* configPtr)
{
	bSchmInit = true;

	(void)configPtr;
}

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vSchM_DeInit(void)
{

	bSchmInit = false;
}

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vSchM_MainFunction(void)
{

	while (1)
	{
		if (bSchmInit == false)
		{
			return;
		}
		else
		{
			vSchM_IdleTask();
			if (Sys_GetRunTime() ^ u32_ScheduleCycle_Sample)
			{

				u32_ScheduleCycle_Sample = Sys_GetRunTime();
				vSchM_BaseRecurrenceTask();
				if (u32_ScheduleCycle_Sample & SCHM_BASE_DIV_2)
				{
					vSchM_BaseRecurrenceTaskDiv2();
				}
				else if (u32_ScheduleCycle_Sample & SCHM_BASE_DIV_4)
				{
					vSchM_BaseRecurrenceTaskDiv4();
				}
				else if (u32_ScheduleCycle_Sample & SCHM_BASE_DIV_8)
				{
					vSchM_BaseRecurrenceTaskDiv8();
				}
				else if (u32_ScheduleCycle_Sample & SCHM_BASE_DIV_16)
				{
					vSchM_BaseRecurrenceTaskDiv16();
				}
				else if (u32_ScheduleCycle_Sample & SCHM_BASE_DIV_32)
				{
					vSchM_BaseRecurrenceTaskDiv32();
				}
				else
				{
					;
				}
			}
		}
	}
}
