/*
 * Hal_Can.c
 *
 *  Created on: Nov 12, 2023
 *      Author: MO
 */


#include "Std_types.h"
#include "Hal_Can.h"
#include "HAL_USART.h"

#define ONE_BYTES_DATA_SIZE		(8u)
#define FOUR_BYTES_DATA_SIZE	(32u)
#define HALF_DATA				(MAX_STD_DLC_LENGHT/2u)

#define TASK_PERIOD	(32u)

#define MESSAGE_PUSH_TIME	(1000u/TASK_PERIOD)


static uint16_t u16_CanStatus;
static volatile uint8_t RxIndx;


inline static void Can_EnableClock(void);
inline static void Can_ConfigPins(void);
inline static void Can_GeneralConfig(void);
inline static void Can_ConfigBaudRate(void);
inline static void Can_EnableIT(void);
inline static void Can_ConfigFilters(void);

void vCan_Init(void)
{
	u16_CanStatus = MESSAGE_PUSH_TIME;

	Can_EnableClock();
	Can_ConfigPins();

	CAN1->MCR |= CAN_MCR_INRQ;						/*Start initialization*/
	while( 0u == (CAN1->MSR & CAN_MSR_INAK_Msk)); 	/*wait Entery in Init state*/


	Can_GeneralConfig();
	Can_ConfigBaudRate();
	Can_EnableIT();
	Can_ConfigFilters();


	CAN1->MCR &= ~CAN_MCR_INRQ_Msk; /*Enter Normal Mode*/
	//while( 0u != ((CAN1->MSR & CAN_MSR_INAK_Msk) | (CAN1->MSR &CAN_MCR_SLEEP_Msk)) );

}

