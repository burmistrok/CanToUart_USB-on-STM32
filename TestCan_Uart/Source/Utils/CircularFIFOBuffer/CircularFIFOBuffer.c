/*
 * CircularFIFOBuffer.c
 *
 *  Created on: Apr 14, 2020
 *      Author: ok
 */

#include "CircularFIFOBuffer.h"


//***********************************************************************************************

bool bCircularFIFOBuffer_isFull(TS_CircularFIFOBuffer* rg_Buffer){
	bool RetVal;
	if ((BUFFER_SIZE-1u) == rg_Buffer->u16_BufferSize)
	{
		RetVal = true ;
	}
	else
	{
		RetVal = false;
	}
	return RetVal;
}

bool bCircularFIFOBuffer_isEmpty(TS_CircularFIFOBuffer* rg_Buffer){
	bool RetVal;
	if( 0u == rg_Buffer->u16_BufferSize )
	{
		RetVal = true ;
	}
	else
	{
		RetVal = false;
	}
	return RetVal;
}



void vCircularFIFOBuffer_Init(TS_CircularFIFOBuffer* rg_Buffer)
{
	memset(rg_Buffer->Buffer, 0, BUFFER_SIZE);

	rg_Buffer->u16_BufferSize = 0u;
	rg_Buffer->first = 0u;
	rg_Buffer->next = 0u;

}


// Get the first element from the FIFO queue
bool bCircularFIFOBuffer_getElement(TS_CircularFIFOBuffer* rg_Buffer, uint8_t* theElement)
{
	bool RetVal = false;
	if ( false == bCircularFIFOBuffer_isEmpty(rg_Buffer))
	{
		if(0u != rg_Buffer->u16_BufferSize)
		{
			*theElement = rg_Buffer->Buffer[rg_Buffer->first];
			if ( rg_Buffer->first < BUFFER_SIZE )
			{
				rg_Buffer->first++;
			}
			else
			{
				rg_Buffer->first = 0;
			}
			rg_Buffer->u16_BufferSize--;
			RetVal = true;
		}
		else
		{
			RetVal = false;
		}
	}
	else
	{
		RetVal = false;
	}

	return RetVal;// Return !0 always if it is not empty
}

// Add an element to the FIFO queue
bool bCircularFIFOBuffer_addElement(TS_CircularFIFOBuffer* rg_Buffer, uint8_t data)
{
	bool RetVal = false;
	if(false == bCircularFIFOBuffer_isFull(rg_Buffer))
	{
		if(BUFFER_SIZE > rg_Buffer->u16_BufferSize)
		{
			rg_Buffer->Buffer[rg_Buffer->next] = data;
			if ( rg_Buffer->next < BUFFER_SIZE )
			{
				rg_Buffer->next++;
			}
			else
			{
				rg_Buffer->next = 0;
			}
			rg_Buffer->u16_BufferSize++;
			RetVal = true;
		}
		else
		{
			RetVal = false;
		}
	}
	else
	{
		RetVal = false;
	}
	return RetVal;
}
