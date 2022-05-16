/*
 * rcc.c
 *
 *  Created on: Mar 9, 2022
 *      Author: MahmoudElImbabi
 */
////////////////////////////INCLUDING FILES .H//////////////////////////////////////

#include "../../../../GPIO/include/SERVCIES/Bit_utils.h"
#include "../../../../GPIO/include/SERVCIES/Std_types.h"
#include <MCAL/RCC/rcc.h>
////////////////////////////////////PINS////////////////////////////////////////////
#define RCC_REGISTER_BIT_0   0
#define RCC_REGISTER_BIT_1	1
#define RCC_REGISTER_BIT_2	2
#define RCC_REGISTER_BIT_3	3
#define RCC_REGISTER_BIT_4	4
#define RCC_REGISTER_BIT_5	5
#define RCC_REGISTER_BIT_6	6
#define RCC_REGISTER_BIT_7	7
#define RCC_REGISTER_BIT_8	8
#define RCC_REGISTER_BIT_9	9
#define RCC_REGISTER_BIT_10	10
#define RCC_REGISTER_BIT_11	11
#define RCC_REGISTER_BIT_12	12
#define RCC_REGISTER_BIT_13	13
#define RCC_REGISTER_BIT_14	14
#define RCC_REGISTER_BIT_15	15
#define RCC_REGISTER_BIT_16	16
#define RCC_REGISTER_BIT_17	17
#define RCC_REGISTER_BIT_18	18
#define RCC_REGISTER_BIT_19	19
#define RCC_REGISTER_BIT_20	20
#define RCC_REGISTER_BIT_21	21
#define RCC_REGISTER_BIT_22	22
#define RCC_REGISTER_BIT_23	23
#define RCC_REGISTER_BIT_24	24
#define RCC_REGISTER_BIT_25	25
#define RCC_REGISTER_BIT_26	26
#define RCC_REGISTER_BIT_27	27
#define RCC_REGISTER_BIT_28	28
#define RCC_REGISTER_BIT_29	29
#define RCC_REGISTER_BIT_30	30
#define RCC_REGISTER_BIT_31	31
#define RCC_REGISTER_BIT_32	32
/////////////////////////////*BIT MASK*////////////////////////////////////////////////
#define RCC_MASK_SWS  0x0000000C
#define RCC_MASK_SW   0x00000003
#define RCC_HSI_MSK	  0x00000000
#define RCC_HSE_MSK	  0x00000004
#define RCC_PLL_MSK	  0x00000008
/////////////////////////*RCC_REGISTER MAPPING*////////////////////////////////////////
typedef struct
{

	volatile u32 RCC_CR;
	volatile u32 RCC_PLLCFGR;
	volatile u32 RCC_CFGR;
	volatile u32 RCC_CIR;
	volatile u32 RCC_AHB1RSTR;
	volatile u32 RCC_AHB2RSTR;
	volatile u32 RCC_Reserved0[2];
	volatile u32 RCC_APB1RSTR;
	volatile u32 RCC_APB2RSTR;
	volatile u32 RCC_Reserved1[2];
	volatile u32 RCC_AHB1ENR;
	volatile u32 RCC_AHB2ENR;
	volatile u32 RCC_Reserved2[2];
	volatile u32 RCC_APB1ENR;
	volatile u32 RCC_APB2ENR;
	volatile u32 RCC_Reserved3[2];
	volatile u32 RCC_AHB1LPENR;
	volatile u32 RCC_AHB2LPENR;
	volatile u32 RCC_Reserved4[2];
	volatile u32 RCC_APB1LPENR;
	volatile u32 RCC_APB2LPENR;
	volatile u32 RCC_Reserved5[2];
	volatile u32 RCC_BDCR;
	volatile u32 RCC_CSR;
	volatile u32 RCC_Reserved6[2];
	volatile u32 RCC_SSCGR;
	volatile u32 RCC_PLLI2SCFGR;
	volatile u32 RCC_DCKCFGR;

} RCC_t;

#define RCC   ((RCC_t*)(0x40023800))
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////MACROS//////////////////////////////////////////////////
#define MAX_TIME    	    		10000
#define MAX_M						63
#define MAX_P						432

#define HSI_IS_ON					GET_BIT(RCC->RCC_CR, RCC_REGISTER_BIT_0)==1
#define HSE_IS_ON					GET_BIT(RCC->RCC_CR, RCC_REGISTER_BIT_16)==1
#define PLL_IS_ON					GET_BIT(RCC->RCC_CR, RCC_REGISTER_BIT_24)==1

