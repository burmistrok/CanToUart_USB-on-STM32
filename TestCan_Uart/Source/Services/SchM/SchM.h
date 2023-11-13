/*
 * SchM.h
 *
 *  Created on: Apr 13, 2020
 *      Author: ok
 */

#ifndef SERVICES_SCHM_SCHM_H_
#define SERVICES_SCHM_SCHM_H_

#include "Std_types.h"

#define SCHM_BASE_DIV_1
#define SCHM_BASE_DIV_2		0x01u
#define SCHM_BASE_DIV_4		0x02u
#define SCHM_BASE_DIV_8		0x04u
#define SCHM_BASE_DIV_16	0x08u
#define SCHM_BASE_DIV_32	0x10u



void vSchM_Init			(const void* configPtr);
void vSchM_DeInit		(void);
void vSchM_MainFunction	(void);



extern void vSchM_IdleTask					(void);
extern void vSchM_BaseRecurrenceTask		(void);
extern void vSchM_BaseRecurrenceTaskDiv2	(void);
extern void vSchM_BaseRecurrenceTaskDiv4	(void);
extern void vSchM_BaseRecurrenceTaskDiv8	(void);
extern void vSchM_BaseRecurrenceTaskDiv16	(void);
extern void vSchM_BaseRecurrenceTaskDiv32	(void);

#endif /* SERVICES_SCHM_SCHM_H_ */
