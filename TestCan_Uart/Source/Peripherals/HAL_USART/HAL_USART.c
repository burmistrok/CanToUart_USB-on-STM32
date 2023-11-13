/*
 * USART.c
 *
 *  Created on: Apr 14, 2020
 *      Author: ok
 */


#include "HAL_USART.h"
#include "CircularFIFOBuffer.h"


static bool bUSARTInit = false;
static TS_CircularFIFOBuffer TX_Buffer;
static TS_CircularFIFOBuffer RX_Buffer;


static void vLL_USART_Init(void);

static void vHal_USART_PublishData(void);


/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vUSART_Init(const void* configPtr())
{

	bUSARTInit = true;
	(void)configPtr();
	vCircularFIFOBuffer_Init(&TX_Buffer);
	vCircularFIFOBuffer_Init(&RX_Buffer);

	vLL_USART_Init();
}

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vUSART_DeInit(void)
{
	bUSARTInit = false;

}

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vUSART_MainFunction(void)
{

	uint16_t u16_len;
	uint8_t tmp_Buffer[BUFFER_SIZE];

	if ( bUSARTInit == false)
	{
		return;
	}
	else
	{
		//HAL_USART_SendBuffer(USART1, (uint8_t*)&"URA\r\n", 5, &u16_len);
		HAL_USART_ReceiveBuffer(USART1, &tmp_Buffer[0], BUFFER_SIZE, &u16_len);
		if (u16_len != 0u)
		{
			HAL_USART_SendBuffer(USART1, &tmp_Buffer[0], u16_len, &u16_len);

		}

	}


}

/****************************************************************************************
 * Funtion:
 * @brief:
 * Parameters:
 * Return:
 */
void vUSART_ITCallBack(void)
{

	if( (false != LL_USART_IsActiveFlag_RXNE(USART1)) && (false != LL_USART_IsEnabledIT_RXNE(USART1)) )
	{
		bCircularFIFOBuffer_addElement(&RX_Buffer, LL_USART_ReceiveData8(USART1));
	}

	if( (false != LL_USART_IsActiveFlag_TXE(USART1)) && (false != LL_USART_IsEnabledIT_TXE(USART1)) )
	{
		vHal_USART_PublishData();
	}


	if( (false != LL_USART_IsActiveFlag_TC(USART1)) && (false != LL_USART_IsEnabledIT_TC(USART1)) )
	{
		LL_USART_DisableIT_TC(USART1);
		LL_USART_ClearFlag_TC(USART1);
	}



}

static void vLL_USART_Init(void)
{
	LL_USART_EnableIT_RXNE(USART1);
}



TE_ERROR HAL_USART_GetChar(TS_USART* USARTx, uint8_t* Data)
{
	uint8_t u8_Data = 0;
	if (false != bCircularFIFOBuffer_getElement(&RX_Buffer, (uint8_t*) &u8_Data) )
	{
		*Data = u8_Data;
		return ERR_OK;
	}
	else
	{
		return ERR_NOK;
	}


}


TE_ERROR HAL_USART_SendChar(TS_USART* USARTx, uint8_t Data)
{

	TE_ERROR eRetStatut = ERR_OK;

	if(false == bCircularFIFOBuffer_addElement(&TX_Buffer, Data) )
	{

		eRetStatut = ERR_BUFFER_FULL;
	}

	vHal_USART_PublishData();

	return eRetStatut;
}



TE_ERROR HAL_USART_SendBuffer(TS_USART* USARTx, uint8_t* Data, uint16_t Len, uint16_t* RecievedLen)
{
	uint16_t u16_Len = 0u;
	*RecievedLen = 0u;

	while (Len > u16_Len)
	{
		if (ERR_OK == HAL_USART_SendChar( USART1, *Data))
		{
			Data++;
			u16_Len++;
			(*RecievedLen)++;
		}
		else
		{
			return ERR_BUFFER_FULL;
		}
	}

	return ERR_OK;

}

TE_ERROR HAL_USART_ReceiveBuffer(TS_USART* USARTx, uint8_t* Data, uint16_t Len, uint16_t* RecievedLen)
{


	uint16_t u16_Len = 0u;
	*RecievedLen = 0u;

	while (Len > u16_Len)
	{
		if (ERR_OK == HAL_USART_GetChar( USART1, Data))
		{
			Data++;
			u16_Len++;
			(*RecievedLen)++;
		}
		else
		{
			return ERR_BUFFER_EMPTY;
		}
	}

	return ERR_OK;
}

static void vHal_USART_PublishData(void)
{
	uint8_t u8_Data = 0u;

	while( (false == bCircularFIFOBuffer_isEmpty(&TX_Buffer)) && (0u != LL_USART_IsActiveFlag_TXE(USART1)))
	{
		bCircularFIFOBuffer_getElement(&TX_Buffer, (uint8_t*) &u8_Data);
		LL_USART_TransmitData8(USART1, u8_Data);
	}

	if(false == bCircularFIFOBuffer_isEmpty(&TX_Buffer))
	{
		if(false == LL_USART_IsEnabledIT_TXE(USART1))
		{
			LL_USART_EnableIT_TXE(USART1);
		}
	}
	else
	{
		if(false != LL_USART_IsEnabledIT_TXE(USART1))
		{
			LL_USART_DisableIT_TXE(USART1);
		}
		if(false == LL_USART_IsEnabledIT_TC(USART1))
		{
			LL_USART_EnableIT_TC(USART1);
		}
	}
}
