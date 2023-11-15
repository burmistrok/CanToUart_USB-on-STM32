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


inline static void Can_EnableClock(void);
inline static void Can_ConfigPins(void);
inline static void Can_GeneralConfig(void);
inline static void Can_ConfigBaudRate(void);
inline static void Can_ConfigFilters(void);

void vCan_Init(void)
{
	u8_CanStatus = 0u;

	Can_EnableClock();
	Can_ConfigPins();

	CAN1->MCR |= CAN_MCR_INRQ;						/*Start initialization*/
	while( 0u == (CAN1->MSR & CAN_MSR_INAK_Msk)); 	/*wait Entery in Init state*/


	Can_GeneralConfig();
	Can_ConfigBaudRate();
	Can_ConfigFilters();


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

		Hal_Can_Transmit(&ls_CanData);
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


inline static void Can_EnableClock(void)
{
	RCC->APB1ENR|= RCC_APB1ENR_CAN1EN; 		/*Enable Can Clock*/
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);	/*Enable PortA Clock*/
}
inline static void Can_ConfigPins(void)
{
	LL_GPIO_AF_RemapPartial2_CAN1();				/*Remap A11 and A12 to can RX and TX*/
	/*RxPin*/
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_FLOATING);
	/*TxPin*/
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
}
inline static void Can_GeneralConfig(void)
{
	CAN1->MCR &= ~CAN_MCR_DBF;						/*0: CAN working during debug*/
	CAN1->MCR |= CAN_MCR_TXFP; 						/* Priority driven by the request order (chronologically)*/
	CAN1->MCR |= CAN_MCR_AWUM;						/*Wake up at message on buss*/
}
inline static void Can_ConfigBaudRate(void)
{
	/*Config baud rate to 500k*/
	CAN1->BTR &= ~CAN_BTR_BRP;
	CAN1->BTR |= 4U << CAN_BTR_BRP_Pos; /* 36MHz/4 =9MHz */
	CAN1->BTR &= ~(CAN_BTR_TS1_Msk);
	CAN1->BTR |= CAN_BTR_TS1_0 | CAN_BTR_TS1_1 | CAN_BTR_TS1_2 | CAN_BTR_TS1_3;		/* Seg1 = 15 */
	CAN1->BTR &= ~(CAN_BTR_TS2_Msk);
	CAN1->BTR |= CAN_BTR_TS2_1;			/* Seg2 = 2 */


#ifdef CAN_DBG_USED
	CAN1->BTR &=~CAN_BTR_SILM;
	CAN1->BTR |= CAN_BTR_LBKM;
#endif
}
inline static void Can_ConfigFilters(void)
{
	/*Receive all the messagess*/
	CAN1->sFilterRegister[0].FR1 = 0x00U;
	CAN1->sFilterRegister[0].FR2 = 0x00U;

	/* Enable first filter */
	CAN1->FA1R |= CAN_FA1R_FACT0;

	//CAN1->FFA1R &= ~(0x01);

	CAN1->FS1R |= CAN_FS1R_FSC0;	/* Single 32-bit scale configuration for filter 1*/

	//CAN1->FM1R &= ~(0x01); /*Mask mode*/

	CAN1->FMR &= ~CAN_FMR_FINIT;	/* Activate Filters */
}
