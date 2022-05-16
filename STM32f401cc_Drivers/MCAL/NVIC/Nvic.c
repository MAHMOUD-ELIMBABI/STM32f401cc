/*
 * Nvic.c
 *
 *  Created on: Apr 18, 2022
 *      Author: MahmoudElImbabi
 */

#include "SERVCIES/Std_types.h"
#include "SERVCIES/Bit_utils.h"
#include <MCAL/NVIC/Nvic.h>
////////////////////////////////////////////////////////////////////////////////////
typedef struct{
	volatile u32 NVIC_ISERx[8];
	volatile u32 NVIC_ICERx[8];
	volatile u32 NVIC_ISPRx[8];
	volatile u32 NVIC_ICPRx[8];
	volatile u32 NVIC_IABRx[8];
	volatile u8  NVIC_IPRx[240];



}NVIC_Reg_t;


#define NVIC  ((NVIC_Reg_t *)(BASEADDRESS))

#define AIRCR *((volatile u32 *) (0xE000ED0C))
///////////////////////////////////////////////////////////////////////
Nvic_tenuErrorStatus NVIC_EnableIRQ(NVIC_IRQn_t IRQn)
{
	Nvic_tenuErrorStatus ErrorStatus = Nvic_enuOk;
	u8 Loc_u8BitNum = IRQn % 32;
	u8 Loc_u8RegisterNum = IRQn / 32;

	if(IRQn>=240)
	{
		ErrorStatus=Nvic_enuOutOfRange;
	}
	else
	{
		NVIC->NVIC_ISERx[Loc_u8RegisterNum] = 1<<Loc_u8BitNum;
	}


	return ErrorStatus;
}
/////////////////////////////////////////////////////////////////////////
Nvic_tenuErrorStatus NVIC_DisableIRQ(NVIC_IRQn_t IRQn)
{
	Nvic_tenuErrorStatus ErrorStatus = Nvic_enuOk;
	u8 Loc_u8BitNum = IRQn % 32;
	u8 Loc_u8RegisterNum = IRQn / 32;

	if(IRQn>=240)
	{
		ErrorStatus=Nvic_enuOutOfRange;
	}
	else
	{
		NVIC->NVIC_ICERx[Loc_u8RegisterNum] = 1<<Loc_u8BitNum;
	}

	return ErrorStatus;

}
/////////////////////////////////////////////////////////////////////////////
Nvic_tenuErrorStatus NVIC_SetPendingIRQ(NVIC_IRQn_t IRQn)
{
	Nvic_tenuErrorStatus ErrorStatus = Nvic_enuOk;
	u8 Loc_u8BitNum = IRQn % 32;
	u8 Loc_u8RegisterNum = IRQn / 32;

	if(IRQn>=240)
	{
		ErrorStatus=Nvic_enuOutOfRange;
	}
	else
	{
		NVIC->NVIC_ISPRx[Loc_u8RegisterNum] = 1<<Loc_u8BitNum;
	}

	return ErrorStatus;
}
//////////////////////////////////////////////////////////////////////////////
Nvic_tenuErrorStatus NVIC_ClearPendingIRQ(NVIC_IRQn_t IRQn)
{
	Nvic_tenuErrorStatus ErrorStatus = Nvic_enuOk;
	u8 Loc_u8BitNum = IRQn % 32;
	u8 Loc_u8RegisterNum = IRQn / 32;

	if(IRQn>=240)
	{
		ErrorStatus=Nvic_enuOutOfRange;
	}
	else
	{
		NVIC->NVIC_ICPRx[Loc_u8RegisterNum] = 1<<Loc_u8BitNum;
	}

	return ErrorStatus;
}
//////////////////////////////////////////////////////////////////////////////////
Nvic_tenuErrorStatus NVIC_GetPendingIRQ(NVIC_IRQn_t IRQn , pu8 Add_pu8Pending)
{
	Nvic_tenuErrorStatus ErrorStatus = Nvic_enuOk;
	u8 Loc_u8BitNum = IRQn % 32;
	u8 Loc_u8RegisterNum = IRQn / 32;

	if(IRQn>=240)
	{
		ErrorStatus=Nvic_enuOutOfRange;
	}

	else
	{
	*Add_pu8Pending = GET_BIT(NVIC->NVIC_ICPRx[Loc_u8RegisterNum] , Loc_u8BitNum);
	}

	return ErrorStatus;

}
///////////////////////////////////////////////////////////////////////////////////
Nvic_tenuErrorStatus NVIC_GetActiveIRQ(NVIC_IRQn_t IRQn , pu8 Add_pu8Pending)
{
	Nvic_tenuErrorStatus ErrorStatus = Nvic_enuOk;
	u8 Loc_u8BitNum = IRQn % 32;
	u8 Loc_u8RegisterNum = IRQn / 32;
	if(IRQn>=240)
	{
		ErrorStatus=Nvic_enuOutOfRange;
	}
	else
	{
	*Add_pu8Pending = GET_BIT(NVIC->NVIC_IABRx[Loc_u8RegisterNum] , Loc_u8BitNum);
	}

	return ErrorStatus;

}
///////////////////////////////////////////////////////////////////////////////////
Nvic_tenuErrorStatus NVIC_SetPriority(NVIC_IRQn_t IRQn, u8 priority)
{
	Nvic_tenuErrorStatus ErrorStatus =Nvic_enuOk;

	if(IRQn>=240)
		{
			ErrorStatus=Nvic_enuOutOfRange;
		}
	else if(priority & NVIC_PRIORITY_VALIDATION_MASK){

		ErrorStatus=Nvic_enuOutOfRange;

	}

	NVIC->NVIC_IPRx[IRQn] = priority;

return ErrorStatus;

}
////////////////////////////////////////////////////////////////////////////////////
Nvic_tenuErrorStatus NVIC_enuSetSubGroup(u32 Copy_u32SubGroup)
{
	Nvic_tenuErrorStatus ErrorStatus =Nvic_enuOk;
	u32 Loc_Temp;

	if(Copy_u32SubGroup & NVIC_SUBGROUP_CLEAR_MASK){

	ErrorStatus =Nvic_enuOutOfRange;


}
	Loc_Temp= AIRCR ;

	Loc_Temp|= NVIC_VICTKEYSTATE << 16;

	Loc_Temp&= NVIC_SUBGROUP_CLEAR_MASK ;

	Loc_Temp |= Copy_u32SubGroup;

	AIRCR = Loc_Temp ;




	return ErrorStatus;
}
/////////////////////////////////////////////////////////////////////////////////////
/*u32 NVIC_u8GetEncodedPeriority(u8 Copy_u8NumberOfSubGroup , u8 Copy_u8PreemptivePeriority , u8 Copy_u8SubGroupPeriority){

	u32 Loc_u8Periority = 0;
		if(Copy_u8NumberOfSubGroup > 4)
		{
			//Error
		}
		else
		{
			//0b1100 0000		//(0b00001111 << (Copy_u8NumberOfSubGroup + 4)
			//0b0101 0000		// Periority
			Loc_u8Periority |= Copy_u8SubGroupPeriority<<4;
			if(Loc_u8Periority & (0b00001111 << (Copy_u8NumberOfSubGroup + 4) ))
			{
				//ERROR
			}
			else{
				//	0b0101 0000     &      0b1000 0000
				if(Loc_u8Periority & (Copy_u8PreemptivePeriority << (Copy_u8NumberOfSubGroup + 4)))
				{
					//ERROR
				}
				else
				{
					Loc_u8Periority |= Copy_u8PreemptivePeriority << (Copy_u8NumberOfSubGroup + 4);
					if(Loc_u8Periority & 0xFFFFFF00)
					{
						//ERROR
					}
				}
			}

		}

		return Loc_u8Periority;

		//0b1100 0000
}*/
///////////////////////////////////////////////////////////////////////////////////////
