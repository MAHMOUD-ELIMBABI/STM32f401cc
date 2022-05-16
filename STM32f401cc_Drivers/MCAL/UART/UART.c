#include <SERVCIES/Std_types.h>
#include <MCAL/UART/UART.h>
#include <MCAL/UART/UART_cfg.h>

//////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	volatile u32 USART_SR;
	volatile u32 USART_DR;
	volatile u32 USART_BRR;
	volatile u32 USART_CR1;
	volatile u32 USART_CR2;
	volatile u32 USART_CR3;
	volatile u32 USART_GTPR;
} UART_Register_t;

#define UART1   ((UART_Register_t *)(0x40011000))
#define UART2   ((UART_Register_t *)(0x40004400))
#define UART6   ((UART_Register_t *)(0x40011400))
//////////////////////////////////////////////////////////////////////////////////////////////

static UartCbf_t Uart_Cbf_Save[3];

static Buffer_t * BufferTxSave[3];

static Buffer_t * BufferRxSave[3];

static u8 ChannelTxBusy[3];

static u8 ChannelRxBusy[3];
//////////////////////////////////////////////////////////////////////////////

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART6_IRQHandler(void);
//////////////////////////////////////////////////////////////////////////////////////////////

void UART_vidInit(Uart_tstrInitCfg * Add_pstrConfig)
{

	UART_Register_t * Loc_pstrUartChannel = Add_pstrConfig->Uart_pvChannel;
	u32 UARTDIV;
	u32 Fraction;
	u32 Mantissa;

	if (Add_pstrConfig->Uart_u32OverSampling)
	{
		UARTDIV = ((SYSTEM_CLK) / (8 * Add_pstrConfig->Uart_u32BaudRate))
				* 1000;
		Fraction = (UARTDIV % 1000) * 8;
	}
	else
	{
		UARTDIV = ((SYSTEM_CLK) / (16 * Add_pstrConfig->Uart_u32BaudRate))
				* 1000;
		Fraction = (UARTDIV % 1000) * 16;

	}

	Mantissa = (UARTDIV / 1000);
	Fraction = (Fraction / 1000.0) + 0.5;

	Loc_pstrUartChannel->USART_CR1 = 0;
	Loc_pstrUartChannel->USART_CR1 =
			(Add_pstrConfig->Uart_u32OverSampling
					| Add_pstrConfig->Uart_u32DataLength
					| Add_pstrConfig->Uart_u32ParityCtrl
					| Add_pstrConfig->Uart_ParitySelection
					| Add_pstrConfig->Uart_InterruptCtrl
					| Add_pstrConfig->Uart_TxRxCtrl);

	Loc_pstrUartChannel->USART_BRR = (Mantissa << 4) | Fraction;

	switch ((u32) Loc_pstrUartChannel)
	{
	case (u32) UART_pvUART1:
		Uart_Cbf_Save[0] = Add_pstrConfig->Uart_pfCbf;
		break;
	case (u32) UART_pvUART2:
		Uart_Cbf_Save[1] = Add_pstrConfig->Uart_pfCbf;
		break;
	case (u32) UART_pvUART6:
		Uart_Cbf_Save[2] = Add_pstrConfig->Uart_pfCbf;
		break;
	}


	Loc_pstrUartChannel->USART_CR1 |= 0x2000;
}
/////////////////////////////////////////////////////////////////////////////////////////////
Uart_tenuErrorStatus Uart_vidSendByte(void * Uart_pvChannel, u16 Copy_u16Byte)
{

	Uart_tenuErrorStatus Loc_tenuErrorStatus = Uart_enuOk;
	UART_Register_t * Loc_pstrUartChannel = Uart_pvChannel;

	if (Loc_pstrUartChannel->USART_SR & UART_TXE_MSK)
	{
		if (Copy_u16Byte < UART_DATA_MAX_VALUE)
		{

			Loc_pstrUartChannel->USART_DR = Copy_u16Byte;
		}
		else
		{

			Loc_tenuErrorStatus = Uart_InvalidData;

		}
	}
	else
	{
		Loc_tenuErrorStatus = Uart_DataRegNotEmpty;

	}

	return Loc_tenuErrorStatus;

}
//////////////////////////////////////////////////////////////////////////////////////////////
Uart_tenuErrorStatus Uart_vidRecieveByte(void * Uart_pvChannel,
		pu16 Add_pu16Byte)
{

	Uart_tenuErrorStatus Loc_tenuErrorStatus = Uart_enuOk;

	UART_Register_t * Loc_pstrUartChannel = Uart_pvChannel;

	if (Add_pu16Byte == NULL || Uart_pvChannel == NULL)
	{

		Loc_tenuErrorStatus = Uart_NullPointer;
	}
	else
	{

		if (Loc_pstrUartChannel->USART_SR & UART_RXNE_MSK)
		{
			*Add_pu16Byte = Loc_pstrUartChannel->USART_DR;
		}
		else
		{
			Loc_tenuErrorStatus = Uart_DataRegEmpty;

		}
	}
	return Loc_tenuErrorStatus;

}
//////////////////////////////////////////////////////////////////////////////////////////////

