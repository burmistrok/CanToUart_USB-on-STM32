/*
 * USART.c
 *
 *  Created on: Apr 14, 2020
 *      Author: ok
 */


#include "HAL_USART.h"
#include "CircularFIFOBuffer.h"

#define ENTRY_CRITICAL_SECTION()	__disable_irq() /* disable all interrupts */
#define EXIT_CRITICAL_SECTION()		__enable_irq()   /* enable all interrupts */

static bool bUSARTInit = false;
static volatile TS_CircularFIFOBuffer TX_Buffer;
static volatile TS_CircularFIFOBuffer RX_Buffer;


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
	vCircularFIFOBuffer_Init((TS_CircularFIFOBuffer*)&TX_Buffer);
	vCircularFIFOBuffer_Init((TS_CircularFIFOBuffer*)&RX_Buffer);

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
		bCircularFIFOBuffer_addElement((TS_CircularFIFOBuffer*)&RX_Buffer, LL_USART_ReceiveData8(USART1));
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
	TE_ERROR le_RetVal = ERR_NOK;
	bool lb_RetVal;
	ENTRY_CRITICAL_SECTION();
	lb_RetVal = bCircularFIFOBuffer_getElement((TS_CircularFIFOBuffer*)&RX_Buffer, (uint8_t*) &u8_Data);
	if (false !=  lb_RetVal)
	{
		*Data = u8_Data;
		le_RetVal = ERR_OK;
	}
	else
	{
		le_RetVal =  ERR_NOK;
	}
	EXIT_CRITICAL_SECTION();
	return le_RetVal;


}


TE_ERROR HAL_USART_SendChar(TS_USART* USARTx, uint8_t Data)
{

	TE_ERROR eRetStatut = ERR_OK;
	bool lb_RetVal;

	ENTRY_CRITICAL_SECTION();
	lb_RetVal = bCircularFIFOBuffer_addElement((TS_CircularFIFOBuffer*)&TX_Buffer, Data);

	if(false == lb_RetVal )
	{

		eRetStatut = ERR_BUFFER_FULL;
	}
	EXIT_CRITICAL_SECTION();
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
	bool lb_RetVal;

	ENTRY_CRITICAL_SECTION();
	while( (false == bCircularFIFOBuffer_isEmpty((TS_CircularFIFOBuffer*)&TX_Buffer)) && (0u != LL_USART_IsActiveFlag_TXE(USART1)))
	{
		bCircularFIFOBuffer_getElement((TS_CircularFIFOBuffer*)&TX_Buffer, (uint8_t*) &u8_Data);
		LL_USART_TransmitData8(USART1, u8_Data);
	}
	lb_RetVal = bCircularFIFOBuffer_isEmpty((TS_CircularFIFOBuffer*)&TX_Buffer);
	EXIT_CRITICAL_SECTION();
	if(false == lb_RetVal)
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
