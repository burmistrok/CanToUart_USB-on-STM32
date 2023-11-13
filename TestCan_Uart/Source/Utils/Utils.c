/*
 * Utils.c
 *
 *  Created on: Apr 13, 2020
 *      Author: ok
 */


#include "Utils.h"


#define START_LOWERCASE		97u
#define STOP_LOWERCASE		122u
#define CASE_DIFFERENCE		32u

/*******************************
 * Funtion:		u16Calculate_CRC16Modbus
 * @brief:
 * Parameters:	u8Data
 * 				u16Len
 * Return:		lu16_crc
 */


uint16_t u16Calculate_CRC16Modbus(uint8_t* u8Data, uint16_t u16Len)
{
	 uint16_t lu16_crc = 0xFFFF;
	 uint16_t lu16_counter, lu16_pos;

	 for (lu16_pos = 0; lu16_pos < u16Len; lu16_pos++)
	 {
		 lu16_crc ^= (uint16_t)u8Data[lu16_pos];
		 for (lu16_counter = 0; lu16_counter < 8; ++lu16_counter)
		 {
			 if (lu16_crc & 1)
				 lu16_crc = (lu16_crc >> 1) ^ 0xA001;
			 else
				 lu16_crc = (lu16_crc >> 1);
		 }
	 }
  return lu16_crc;
}



void vtoUpperMv(uint8_t* lu8_Data, uint16_t lu16_Len)
{
	while (lu16_Len--)
	{
		if ( ( START_LOWERCASE <= *lu8_Data ) && ( STOP_LOWERCASE >= *lu8_Data ) )
		{
			*lu8_Data -= CASE_DIFFERENCE;
		}
		lu8_Data++;
	}
}


void vMemsetMv(uint8_t* lu8_Data, const uint8_t cu8_El, uint16_t lu16_Len)
{
	while (lu16_Len--)
	{
		*(lu8_Data++) = cu8_El;
	}
}

bool bStrstrMv( const uint8_t* const cpu8_Des, const uint8_t* const cpu8_str, const uint16_t cu16_Len)
{
	uint16_t lu16_CurPosDs = 0u;
	uint16_t lu16_CurPosNs = 0u;
	uint16_t lu16_StartPsNs = 0u;

	for (lu16_CurPosDs = 0u; lu16_CurPosDs < cu16_Len; lu16_CurPosDs++ )
	{

		for (lu16_CurPosNs = lu16_CurPosDs, lu16_StartPsNs = 0u; cpu8_Des[lu16_CurPosNs] == cpu8_str[lu16_StartPsNs]; lu16_CurPosNs++, lu16_StartPsNs++)
		{
			if ( cpu8_str[lu16_StartPsNs+1u] == '\0')
			{
				return true;
			}
		}
	}


	return false;
}

uint16_t u16_strlenMv(const uint8_t* cpu8_Des)
{
	uint16_t lu16_Len = 0u;
	while ('\0' != *(cpu8_Des++))
	{
		lu16_Len++;
	}
	return lu16_Len;
}

uint16_t u16_strcatMv( uint8_t*  cpu8_Des, const uint8_t* cpu8_str, uint16_t lu16_Len)
{
	while ('\0' != (*cpu8_str))
	{
		cpu8_Des[lu16_Len++] = *cpu8_str;
		cpu8_str++;
	}
	return lu16_Len;
}
