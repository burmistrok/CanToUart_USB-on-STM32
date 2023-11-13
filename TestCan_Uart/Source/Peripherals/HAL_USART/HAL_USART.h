/*
 * HAL_USART.h
 *
 *  Created on: Apr 14, 2020
 *      Author: ok
 */

#ifndef PERIPHERALS_HAL_USART_HAL_USART_H_
#define PERIPHERALS_HAL_USART_HAL_USART_H_


#include "Std_types.h"
#include <stdio.h>


void vUSART_Init			(const void* configPtr());
void vUSART_DeInit			(void);
void vUSART_MainFunction	(void);

void vUSART_ITCallBack(void);


TE_ERROR HAL_USART_GetChar(TS_USART* USARTx, uint8_t* Data);

TE_ERROR HAL_USART_SendChar(TS_USART* USARTx, uint8_t Data);

TE_ERROR HAL_USART_SendBuffer(TS_USART* USARTx, uint8_t* Data, uint16_t Len, uint16_t* RecievedLen);

TE_ERROR HAL_USART_ReceiveBuffer(TS_USART* USARTx, uint8_t* Data, uint16_t Len, uint16_t* RecievedLen);


#endif /* PERIPHERALS_HAL_USART_HAL_USART_H_ */
