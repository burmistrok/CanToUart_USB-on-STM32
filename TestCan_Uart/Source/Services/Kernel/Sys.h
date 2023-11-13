/*
 * Sys.h
 *
 *  Created on: 13 апр. 2020 г.
 *      Author: ok
 */

#ifndef SERVICES_KERNEL_SYS_H_
#define SERVICES_KERNEL_SYS_H_

#include "Std_types.h"



void vSys_Init(const void* configPtr);
void vSys_DeInit(void);
uint32_t u32Sys_GetRunTime(void);
volatile void vSysRunModeTick(void);

#endif /* SERVICES_KERNEL_SYS_H_ */