#define PLL_SRC_HSI					GET_BIT(RCC->RCC_PLLCFGR, RCC_REGISTER_BIT_22)==0
#define PLL_SRC_HSE					GET_BIT(RCC->RCC_PLLCFGR, RCC_REGISTER_BIT_22)==1

#define HSI_READY					GET_BIT(RCC->RCC_CR,RCC_REGISTER_BIT_1)
#define HSE_READY					GET_BIT(RCC->RCC_CR,RCC_REGISTER_BIT_17)
#define PLL_READY					GET_BIT(RCC->RCC_CR,RCC_REGISTER_BIT_25)

#define WAIT_HSI_READY				GET_BIT(RCC->RCC_CR,RCC_REGISTER_BIT_1) == RCC_STATE_NOTREADY
#define WAIT_HSE_READY				GET_BIT(RCC->RCC_CR,RCC_REGISTER_BIT_17) == RCC_STATE_NOTREADY
#define WAIT_PLL_READY				GET_BIT(RCC->RCC_CR,RCC_REGISTER_BIT_25) == RCC_STATE_NOTREADY

#define SWITCH_SYSCLK_HSI			RCC->RCC_CFGR = RCC_SW_HSI
#define SWITCH_SYSCLK_HSE			RCC->RCC_CFGR = RCC_SW_HSE
#define SWITCH_SYSCLK_PLL			RCC->RCC_CFGR = RCC_SW_PLL

#define RCC_STATE_TURNON_HSI   		SET_BIT(RCC->RCC_CR, RCC_REGISTER_BIT_0)
#define RCC_STATE_TURNOFF_HSI	 	CLR_BIT(RCC->RCC_CR, RCC_REGISTER_BIT_0)
#define RCC_STATE_TURNON_HSE   		SET_BIT(RCC->RCC_CR, RCC_REGISTER_BIT_16)
#define RCC_STATE_TURNOFF_HSE	 	CLR_BIT(RCC->RCC_CR, RCC_REGISTER_BIT_16)
#define RCC_STATE_TURNON_PLL	 	SET_BIT(RCC->RCC_CR, RCC_REGISTER_BIT_24)
#define RCC_STATE_TURNOFF_PLL  		CLR_BIT(RCC->RCC_CR, RCC_REGISTER_BIT_24)

#define RCC_SW_HSI 					((RCC->RCC_CFGR & ~(RCC_MASK_SW)) | RCC_CLK_HSI)
#define RCC_SW_HSE 					((RCC->RCC_CFGR & ~(RCC_MASK_SW)) | RCC_CLK_HSE)
#define RCC_SW_PLL 					((RCC->RCC_CFGR & ~(RCC_MASK_SW)) | RCC_CLK_PLL)
#define RCC_SWS_STATUS 				(((RCC->RCC_CFGR) & (RCC_MASK_SWS)) >> 2)

#define RCC_CNFGR_PLLM			    ((COPY_u8M))
#define RCC_CNFGR_PLLN				((Copy_u16N) << RCC_REGISTER_BIT_6)
#define RCC_CNFGR_PLLP				((Copy_u8P) << RCC_REGISTER_BIT_16)
#define RCC_CNFGR_SRCPLL			((Copy_u8SrcPll) << RCC_REGISTER_BIT_22)
#define RCC_CNFGR_Q					((Copy_u8Q) << RCC_REGISTER_BIT_24)

