/*
 * Utils.h
 *
 *  Created on: Apr 13, 2020
 *      Author: ok
 */

#ifndef UTILS_UTILS_H_
#define UTILS_UTILS_H_

#include "Std_Types.h"
#include "stdint.h"



uint16_t u16Calculate_CRC16Modbus(uint8_t* u8Data, uint16_t u16Len);
/****************************************************************************************
 * Funtion:		vtoUpperMv
 * @brief:		Move lu16_Len lower case letters to upper case letter
 * Parameters:	lu8_Data - pointer to start of data
 * 				lu16_Len - Length of data
 * Return:
 */
void vtoUpperMv(uint8_t* lu8_Data, uint16_t lu16_Len);
/****************************************************************************************
 * Funtion:		vMemsetMv
 * @brief:		Fill lu16_Len data with cu8_El form lu8_Data
 * Parameters:	lu8_Data - pointer to start of data
 * 				cu8_El - element to be used for to fill up data
 * 				lu16_Len - Length of data
 * Return:
 */
void vMemsetMv(uint8_t* lu8_Data, const uint8_t cu8_El, uint16_t lu16_Len);

/****************************************************************************************
 * Funtion:		bStrstrMv
 * @brief:		Search string cpu8_str (should be finished with '\0') in first cu16_Len symbols
 * 					starting from cpu8_Des
 * Parameters:	cpu8_Des - constant pointer constant to array where can be string from cpu8_str
 * 				cpu8_str - constant pointer constant to string (should be finished with '\0')
 * 				cu16_Len - length of array where have to find string from cpu8_str
 * Return:		true - if string cpu8_str was founded at least once in fist cu16_Len symbols
 * 					starting from cpu8_Des address
 * 				false - otherwise
 */
bool bStrstrMv( const uint8_t* const cpu8_Des, const uint8_t* const cpu8_str, const uint16_t cu16_Len);

/****************************************************************************************
 * Funtion:		u16_strlenMv
 * @brief:		Count ascii symbols until first '\0' in  cpu8_Des
 * Parameters:	pu8_Des - pointer to start of array
 * Return:		length of string
 */
uint16_t u16_strlenMv(const uint8_t* cpu8_Des);

/****************************************************************************************
 * Funtion:		strcatMv
 * @brief:		Add string from cpu8_str to the end of array cpu8_Des
 * Parameters:	cpu8_Des - pointer to start of array at the end of that should be added string from cpu8_str
 * 				cpu8_str - pointer to constant  string (should be finished with '\0')
 * 				cu16_Len - length of array from cpu8_Des
 * Return:
 */
uint16_t u16_strcatMv( uint8_t*  cpu8_Des, const uint8_t* cpu8_str, uint16_t lu16_Len);

#endif /* UTILS_UTILS_H_ */
