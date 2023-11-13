/*
 * Std_Types.h
 *
 *  Created on: Apr 13, 2020
 *      Author: ok
 */

#ifndef COMMON_STD_TYPES_H_
#define COMMON_STD_TYPES_H_

#include "stdbool.h"
#include "Utils.h"
#include "main.h"
#include "stdint.h"
#include "Modules.h"
#include "string.h"

#define NULL_PTR	((void*)0)




typedef unsigned char 		uint8_t;
typedef signed char 		int8_t;
typedef unsigned short 		uint16_t;
typedef signed short 		int16_t;
typedef unsigned long 		uint32_t;
typedef signed long 		int32_t;
typedef unsigned long long 	uint64_t;
typedef signed long long 	int64_t;
typedef USART_TypeDef TS_USART;
typedef SPI_TypeDef TS_SPI;

typedef enum
{
	ERR_OK = 0u,
	ERR_NOK,
	ERR_BUFFER_FULL,
	ERR_BUFFER_EMPTY,
	ERR_TIMEOUT,


}TE_ERROR;


#endif /* COMMON_STD_TYPES_H_ */
