/*
 * Hal_Can.c
 *
 *  Created on: Nov 12, 2023
 *      Author: MO
 */


#include "Std_types.h"
#include "Hal_Can.h"

#define ONE_BYTES_DATA_SIZE		(8u)
#define FOUR_BYTES_DATA_SIZE	(32u)
#define HALF_DATA				(MAX_STD_DLC_LENGHT/2u)

static uint8_t u8_CanStatus;
static volatile uint8_t RxIndx;

void vCan_Init(void)
{
	u8_CanStatus = 0u;
	RCC->APB1ENR|= RCC_APB1ENR_CAN1EN; 		/*Enable Can Clock*/
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);	/*Enable PortA Clock*/

	/*Set RX and Tx Pin*/
#ifndef USECMSIS_FORCAN
	/* PA11 - CAN_RX */
	GPIOA->CRH	&= ~GPIO_CRH_CNF11;   /* CNF11 = 00 */
	GPIOA->CRH	|= GPIO_CRH_CNF11_1;  /* CNF11 = 10 -> AF Out | Push-pull (CAN_RX) */
	GPIOA->CRH 	|= GPIO_CRH_MODE11;   /* MODE8 = 11 -> Maximum output speed 50 MHz */
	/* PA12 - CAN_TX */
	GPIOA->CRH	&= ~GPIO_CRH_CNF12;	  /* CNF12 = 00 */
	GPIOA->CRH	|= GPIO_CRH_CNF12_1;	/* CNF12 = 10 -> AF Out | Push-pull (CAN_TX) */
	GPIOA->CRH 	|= GPIO_CRH_MODE12;   /* MODE8 = 11 -> Maximum output speed 50 MHz */
#else
	LL_GPIO_AF_RemapPartial1_CAN1();				/*Remap A11 and A12 to can RX and TX*/
	/*RxPin*/
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_11, LL_GPIO_MODE_FLOATING);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_11, LL_GPIO_SPEED_FREQ_HIGH);
	/*TxPin*/
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_12, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_12, LL_GPIO_SPEED_FREQ_HIGH);
#endif
	CAN1->MCR |= CAN_MCR_INRQ;						/*Start initialization*/
	while( 0u == (CAN1->MSR & CAN_MSR_INAK_Msk)); 	/*wait Entery in Init state*/
	CAN1->MCR &= ~CAN_MCR_DBF;						/*0: CAN working during debug*/
	CAN1->MCR |= CAN_MCR_TXFP; 						/* Priority driven by the request order (chronologically)*/
	CAN1->MCR |= CAN_MCR_AWUM;						/*Wake up at message on buss*/

	/*Config baud rate to 250k*/
	CAN1->BTR &= ~CAN_BTR_BRP;
	CAN1->BTR |= 8U << CAN_BTR_BRP_Pos; /* 36MHz/8 =4.5MHz */
	CAN1->BTR &= ~(0xFU << CAN_BTR_TS1_Pos);
	CAN1->BTR |= 12U << CAN_BTR_TS1_Pos;
	CAN1->BTR &= ~(7U << CAN_BTR_TS2_Pos);
	CAN1->BTR |=   1U << CAN_BTR_TS2_Pos;


#ifndef CAN_DBG_USED
	//CAN1->BTR &=~CAN_BTR_SILM;
	//CAN1->BTR |= CAN_BTR_LBKM;
#endif
	CAN1->MCR &= ~CAN_MCR_INRQ_Msk; /*Enter Normal Mode*/
	//while( 0u != ((CAN1->MSR & CAN_MSR_INAK_Msk) | (CAN1->MSR &CAN_MCR_SLEEP_Msk)) );

}

void Hal_Can_MainFunction(void)
{
	TS_CanFrame ls_CanData = {0u};
	if(0u == u8_CanStatus)
	{
		ls_CanData.u32CanFrameId = 0x50;
		ls_CanData.u8_DLC = 8u;
		ls_CanData.ua8_Data[0u] = 0xAA;
		ls_CanData.ua8_Data[2u] = 0xAA;
		ls_CanData.ua8_Data[4u] = 0xAA;
		ls_CanData.ua8_Data[6u] = 0xAA;
		ls_CanData.ua8_Data[7u] = 0xAA;

		//Hal_Can_Transmit(&ls_CanData);
		u8_CanStatus = 1u;
	}

	if(0u != ( (CAN1->RF0R & CAN_RF0R_FMP0_Msk) | (CAN1->RF1R & CAN_RF0R_FMP0_Msk) ) )
	{
		RxIndx++;
	}

}


void Hal_Can_Transmit( TS_CanFrame *lptr_FrameInfo)
{
	uint8_t lu8_DataInx;

	if(0u != lptr_FrameInfo->RTR)
	{
		CAN1->sTxMailBox[0].TIR |= CAN_TI0R_RTR;
	}
	else
	{
		CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_RTR;
	}
	if(0u != lptr_FrameInfo->IDE)
	{
		CAN1->sTxMailBox[0].TIR |= CAN_TI0R_IDE;
	}
	else
	{
		CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_IDE;
	}
	/* Write Can Frame ID */
	CAN1->sTxMailBox[0].TIR &= ~CAN_TI0R_STID;
	CAN1->sTxMailBox[0].TIR |= (lptr_FrameInfo->u32CanFrameId << CAN_TI0R_STID_Pos);
	if(MAX_STD_DLC_LENGHT < lptr_FrameInfo->u8_DLC)
	{
		lptr_FrameInfo->u8_DLC = MAX_STD_DLC_LENGHT;
	}
	CAN1->sTxMailBox[0].TDTR &= ~CAN_TDT0R_DLC;
	CAN1->sTxMailBox[0].TDTR |= (lptr_FrameInfo->u8_DLC << CAN_TDT0R_DLC_Pos);

	CAN1->sTxMailBox[0].TDLR = 0u;
	CAN1->sTxMailBox[0].TDHR = 0u;

	for(lu8_DataInx = 0u; lu8_DataInx < lptr_FrameInfo->u8_DLC; lu8_DataInx++)
	{
		if( HALF_DATA > lu8_DataInx)
		{
			CAN1->sTxMailBox[0].TDLR |= (lptr_FrameInfo->ua8_Data[lu8_DataInx] << (ONE_BYTES_DATA_SIZE * lu8_DataInx));
		}
		else
		{
			CAN1->sTxMailBox[0].TDHR |= (lptr_FrameInfo->ua8_Data[lu8_DataInx] << ((ONE_BYTES_DATA_SIZE * lu8_DataInx)- FOUR_BYTES_DATA_SIZE));
		}
	}

	CAN1->sTxMailBox[0].TIR |= CAN_TI0R_TXRQ;	/*Requeste transmition*/

}
