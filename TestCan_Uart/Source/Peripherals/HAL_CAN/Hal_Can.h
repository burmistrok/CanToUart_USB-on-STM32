/*
 * Hal_Can.h
 *
 *  Created on: Nov 12, 2023
 *      Author: MO
 */

#ifndef PERIPHERALS_HAL_CAN_HAL_CAN_H_
#define PERIPHERALS_HAL_CAN_HAL_CAN_H_



#define USE_TX_CONFIRMATIN



#define MAX_STD_DLC_LENGHT	(8u)

typedef struct
{
	uint32_t u32CanFrameId;
	uint8_t u8_DLC;
	uint8_t RTR;
	uint8_t IDE;
	uint8_t CANID;
	uint8_t ua8_Data[MAX_STD_DLC_LENGHT];
}TS_CanFrame;

void vCan_Init(void);

void Hal_Can_Transmit( TS_CanFrame *lptr_FrameInfo);

void vCan_MainFunction(void);

#endif /* PERIPHERALS_HAL_CAN_HAL_CAN_H_ */
