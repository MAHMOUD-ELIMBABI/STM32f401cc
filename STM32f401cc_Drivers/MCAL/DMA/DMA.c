/*
 * DMA.c
 *
 *  Created on: Apr 28, 2022
 *      Author: MahmoudElImbabi
 */
#include<SERVCIES/Std_types.h>
#include<MCAL/DMA/DMA.h>
#include<MCAL/DMA/DMA_cfg.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	volatile u32 DMA_SxCR;
	volatile u32 DMA_SxNDTR;
	volatile u32 DMA_SxPAR;
	volatile u32 DMA_SxM0AR;
	volatile u32 DMA_SxM1AR;
	volatile u32 DMA_SxFCR;
} DMA_Stream_t;

typedef struct
{
	volatile u32 DMA_LISR;
	volatile u32 DMA_HISR;
	volatile u32 DMA_LIFCR;
	volatile u32 DMA_HIFCR;
	volatile DMA_Stream_t DMA_Sx[8];
} DMA_Reg_t;
////////////////////////////////////////////MACROS///////////////////////////////////////////////////////
#define DMA_REG							((DMA_Reg_t *)(DMA_pstrConfiguration->DMA_pvNum))
#define DMA_S_REG						((DMA_Reg_t *)(DMA_pvNum))
#define DMA_NUM_OF_STRAEMS						0x7
/////////////////////////////////////////////////////////////////////////////////////////////////////////
DMA_tenuErrorStatus DMA_enuInit(DMA_Cfg_t * DMA_pstrConfiguration)
{
	DMA_tenuErrorStatus LocErrorStatus = DMA_enuOk;

	if (DMA_pstrConfiguration->DMA_u16NumberOfData > DMA_NUM_OF_STRAEMS)
	{
		LocErrorStatus = DMA_enuOutOfRange;
	}
	else if (DMA_pstrConfiguration->DMA_pvMemoryAddress == NULL)
	{
		LocErrorStatus = DMA_enuNullPointer;
	}
	else if (DMA_pstrConfiguration->DMA_pvPeripheralAddress == NULL)
	{
		LocErrorStatus = DMA_enuNullPointer;
	}
	else
	{
		DMA_REG->DMA_Sx[DMA_pstrConfiguration->DMA_u8Stream].DMA_SxCR = 0;

		DMA_REG->DMA_Sx[DMA_pstrConfiguration->DMA_u8Stream].DMA_SxCR =
				(DMA_pstrConfiguration->DMA_u32Channel
						| DMA_pstrConfiguration->DMA_u32Priority
						| DMA_pstrConfiguration->DMA_u32MemoryDataSize
						| DMA_pstrConfiguration->DMA_u32MemoryIncrementMode
						| DMA_pstrConfiguration->DMA_u32PeripheralDataSize
						| DMA_pstrConfiguration->DMA_u32PeripheralIncrementMode
						| DMA_pstrConfiguration->DMA_u32CircuilarMode
						| DMA_pstrConfiguration->DMA_u32DataDirection
						| DMA_pstrConfiguration->DMA_u32InterruptCtrl);

		DMA_REG->DMA_Sx[DMA_pstrConfiguration->DMA_u8Stream].DMA_SxNDTR =
				DMA_pstrConfiguration->DMA_u16NumberOfData;

		DMA_REG->DMA_Sx[DMA_pstrConfiguration->DMA_u8Stream].DMA_SxPAR =
				(u32) DMA_pstrConfiguration->DMA_pvPeripheralAddress;

		DMA_REG->DMA_Sx[DMA_pstrConfiguration->DMA_u8Stream].DMA_SxM0AR =
				(u32) DMA_pstrConfiguration->DMA_pvMemoryAddress;

	}
	return LocErrorStatus;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
DMA_tenuErrorStatus DMA_enuState(void * DMA_pvNum, u8 DMA_u8Stream,
		u32 Copy_u32State)
{

	DMA_tenuErrorStatus LocErrorStatus = DMA_enuOk;

	if (DMA_pvNum == NULL)
	{
		LocErrorStatus = DMA_enuNullPointer;
	}
	else if (DMA_u8Stream > DMA_NUM_OF_STRAEMS)
	{
		LocErrorStatus = DMA_enuOutOfRange;
	}
	else
	{
		switch (Copy_u32State)
		{
		case DMA_ENABLE:
			DMA_S_REG->DMA_Sx[DMA_u8Stream].DMA_SxCR |= DMA_ENABLE;
			break;

		case DMA_DISABLE:

			DMA_S_REG->DMA_Sx[DMA_u8Stream].DMA_SxCR &= DMA_DISABLE;

			break;

		default:

			LocErrorStatus = DMA_enuNOk;

			break;

		}

	}

	return LocErrorStatus;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
DMA_tenuErrorStatus DMA_enuReadFlag(void * DMA_pvNum, u64 DMA_u64Flag,
		pu8 Add_pu8FlagState)
{

	DMA_tenuErrorStatus LocErrorStatus = DMA_enuOk;

	if (DMA_pvNum == NULL)
	{
		LocErrorStatus = DMA_enuNullPointer;
	}

	else
	{
		if (DMA_S_REG->DMA_LISR & (u32) DMA_u64Flag)
		{
			*Add_pu8FlagState = DMA_FLAG_HIGH;
		}
		else
		{
			*Add_pu8FlagState = DMA_FLAG_LOW;
		}

		if (DMA_S_REG->DMA_HISR & ((u32) (DMA_u64Flag >> 32)))
		{
			*Add_pu8FlagState = DMA_FLAG_HIGH;
		}
		else
		{
			*Add_pu8FlagState = DMA_FLAG_LOW;
		}
	}

	return LocErrorStatus;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Function:  DMA_enuInit
 * --------------------
 * Initialize DMA Peripheral To start Transmission Data from Source To Specific Destination.
 *
 *  DMA_pstrConfiguration: Address of DMA Configuration Struct Passing Specific Configurations
 *  To Initialize The DMA.
 *
 *  returns: The Error Status of Function "DMA_tenuErrorStatus"
 *           Check "DMA_tenuErrorStatus" Enum for the Error Types.
 */
DMA_tenuErrorStatus DMA_enuClearFlag(void * DMA_pvNum, u64 DMA_u64Flag)
{
	DMA_tenuErrorStatus LocErrorStatus = DMA_enuOk;
	if (DMA_pvNum == NULL)
	{
		LocErrorStatus = DMA_enuNullPointer;
	}
	else
	{
		DMA_S_REG->DMA_LIFCR |= (u32) DMA_u64Flag;
		DMA_S_REG->DMA_HIFCR |= (u32) (DMA_u64Flag >> 32);
	}

	return LocErrorStatus;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
