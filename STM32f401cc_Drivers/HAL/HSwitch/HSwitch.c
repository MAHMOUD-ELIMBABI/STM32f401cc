/*
 * HSwitch.c
 *
 *  Created on: Apr 14, 2022
 *      Author: MahmoudElImbabi
 */

#include<SERVCIES/Std_types.h>
#include<HAL/HSwitch/HSwitch.h>
#include<HAL/HSwitch/HSwitch_Cfg.h>
#include<MCAL/GPIO/Gpio.h>
#include<MCAL/RCC/rcc.h>
///////////////////////////////////////////////////////////////////////////////////
static u8 Sw_State[NumOfSwitch];
///////////////////////////////////////////////////////////////////////////////////
extern void HSwitch_vidInit(void){

u16 LocSwIteration =0;
u32 Rcc_Cfg = 0;

	for (LocSwIteration = 0; LocSwIteration < NumOfSwitch; LocSwIteration++)
	{

		switch (SwitchConfigurations[LocSwIteration].HSwitch_Port)
		{

		case GPIO_PORT_A:

			Rcc_Cfg |= AHB1_GPIOA_EN;

			break;

		case GPIO_PORT_B:

			Rcc_Cfg |= AHB1_GPIOB_EN;

			break;

		case GPIO_PORT_C:

			Rcc_Cfg |= AHB1_GPIOC_EN;

			break;

		case GPIO_PORT_D:

			Rcc_Cfg |= AHB1_GPIOD_EN;

			break;
		case GPIO_PORT_E:

			Rcc_Cfg |= AHB1_GPIOE_EN;

			break;
		case GPIO_PORT_H:

			Rcc_Cfg |= AHB1_GPIOH_EN;

			break;

		}

	}

	Rcc_enuEnablePeriphral( RCC_REGISTER_AHB1, Rcc_Cfg);

	GpioPinCfg_t Cfg =
	{

	.gpio_mode_x = GPIO_MODE_u64_INPUT,
	.gpio_speed_x = GPIO_SPEED_VHIGH,

	};

	for (LocSwIteration = 0; LocSwIteration < NumOfSwitch; LocSwIteration++)
	{

		Cfg.gpio_pin_x = SwitchConfigurations[LocSwIteration].HSwitch_Pin;
		Cfg.gpio_port_x = SwitchConfigurations[LocSwIteration].HSwitch_Port;
		Cfg.gpio_Pull_x=SwitchConfigurations[LocSwIteration].HSwitch_InputType;
		Gpio_init(&Cfg);

	}

}

///////////////////////////////////////////////////////////////////////////////////

extern void HSwitch_Task(void){

	u8 LocSwIteration = 0;
	u32 state = 0 ;
	static u8 Counter[NumOfSwitch];
	static u8 PrevState[NumOfSwitch];
	GpioPinCfg_t Cfg;


	for(LocSwIteration=0;LocSwIteration<NumOfSwitch;LocSwIteration++){
		Cfg.gpio_port_x=SwitchConfigurations[LocSwIteration].HSwitch_Port;
		Cfg.gpio_pin_x=SwitchConfigurations[LocSwIteration].HSwitch_Pin;
		Cfg.gpio_mode_x=GPIO_MODE_u64_INPUT;
		Gpio_readPinValue(&Cfg ,&state );

		if(state==PrevState[LocSwIteration]){
			Counter[LocSwIteration]++;
		}

		else{

			Counter[LocSwIteration]=0;
		}

		if(Counter[LocSwIteration]==5){

		Sw_State[LocSwIteration]=state;

			Counter[LocSwIteration]=0;
		}

		PrevState[LocSwIteration]=state;


	}


}
///////////////////////////////////////////////////////////////////////////////////////////////////
extern HSwitch_tenuErrorStatus  HSwitch_readSwState(u16 CopySwitchNum , pu8 AddState){

	HSwitch_tenuErrorStatus LocenuErrorStatus = HSwitch_enuOk;

	if(AddState==NULL){

		LocenuErrorStatus= HSwitch_enuSwitchError;
}

else if(CopySwitchNum >= NumOfSwitch){

	LocenuErrorStatus = HSwitch_enuNullPointer;
}
else{
*AddState=Sw_State[CopySwitchNum];

}
return LocenuErrorStatus;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////