#define RCC_CNFGR_PLL      			RCC_CNFGR_PLLM | RCC_CNFGR_PLLN | RCC_CNFGR_PLLP | RCC_CNFGR_SRCPLL | RCC_CNFGR_Q
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Rcc_tenuErrorStatus Rcc_enuSelectSysCLk(u8 Copy_u8NewSysClk,
		u8 Copy_u8OldSysClk)
{

	/******************************************VARIABLES*************************************************/
	Rcc_tenuErrorStatus Error_Status = Rcc_enuOk;
	u8 LOC_CurrentSysClk = ((RCC->RCC_CFGR) & (RCC_MASK_SWS));
	u16 LOC_ITER = 0;
	/***************************************************************************************************/

	/*******************************MAKING THE SELECTED  CLK AS  SYSCLK*********************************/
	switch (Copy_u8NewSysClk)
	{
	case RCC_CLK_HSI:

		RCC_STATE_TURNON_HSI;

		while (WAIT_HSI_READY)
		{
			LOC_ITER++;
						if (LOC_ITER == MAX_TIME)
						{
							Error_Status = Rcc_enuTimeOutError;

							break;
						}

		}

		SWITCH_SYSCLK_HSI;

		while ( RCC_SWS_STATUS != RCC_CLK_HSI) //WAIT UNTIL SWS BE IN HSI STATUS
		{
			LOC_ITER++;
			if (LOC_ITER == MAX_TIME)
			{
				Error_Status = Rcc_enuTimeOutError;
				break;
			}
		}

		break;
		/***********************************************************************************************/
	case RCC_CLK_HSE:

		RCC_STATE_TURNON_HSE;

		while (WAIT_HSE_READY)
		{
			LOC_ITER++;
						if (LOC_ITER == MAX_TIME)
						{
							Error_Status = Rcc_enuTimeOutError;
							break;
						}
		}

		SWITCH_SYSCLK_HSE;

		while ( RCC_SWS_STATUS != RCC_CLK_HSE) //WAIT UNTIL SWS BE IN HSE STATUS
		{
			LOC_ITER++;
			if (LOC_ITER == MAX_TIME)
			{
				Error_Status = Rcc_enuTimeOutError;
				break;
			}
		}

		break;
		/***********************************************************************************************/
	case RCC_CLK_PLL:

		RCC_STATE_TURNON_PLL;

		while (WAIT_PLL_READY)
		{
			LOC_ITER++;
						if (LOC_ITER == MAX_TIME)
						{
							Error_Status = Rcc_enuTimeOutError;
							break;
						}
		}

		SWITCH_SYSCLK_PLL;

		while ( RCC_SWS_STATUS != RCC_CLK_PLL) //WAIT UNTIL SWS BE IN PLL STATUS
		{
			LOC_ITER++;
			if (LOC_ITER >= MAX_TIME)
			{
				Error_Status = Rcc_enuTimeOutError;
				break;
			}
		}

		break;
		/***********************************************************************************************/
	default:

		break;
	}
	/***************************TURNNIG OFF THE OLD SYSCLK OR NOT**************************/

	if (Copy_u8OldSysClk == RCC_STATE_NOTPRESERVE)
	{
		switch (LOC_CurrentSysClk)
		{
		case RCC_HSI_MSK:

			RCC_STATE_TURNOFF_HSI;

			break;
			/***********************************************************************************************/
		case RCC_HSE_MSK:

			RCC_STATE_TURNOFF_HSE;

			break;
			/***********************************************************************************************/
		case RCC_PLL_MSK:

			RCC_STATE_TURNOFF_PLL;

			break;
			/***********************************************************************************************/
		default:

			break;
		}

	}

	return Error_Status;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

Rcc_tenuErrorStatus Rcc_enuControlCLk(u8 Copy_u8ControlHSI,
		u8 Copy_u8ControlHSE, u8 Copy_u8ControlPLL)
{

////////////////////////////////////////VARIABLES///////////////////////////////////////////////////
	Rcc_tenuErrorStatus Error_Status = Rcc_enuOk;
	/**************************************************************************************************/

	switch (Copy_u8ControlHSI)
	{

	case RCC_CNTROL_HSION:

		if (HSI_IS_ON)
		{

			Error_Status = RCC_enuUnLogicActionError;
		}

		else
		{
			RCC_STATE_TURNON_HSI;
		}
		break;
		/***********************************************************************************************/
	case RCC_CNTROL_HSIOFF:

		if (RCC_SWS_STATUS == RCC_CLK_HSI)
		{

			Error_Status = RCC_enuUnLogicActionError;

		}

		else if (PLL_SRC_HSI)
		{

			Error_Status = RCC_enuUnLogicActionError;

		}
		else
		{

			RCC_STATE_TURNOFF_HSI;
		}

		break;

	}
	/***********************************************************************************************/
	switch (Copy_u8ControlHSE)
	{

	case RCC_CNTROL_HSEON:

		if (HSE_IS_ON)
		{

			Error_Status = RCC_enuUnLogicActionError;
		}

		else
		{
			RCC_STATE_TURNON_HSE;
		}
		break;
		/***********************************************************************************************/
	case RCC_CNTROL_HSEOFF:

		if (RCC_SWS_STATUS == RCC_CLK_HSE)
		{

			Error_Status = RCC_enuUnLogicActionError;

		}
		else if (PLL_SRC_HSE)
		{

			Error_Status = RCC_enuUnLogicActionError;

		}
		else
		{

			RCC_STATE_TURNOFF_HSE;
		}

		break;

	}
	/***********************************************************************************************/
	switch (Copy_u8ControlPLL)
	{

	case RCC_CNTROL_PLLON:

		if (PLL_IS_ON)
		{

			Error_Status = RCC_enuUnLogicActionError;
		}

		else
		{
			RCC_STATE_TURNON_PLL;
		}
		break;
		/***********************************************************************************************/
	case RCC_CNTROL_PLLOFF:

		if (RCC_SWS_STATUS == RCC_CLK_PLL)
		{

			Error_Status = RCC_enuUnLogicActionError;

		}
		else
		{

			RCC_STATE_TURNOFF_PLL;
		}

		break;

	}
	/***********************************************************************************************/
	return Error_Status;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
Rcc_tenuErrorStatus Rcc_enuCheckCLk(u8 Copy_u8CLk, pu8 Add_pu8RdyStatus)
{
/////////////////////////////////VARIABLE////////////////////////////////////////////////////////
	Rcc_tenuErrorStatus Error_Status = Rcc_enuOk;
	/***********************************************************************************************/
	if (Add_pu8RdyStatus == NULL)
	{

		Error_Status = RCC_enuNullPointerError;

	}
	/***********************************************************************************************/
	else
	{

		switch (Copy_u8CLk)
		{

		case RCC_CLK_HSI:
			if (HSI_READY)
			{
				*Add_pu8RdyStatus = RCC_STATE_READY;
			}
			else
			{
				*Add_pu8RdyStatus = RCC_STATE_NOTREADY;
			}

			break;
			/***********************************************************************************************/
		case RCC_CLK_HSE:
			if (HSE_READY)
			{
				*Add_pu8RdyStatus = RCC_STATE_READY;
			}
			else
			{
				*Add_pu8RdyStatus = RCC_STATE_NOTREADY;
			}
			break;

			/***********************************************************************************************/
		case RCC_CLK_PLL:

			if (PLL_READY)
			{

				*Add_pu8RdyStatus = RCC_STATE_READY;

			}
			else
			{

				*Add_pu8RdyStatus = RCC_STATE_NOTREADY;

			}

			break;

			/***********************************************************************************************/
		}

	}

	return Error_Status;

}
///////////////////////////////////////////////////////////////////////////////////////////////////
Rcc_tenuErrorStatus Rcc_enuCnfgrPll(u8 COPY_u8M, u16 Copy_u16N, u8 Copy_u8P,
		u8 Copy_u8SrcPll, u8 Copy_u8Q)
{
//////////////////////////////////////VARIABLES/////////////////////////////////////////////////
	Rcc_tenuErrorStatus Error_Status = Rcc_enuOk;
	/***********************************************************************************************/
	if (PLL_IS_ON)
	{

		Error_Status = RCC_enuPll_IS_On_Error;

	}
	/***********************************************************************************************/
	else if (COPY_u8M > MAX_M)
	{

		Error_Status = RCC_enuInvalid_M_Error;

	}
	/***********************************************************************************************/
	else if (Copy_u16N > MAX_P)
	{

		Error_Status = RCC_enuInvalid_N_Error;

	}
	/***********************************************************************************************/
	else if (Copy_u8P > RCC_PLLP_DIVBY_8)
	{

		Error_Status = RCC_enuInvalid_P_Error;

	}
	/***********************************************************************************************/
	else if (Copy_u8SrcPll > RCC_PLLSRC_HSE)
	{

		Error_Status = RCC_enuInvalid_ScrPll_Error;
	}
	/***********************************************************************************************/
	else if (Copy_u8Q > RCC_Q_DIVBY_15)
	{

		Error_Status = RCC_enuInvalid_Q_Error;

	}
	/***********************************************************************************************/
	else
	{

		RCC->RCC_PLLCFGR = RCC_CNFGR_PLL;

	}

	return Error_Status;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////
Rcc_tenuErrorStatus Rcc_enuEnablePeriphral(u8 Copy_u8PeriphralBus,
		u32 Copy_u32Periphral)
{
///////////////////////////////VARIABLES/////////////////////////////////////////////////
	Rcc_tenuErrorStatus Error_Status = Rcc_enuOk;
	/***************************************************************************************/
	switch (Copy_u8PeriphralBus)
	{
	case RCC_REGISTER_AHB1:
		RCC->RCC_AHB1ENR |= Copy_u32Periphral;
		break;
		/***************************************************************************************/
	case RCC_REGISTER_AHB2:
		RCC->RCC_AHB2ENR |= Copy_u32Periphral;
		break;
		/***************************************************************************************/
	case RCC_REGISTER_APB1:
		RCC->RCC_APB1ENR |= Copy_u32Periphral;
		break;
		/***************************************************************************************/
	case RCC_REGISTER_APB2:
		RCC->RCC_APB2ENR |= Copy_u32Periphral;
		break;
		/***************************************************************************************/
	default:
		Error_Status = Rcc_enuWrongPeriphralBusError;
		break;
	}

	return Error_Status;

}
/////////////////////////////////////////////////////////////////////////////////////////////
Rcc_tenuErrorStatus Rcc_enuDisablePeriphral(u8 Copy_u8PeriphralBus,
		u32 Copy_u32Periphral)
{
///////////////////////////////VARIABLES/////////////////////////////////////////////////
	Rcc_tenuErrorStatus Error_Status = Rcc_enuOk;
	/***************************************************************************************/
	switch (Copy_u8PeriphralBus)
	{
	case RCC_REGISTER_AHB1:
		RCC->RCC_AHB1ENR &= ~Copy_u32Periphral;
		break;
		/***************************************************************************************/
	case RCC_REGISTER_AHB2:
		RCC->RCC_AHB2ENR &= ~Copy_u32Periphral;
		break;
		/***************************************************************************************/
	case RCC_REGISTER_APB1:
		RCC->RCC_APB1ENR &= ~Copy_u32Periphral;
		break;
		/***************************************************************************************/
	case RCC_REGISTER_APB2:
		RCC->RCC_APB2ENR &= ~Copy_u32Periphral;
		break;
		/***************************************************************************************/
	default:
		Error_Status = Rcc_enuWrongPeriphralBusError;
		break;
	}

	return Error_Status;

}
////////////////////////////////////////////////////////////////////////////////////////////////////
Rcc_tenuErrorStatus Rcc_enuCfgBusPrescalers(u32 Copy_u32Prescaler)
{
/////////////////////////////////////VARIABLE//////////////////////////////////////////////////////
	Rcc_tenuErrorStatus Error_Status = Rcc_enuOk;
	/***************************************************************************************/
	switch (Copy_u32Prescaler)
	{
	case RCC_APB2_NOTDIV:
		RCC->RCC_CFGR &= RCC_APB2_NOTDIV;
		break;

	case RCC_APB2_DIV2:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_APB2_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_APB2_DIV4:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_APB2_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_APB2_DIV8:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_APB2_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_APB2_DIV16:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_APB2_NOTDIV) | Copy_u32Prescaler;
		break;

		/*********************************************************************************************/

	case RCC_APB1_NOTDIV:
		RCC->RCC_CFGR &= RCC_APB1_NOTDIV;
		break;

	case RCC_APB1_DIV2:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_APB1_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_APB1_DIV4:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_APB1_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_APB1_DIV8:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_APB1_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_APB1_DIV16:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_APB1_NOTDIV) | Copy_u32Prescaler;
		break;

		/****************************************************************************************/

	case RCC_AHB_NOTDIV:
		RCC->RCC_CFGR &= RCC_AHB_NOTDIV;
		break;

	case RCC_AHB_DIV2:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_AHB_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_AHB_DIV4:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_AHB_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_AHB_DIV8:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_AHB_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_AHB_DIV16:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_AHB_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_AHB_DIV64:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_AHB_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_AHB_DIV128:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_AHB_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_AHB_DIV256:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_AHB_NOTDIV) | Copy_u32Prescaler;
		break;

	case RCC_AHB_DIV512:
		RCC->RCC_CFGR = (RCC->RCC_CFGR & RCC_AHB_NOTDIV) | Copy_u32Prescaler;
		break;

		/****************************************************************************************/

	default:
		Error_Status = Rcc_enuWrongBusPrescalerError;
		break;

	}

	return Error_Status;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

