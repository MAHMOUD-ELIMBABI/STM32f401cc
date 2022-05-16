/*
 * HSwitch_Cfg.c
 *
 *  Created on: Apr 14, 2022
 *      Author: MahmoudElImbabi
 */

#include<SERVCIES/Std_types.h>
#include<HAL/HSwitch/HSwitch.h>
#include<HAL/HSwitch/HSwitch_Cfg.h>
#include<MCAL/GPIO/Gpio.h>
////////////////////////////////////////////////////////////////////////////////////
const HSwitch_SwitchCfg SwitchConfigurations[NumOfSwitch] =
{ [StartSwitch]
{

.HSwitch_Port = GPIO_PORT_A, .HSwitch_Pin = GPIO_PIN_02, .HSwitch_InputType =
		GPIO_PUPD_u32_PULL_DOWN

},

//[StopSwitch]
//{
//	.HSwitch_Port = GPIO_PORT_A, .HSwitch_Pin = GPIO_PIN_05, .HSwitch_InputType =
//			GPIO_PUPD_u32_PULL_DOWN
//
//},
};
///////////////////////////////////////////////////////////////////////////////////////