void vCan_MainFunction(void)
{
	TS_CanFrame ls_CanData = {0u};
	if(MESSAGE_PUSH_TIME < u16_CanStatus)
	{

		ls_CanData.u32CanFrameId = 0x50;
		ls_CanData.u8_DLC = 8u;
		ls_CanData.ua8_Data[0u] = 0xAA;
		ls_CanData.ua8_Data[2u] = 0xAA;
		ls_CanData.ua8_Data[4u] = 0xAA;
		ls_CanData.ua8_Data[6u] = 0xAA;
		ls_CanData.ua8_Data[7u] = 0xAA;

		Hal_Can_Transmit(&ls_CanData);
		u16_CanStatus = 0u;
	}
	else
	{
		u16_CanStatus++;
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
	CAN1->BTR |= 3U << CAN_BTR_BRP_Pos; /* 36MHz/4 =9MHz */
	CAN1->BTR &= ~CAN_BTR_SJW;
	CAN1->BTR &= ~(CAN_BTR_TS1_Msk);
	CAN1->BTR |= CAN_BTR_TS1_1 | CAN_BTR_TS1_2 | CAN_BTR_TS1_3;		/* Seg1 = 15 */
	CAN1->BTR &= ~(CAN_BTR_TS2_Msk);
	CAN1->BTR |= CAN_BTR_TS2_0;			/* Seg2 = 2 */


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


inline static void Can_EnableIT(void)
{
#ifdef USE_TX_CONFIRMATIN
	CAN1->IER |= CAN_IER_TMEIE;
	NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
#endif


	CAN1->IER |= CAN_IER_FMPIE0;
	NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);

	CAN1->IER |= CAN_IER_FMPIE1;
	NVIC_SetPriority(CAN1_RX1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	NVIC_EnableIRQ(CAN1_RX1_IRQn);

}

#ifdef USE_TX_CONFIRMATIN
void USB_HP_CAN1_TX_IRQHandler(void)
{
	CAN1->TSR |= CAN_TSR_RQCP0;

}
#endif

/**
  * @brief This function handles USB low priority or CAN RX0 interrupts.
  */
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	uint8_t lua8_Buffer[13];
	uint16_t u16_len;
	TS_CanFrame ls_Local_Frame;
	ls_Local_Frame.u32CanFrameId = (uint32_t)((uint32_t)(CAN1->sFIFOMailBox[0].RIR & CAN_RI0R_STID_Msk) >> CAN_RI0R_STID_Pos);
	ls_Local_Frame.RTR = (uint32_t)((uint32_t)(CAN1->sFIFOMailBox[0].RIR & CAN_RI0R_RTR_Msk) >> CAN_RI0R_RTR_Pos);
	ls_Local_Frame.IDE = (uint32_t)((uint32_t)(CAN1->sFIFOMailBox[0].RIR & CAN_RI0R_IDE_Msk) >> CAN_RI0R_IDE_Pos);
	ls_Local_Frame.u8_DLC = (uint32_t)((uint32_t)(CAN1->sFIFOMailBox[0].RDTR & CAN_RDT0R_DLC_Msk) >> CAN_RDT0R_DLC_Pos);

	ls_Local_Frame.ua8_Data[0u] = (uint32_t)((uint32_t)(CAN1->sFIFOMailBox[0].RDLR & CAN_RDL0R_DATA0_Msk) >> CAN_RDL0R_DATA0_Pos);
	ls_Local_Frame.ua8_Data[1u] = (uint32_t)((uint32_t)(CAN1->sFIFOMailBox[0].RDLR & CAN_RDL0R_DATA1_Msk) >> CAN_RDL0R_DATA1_Pos);
	ls_Local_Frame.ua8_Data[2u] = (uint32_t)((uint32_t)(CAN1->sFIFOMailBox[0].RDLR & CAN_RDL0R_DATA2_Msk) >> CAN_RDL0R_DATA2_Pos);
	ls_Local_Frame.ua8_Data[3u] = (uint32_t)((uint32_t)(CAN1->sFIFOMailBox[0].RDLR & CAN_RDL0R_DATA3_Msk) >> CAN_RDL0R_DATA3_Pos);

	ls_Local_Frame.ua8_Data[4u] = (uint32_t)((uint32_t)(CAN1->sFIFOMailBox[0].RDHR & CAN_RDH0R_DATA4_Msk) >> CAN_RDH0R_DATA4_Pos);
	ls_Local_Frame.ua8_Data[5u] = (uint32_t)((uint32_t)(CAN1->sFIFOMailBox[0].RDHR & CAN_RDH0R_DATA5_Msk) >> CAN_RDH0R_DATA5_Pos);
	ls_Local_Frame.ua8_Data[6u] = (uint32_t)((uint32_t)(CAN1->sFIFOMailBox[0].RDHR & CAN_RDH0R_DATA6_Msk) >> CAN_RDH0R_DATA6_Pos);
	ls_Local_Frame.ua8_Data[7u] = (uint32_t)((uint32_t)(CAN1->sFIFOMailBox[0].RDHR & CAN_RDH0R_DATA7_Msk) >> CAN_RDH0R_DATA7_Pos);


	lua8_Buffer[0u] = (uint8_t)(ls_Local_Frame.u32CanFrameId & 0xFF);
	lua8_Buffer[1u] = (uint8_t)((ls_Local_Frame.u32CanFrameId & 0xFF) >> 8u);
	lua8_Buffer[2u] = (uint8_t)((ls_Local_Frame.u32CanFrameId & 0xFF) >> 16u);
	lua8_Buffer[3u] = (uint8_t)((ls_Local_Frame.u32CanFrameId & 0xFF) >> 24u);

	lua8_Buffer[4u] = ls_Local_Frame.u8_DLC;

	lua8_Buffer[5u] = ls_Local_Frame.ua8_Data[0u];
	lua8_Buffer[6u] = ls_Local_Frame.ua8_Data[1u];
	lua8_Buffer[7u] = ls_Local_Frame.ua8_Data[2u];
	lua8_Buffer[8u] = ls_Local_Frame.ua8_Data[3u];
	lua8_Buffer[9u] = ls_Local_Frame.ua8_Data[4u];
	lua8_Buffer[10u] = ls_Local_Frame.ua8_Data[5u];
	lua8_Buffer[11u] = ls_Local_Frame.ua8_Data[6u];
	lua8_Buffer[12u] = ls_Local_Frame.ua8_Data[7u];


	HAL_USART_SendBuffer(USART1, &lua8_Buffer[0], 13u, &u16_len);


	CAN1->RF0R |= CAN_RF0R_RFOM0;	/*Release FIFO mailbox*/

}

/**
  * @brief This function handles CAN RX1 interrupt.
  */
void CAN1_RX1_IRQHandler(void)
{


}

