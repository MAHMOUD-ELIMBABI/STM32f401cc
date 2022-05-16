/*
 * HSched.c
 *
 *  Created on: Apr 13, 2022
 *      Author: MahmoudElImbabi
 */
#include<SERVCIES/Std_types.h>
#include<MCAL/SYSTICK/Systick.h>
#include<HAL/HSched/HSched.h>
#include<HAL/HSched/HSched_Cfg.h>
/////////////////////////////////////////////////////////////////////////////////////
/*CREATE STATIC ARRAY OF POINTERS OF TYPE Sched_Cfg_t AND THE INDEX OF THE ARRAY IS THE NUMBER OF TASKS*/
static  Sched_Cfg_t* RunnableCfgs[HSCHED_NUM_OF_RUNNABLES];  /*STORE ADDRESSES*/
/*PROTOTYPE FOR THE Sched_Tasks FUNCTIONS */
static  void Sched_Tasks(void);
/////////////////////////////////////////////////////////////////////////////////////
/*IMPLEMENTING THE Sched_vidInit FUNCTION */
void Sched_vidInit(void){
	/*CALLING THE Systick_Init FUNCTION WITH INPUT HSCHED_TIME_MS */
	Systick_Init(HSCHED_TIME_MS);
	/*CALLING THE Systick_RegisterCbf FUNCTION  WITH INPUT Sched_Tasks WHICH WILL BE CALLED EVERY HSCHED_TIME_MS*/
	Systick_RegisterCbf(Sched_Tasks);
}
//////////////////////////////////////////////////////////////////////////////////////
/*IMPLEMENTING THE Sched_RegisterRunnable FUNCTION WITH INPUT PARAMTER POINTER OF Sched_Cfg_t WHICH IS AN ADDRESS */
HSched_tenu_ErrorStatus Sched_RegisterRunnable(Sched_Cfg_t * Add_pstrConfig){
	/*INTIALIZING A LOCAL VARIABLE OF TYPE HSched_tenu_ErrorStatus  WITH VALUE Sched_enuOk */
	HSched_tenu_ErrorStatus  LocenuErrorStatus= Sched_enuOk;
/*CHECK IF THE INPUT PARAMTER IS EQUAL NULL*/
if(Add_pstrConfig==NULL ){//THIS MEANS THAT YOU HAVE NO ADDRESS SO THIS IS AN ERROR
	  /*OVERWRITE THE LOCAL VARIABLE  WITH Sched_enuNullPointer*/
	  LocenuErrorStatus= Sched_enuNullPointer;
}
/*CHECK IF THE ADDRESS IN THE RunnableCfgs ARRAY IN INDEX IS NOT EQUAL NULL THEN THIS MEANS THAT
 *  WE HAVE ANOTHER ADDRESS IN THIS INDEXIN THE ARRAY SO THIS IA AN ERROR */
else if( RunnableCfgs[Add_pstrConfig->Priority]!= NULL){
/*OVERWRITE THE LOCAL VARIABLE WITH Sched_enuPriorityError */
	LocenuErrorStatus= Sched_enuPriorityError;

}
/*CHECK IF THE ADDRESS IN THE RunnableCfgs ARRAY IN INDEX IS EQUAL NULL THEN WE HAVE EMPTY SPACE SO WE CAN
 * ADD THE ADDRESS IN THIS INDEX OF THE ARRAY */
else if (RunnableCfgs[Add_pstrConfig->Priority]==NULL){

		/*ADDING THE ADDRESS Add_pstrConfig IN THE INDEX Add_pstrConfig->Priority OF THE ARRAY */
		RunnableCfgs[Add_pstrConfig->Priority]=Add_pstrConfig;

	}
/*RETURNING THE ERROR STATUS FOR THE Sched_RegisterRunnable FUNCTION */
return LocenuErrorStatus ;

}
//////////////////////////////////////////////////////////////////////////////////////
/*IMPLEMENTING THE  Sched_Start FUNCTION*/
void Sched_Start(void)
{
	/*CALLLING THE Systick_Start FUNCTION TO START THE SCHED*/
	Systick_Start();
	/*MAKE WHILE ONE LOOP TO LET THE SCHED WORKING FOREVER*/
	while(1);
}
//////////////////////////////////////////////////////////////////////////////////////
/*IMPLEMENTING THE STATIC Sched_Tasks FUNCTIONS*/
static void Sched_Tasks(void){
	/*DECLARING LOCAL VARIABLE CALLED Loc_u16Iter TO STORE THE ITERATOR OF THE FOR LOOP */
	u16 Loc_u16Iter;
	/*DECLARING THE STATIC LOCAL VARIABLE CALLED Loc_u32TimeMs FOR STORING THE VALUE OF HSCHED_TIME_MS AFTER INCREMENTING IT */
	static u32 Loc_u32TimeMs;
	/*INCREMENTING THE VALUE OF HSCHED_TIME_MS AND STORE IT IN THE LOCAL VARIABLE Loc_u32TimeMs */
	Loc_u32TimeMs+=HSCHED_TIME_MS;
		/*FOR LOOP TO LOOP ON THE TASKS IN THE RunnableCfgs ARRAY*/
		for(Loc_u16Iter = 0 ; Loc_u16Iter < HSCHED_NUM_OF_RUNNABLES ; Loc_u16Iter++)
		{
			/*CHECK IF THE INDEX Loc_u16Iter IN THE RunnableCfgs ARRAY IS NOT EQUAL NULL */
			if(RunnableCfgs[Loc_u16Iter] != NULL)
			{
				/*CHECK IF THE Loc_u32TimeMs MODULUS THE VALUE OF THE CyclicTimeMs IN THE INDEX Loc_u16Iter IN
				 * THE RunnableCfgs ARRAY  IS EQUAL ZERO*/
				if(Loc_u32TimeMs % RunnableCfgs[Loc_u16Iter] -> CyclicTimeMs == 0)
				{
					/*CALLING THE CallbackFunc FUNCTION IN THE  INDEX  Loc_u16Iter IN THE RunnableCfgs ARRAY*/
					(RunnableCfgs[Loc_u16Iter] -> CallbackFunc)();
				}
			}

		}

}
//////////////////////////////////////////////////////////////////////////////////////