Uart_tenuErrorStatus UART_vidSendBuffer(void * Channel, Buffer_t * Add_Buffer)
{
	Uart_tenuErrorStatus Loc_tenuErrorStatus = Uart_enuOk;

	u8 LocTimeOut = 100;
	switch ((u32) Channel)
	{
	case (u32) UART1:

		if (ChannelTxBusy[0] == 1)
		{
			Loc_tenuErrorStatus = Uart_TxBusyFlag;
		}

		else
		{

			BufferTxSave[0] = Add_Buffer;

			Add_Buffer->Used = 0;

			if (Add_Buffer->Size != 0)
			{

				while ((!(UART1->USART_SR & UART_TXE_MSK)) && (LocTimeOut))
				{

					LocTimeOut--;

				}
				if ((UART1->USART_SR & UART_TXE_MSK))
				{
					UART1->USART_DR = Add_Buffer->Data[0];

					Add_Buffer->Used++;


					ChannelTxBusy[0] = 1;
					UART1->USART_CR1 |= UART_u32TC_INTERRUPT;
				}
				else
				{
					Loc_tenuErrorStatus = Uart_TimeOut;
				}
			}

		}

		break;

	case (u32) UART2:

		if (ChannelTxBusy[1] == 1)
		{
			Loc_tenuErrorStatus = Uart_TxBusyFlag;

		}

		else
		{

			BufferTxSave[1] = Add_Buffer;

			Add_Buffer->Used = 0;

			if (Add_Buffer->Size != 0)
			{
				while ((!(UART2->USART_SR & UART_TXE_MSK)) && (LocTimeOut))
				{

					LocTimeOut--;

				}
				if ((UART2->USART_SR & UART_TXE_MSK))
				{

					UART2->USART_DR = Add_Buffer->Data[0];

					Add_Buffer->Used++;

					ChannelTxBusy[1] = 1;
					UART2->USART_CR1 |= UART_u32TC_INTERRUPT;
				}
				else
				{
					Loc_tenuErrorStatus = Uart_TimeOut;

				}
			}

		}

		break;

	case (u32) UART6:

		if (ChannelTxBusy[2] == 1)
		{
			Loc_tenuErrorStatus = Uart_TxBusyFlag;

		}

		else
		{

			BufferTxSave[2] = Add_Buffer;

			Add_Buffer->Used = 0;

			if (Add_Buffer->Size != 0)
			{

				while ((!(UART6->USART_SR & UART_TXE_MSK)) && (LocTimeOut))
				{

					LocTimeOut--;

				}
				if ((UART6->USART_SR & UART_TXE_MSK))
				{

					UART6->USART_DR = Add_Buffer->Data[0];

					Add_Buffer->Used++;

					ChannelTxBusy[2] = 1;
					UART6->USART_CR1 |= UART_u32TC_INTERRUPT;
				}
				else
				{
					Loc_tenuErrorStatus = Uart_TimeOut;

				}
			}

		}
		break;

	}
	return Loc_tenuErrorStatus;
}
//////////////////////////////////////////////////////////////////////////////////////////////
Uart_tenuErrorStatus UART_vidRecieveBuffer(void * Channel,
		Buffer_t * Add_Buffer)
{

	Uart_tenuErrorStatus Loc_tenuErrorStatus = Uart_enuOk;

	switch ((u32) Channel)
	{
	case (u32) UART1:

		if (ChannelRxBusy[0] == 1)
		{
			Loc_tenuErrorStatus = Uart_RxBusyFlag;

		}

		else
		{

			BufferRxSave[0] = Add_Buffer;

			Add_Buffer->Used = 0;

			if (Add_Buffer->Size != 0)
			{
				ChannelRxBusy[0] = 1;
				UART1->USART_CR1 |= 0x20;
			}

		}

		break;

	case (u32) UART2:
		if (ChannelRxBusy[1] == 1)
		{
			Loc_tenuErrorStatus = Uart_RxBusyFlag;

		}

		else
		{

			BufferRxSave[1] = Add_Buffer;

			Add_Buffer->Used = 0;

			if (Add_Buffer->Size != 0)
			{
				ChannelRxBusy[1] = 1;
				UART2->USART_CR1 |= 0x20;

			}

		}

		break;

	case (u32) UART6:
		if (ChannelRxBusy[2] == 1)
		{
			Loc_tenuErrorStatus = Uart_RxBusyFlag;

		}

		else
		{

			BufferRxSave[2] = Add_Buffer;

			Add_Buffer->Used = 0;

			if (Add_Buffer->Size != 0)
			{
				ChannelRxBusy[2] = 1;
				UART6->USART_CR1 |= 0x20;
			}

		}

		break;

	}

	return Loc_tenuErrorStatus;

}
//////////////////////////////////////////////////////////////////////////////////////////////
void USART1_IRQHandler(void)
{
	if ((UART1->USART_SR & UART_u32TC_INTERRUPT) && ChannelTxBusy[0])
	{
		if (BufferTxSave[0]->Used < BufferTxSave[0]->Size)
		{
			UART1->USART_DR = BufferTxSave[0]->Data[BufferTxSave[0]->Used];

			BufferTxSave[0]->Used++;
		}
		else
		{
			UART1->USART_SR &= UART_TXC_CLR_MSK;
			ChannelTxBusy[0] = 0;
			Uart_Cbf_Save[0]();
		}
	}
	if ((UART1->USART_SR & UART_RXNE_MSK) && ChannelRxBusy[0])
	{
		if (BufferRxSave[0]->Used < BufferRxSave[0]->Size)
		{
			BufferRxSave[0]->Data[BufferRxSave[0]->Used] = UART1->USART_DR;
			BufferRxSave[0]->Used++;
		}
		if(BufferRxSave[0]->Used == BufferRxSave[0]->Size)
		{
			ChannelRxBusy[0] = 0;
			Uart_Cbf_Save[0]();
		}
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////
void USART2_IRQHandler(void)
{
	if ((UART1->USART_SR & UART_u32TC_INTERRUPT) && ChannelTxBusy[1])
	{
		if (BufferTxSave[1]->Used < BufferTxSave[1]->Size)
		{
			UART2->USART_DR = BufferTxSave[1]->Data[BufferTxSave[1]->Used];

			BufferTxSave[1]->Used++;
		}
		else
		{
			UART2->USART_SR &= UART_TXC_CLR_MSK;
			ChannelTxBusy[1] = 0;
			Uart_Cbf_Save[1]();
		}
	}
	if ((UART2->USART_SR & UART_RXNE_MSK) && ChannelRxBusy[1])
	{
		if (BufferRxSave[1]->Used < BufferRxSave[1]->Size)
		{
			BufferRxSave[1]->Data[BufferRxSave[1]->Used] = UART1->USART_DR;
			BufferRxSave[1]->Used++;
		}
		else
		{
			ChannelRxBusy[1] = 0;
			Uart_Cbf_Save[1]();
		}
	}

}
///////////////////////////////////////////////////////////////////////////////////////////////
void USART6_IRQHandler(void)
{
	if ((UART6->USART_SR & UART_u32TC_INTERRUPT) && ChannelTxBusy[2])
	{
		if (BufferTxSave[2]->Used < BufferTxSave[2]->Size)
		{
			UART6->USART_DR = BufferTxSave[2]->Data[BufferTxSave[2]->Used];

			BufferTxSave[2]->Used++;
		}
		else
		{
			UART6->USART_SR &= UART_TXC_CLR_MSK;
			ChannelTxBusy[2] = 0;
			Uart_Cbf_Save[2]();
		}
	}
	if ((UART6->USART_SR & UART_RXNE_MSK) && ChannelRxBusy[2])
	{
		if (BufferRxSave[2]->Used < BufferRxSave[2]->Size)
		{
			BufferRxSave[2]->Data[BufferRxSave[2]->Used] = UART1->USART_DR;
			BufferRxSave[2]->Used++;
		}
		else
		{
			ChannelRxBusy[2] = 0;
			Uart_Cbf_Save[2]();
		}
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////
Uart_tenuErrorStatus Uart_vidReadTXEFlag(void * Uart_pvChannel, pu8 Add_pu8Status)
{
	Uart_tenuErrorStatus Loc_tenuErrorStatus = Uart_enuOk;
	UART_Register_t * Loc_pstrUartChannel = Uart_pvChannel;
	if ((Add_pu8Status == NULL) || (Uart_pvChannel == NULL))
	{

		Loc_tenuErrorStatus = Uart_NullPointer;

	}

	if (Loc_pstrUartChannel->USART_SR & UART_TXE_MSK)
	{
		*Add_pu8Status = UART_TXE_EMPTY;
	}
	else
	{
		*Add_pu8Status = UART_TXE_NOTEMPTY;
	}

	return Loc_tenuErrorStatus;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
Uart_tenuErrorStatus Uart_vidReadTXCFlag(void * Uart_pvChannel, pu8 Add_pu8Status)
{
	Uart_tenuErrorStatus Loc_tenuErrorStatus = Uart_enuOk;
	UART_Register_t * Loc_pstrUartChannel = Uart_pvChannel;
	if ((Add_pu8Status == NULL) || (Uart_pvChannel == NULL))
	{

		Loc_tenuErrorStatus = Uart_NullPointer;

	}

	if (Loc_pstrUartChannel->USART_SR & UART_u32TC_INTERRUPT)
	{
		*Add_pu8Status = UART_TXC_COMPLETE;
	}
	else
	{
		*Add_pu8Status = UART_TXC_NOTCOMPLETE;
	}

	return Loc_tenuErrorStatus;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
Uart_tenuErrorStatus Uart_vidReadRXNEFlag(void * Uart_pvChannel, pu8 Add_pu8Status)
{
	Uart_tenuErrorStatus Loc_tenuErrorStatus = Uart_enuOk;
	UART_Register_t * Loc_pstrUartChannel = Uart_pvChannel;
	if ((Add_pu8Status == NULL) || (Uart_pvChannel == NULL))
	{

		Loc_tenuErrorStatus = Uart_NullPointer;

	}

	if (Loc_pstrUartChannel->USART_SR & UART_RXNE_MSK)
	{
		*Add_pu8Status = UART_RXNE_NOTEMPTY;
	}
	else
	{
		*Add_pu8Status = UART_RXNE_EMPTY;
	}

	return Loc_tenuErrorStatus;
}
////////////////////////////////////////////////////////////////////////////////////////////////
