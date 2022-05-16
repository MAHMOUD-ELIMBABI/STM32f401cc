/*
 * Systick.c
 *
 *  Created on: Apr 13, 2022
 *      Author: MahmoudElImbabi
 */

#include"../../include/SERVCIES/Std_types.h"
#include <MCAL/SYSTICK/Systick.h>
#include <MCAL/SYSTICK/Systick_Cfg.h>
///////////////////////////////////////////////////////////////////////////////
/*MAPPING THE SYSTICK REGISTERS INTO TYPEDEF STRUCT */
typedef struct{
	/*STK_CTRL REGISTER*/
	volatile u32 STK_CTRL;
	/*STK_LOAD  REGISTER*/
	volatile u32 STK_LOAD;
	/*STK_VAL REGISTER*/
	volatile u32 STK_VAL;
	/*STK_CALIB REGISTER*/
	volatile u32 STK_CALIB;

}SYSTICK_t; //NAME OF THE TYPEDEF STRUCT
//////////////////////////////////////////////////////////////////////////////////
void SysTick_Handler (void); //THE ISR FUNCTION PROTOTYPE
//////////////////////////////////////////////////////////////////////////////////
/*MACRO OF SYSTICK  BASE ADDRESS*/
#define  SYSTICK_BASEADDRESS   			0xE000E010
/*MACRO TO ACCSSES THE REGISTERS IN THE TYPEDEF STRUCT  SYSTICK_t */
#define  SYSTICK					((SYSTICK_t*)(SYSTICK_BASEADDRESS))
//////////////////////////////////////////////////////////////////////////////////
Systick_Cbf SystickCallback;
///////////////////////////////////////////////////////////////////////////////////
/*IMPLEMENT THE Systick_Init FUNCTION WITH ONE PARAMETER THE VALUE OF TIME PER Ms AND WITH ERROR STATUS RETURN */
extern Systick_tenuErrorStatus Systick_Init(u32 Copy_TimeMs){
	/*INTIALIZATION OF LOCAL VARIABLE WHICH CARRY THE ERROR STATUS VALUE EQUAL Systick_enuOk  */
	Systick_tenuErrorStatus  Loc_enuErrorStatus= Systick_enuOk;
		/*CHECK IF THE  SYSTICK_PRESCALER IS DIVIDED BY EIGHT OR NOT*/
#if		 SYSTICK_PRESCALER ==  SYSTICK_PRESCALER_DIV8
	/*INITALIZING LOCAL VARIABLE LocStepCounter WHICH CARRY THE SYSTICK CLK FREQUENCY DIVIDED BY 8000 DUE TO PRESCALER */
	u32 LocStepCounter = (SYSTICK_AHB_FREQ_CLK/8000) ;

#elif
	/*INITALIZING LOCAL VARIABLE LocStepCounter WHICH CARRY THE SYSTICK CLK FREQUENCY DIVIDED BY 1000  */
	u32 LocStepCounter = SYSTICK_AHB_FREQ_CLK/1000 ;

#endif
	/*THE LOCAL VALUE WHICH IS EQUAL THE NUMBER OF STEPS TO REACH 1Ms MULTIPLY WITH THE Copy_TimeMs FROM THE USER */
	LocStepCounter *= Copy_TimeMs;
	/*CHECK IF THE NUMBER OF THE RELOAD VALUE IS OUT OF RANGE OR NOT BY ANDING IT WITH THE VALIDATE MASK*/
	if(LocStepCounter & VALIDATE_RELOADVALUE){
		/*OVERWRITE Systick_enuErrorTimeMs IN THE ERROR STATUS  LOCAL VARIABLE Loc_enuErrorStatus*/
		Loc_enuErrorStatus = Systick_enuErrorTimeMs;

	}

	else{
		/*ACCESSING THE STK_LOAD REGISTER AND LOAD THE VALUE OF LocStepCounter IN IT*/
		SYSTICK->STK_LOAD = LocStepCounter;
		/*INTIALIZING THE STK_VAL REGISTER WITH ZERO*/
		SYSTICK->STK_VAL = 0;
		/*ACCESSING THE STK_CTRL REGISTER AND FROM CHOOSE CLOCK SOURCE SELECTION OPTION AND SET THE INTERRUPT BIT WITH
		 * ZERO IN THEPRESCALER MASK*/
		SYSTICK->STK_CTRL = SYSTICK_PRESCALER;

	}
/*RETURN THE ERROR STATUS OF THE FUNCTION Systick_Init*/
	return Loc_enuErrorStatus ;


}
////////////////////////////////////////////////////////////////////////////////////////
/*IMPLEMETING THE Systick_Start FUNCTION*/
extern void Systick_Start(void){
	/*ORING THE  SYSTICK_ENABLE MASK WITH THE STK_CTRL REGISTER TO ENABLE THE SYSTICK TIMER*/
	SYSTICK->STK_CTRL |= SYSTICK_ENABLE;

}
////////////////////////////////////////////////////////////////////////////////////////
/*IMPLEMENTING THE Systick_Stop FUNCTION*/
extern void Systick_Stop(void){
	/*ANDING THE STK_CTRL REGISTER WITH ~1 TO DIABLE THE SYSTICL WITHOUT CHANGING THE  OTHER VALUES OF THE REGISTER*/
	SYSTICK->STK_CTRL &= ~1;

}
/////////////////////////////////////////////////////////////////////////////////////////
/*IMPLEMENTING THE Systick_RegisterCbf CALLBACK FUNCTION WITH INPUT PARAMTER ADDRESS WITH TYPE POINTER TO FUNCTION CALLED
 * Systick_Cbf */
extern Systick_tenuErrorStatus Systick_RegisterCbf(Systick_Cbf Add_pfCbf)
{
	/*INTIALIZING A LOCAL VARIABLE Loc_enuErrorStatus WITH VALUE  Systick_enuOk WHICH REFER TO THE ERROR STATUS */
	Systick_tenuErrorStatus  Loc_enuErrorStatus= Systick_enuOk;
	/*CHECK IF THE INPUT PARAMTER IS EQUAL NULL OR NOT*/
	if(Add_pfCbf == NULL)
	{
		/*OVERWRITE THE LOCAL VARIABLE Loc_enuErrorStatus WITH Systick_enuErrorNullPointer WHICH REFER TO THE ERROR STATUS*/
		Loc_enuErrorStatus = Systick_enuErrorNullPointer;
	}
	else
	{
		/*SAVEING THE ADDRESS FROM THE INPUT PARMTER INTO THE SystickCallback  VARIABLE*/
		SystickCallback = Add_pfCbf;
	}

	/*RETURN THE ERROR STATUS FOR THE FUNCTION Systick_RegisterCbf*/
	return Loc_enuErrorStatus;
}
/////////////////////////////////////////////////////////////////////////////////////////
/*IMPLEMENT THE ISR FUNCTION WHICH CALLED SysTick_Handler*/
void SysTick_Handler (void)
{
	/*CHECK IF THE GLOBAL VARIABLE SystickCallback IS NOT EQUAL NULL */
	if(SystickCallback != NULL)
	{
		/*CALL THE SystickCallback FUNCTION WHICH IS THE ADDRRES FOR THE USER FUNCTION*/
		SystickCallback();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
