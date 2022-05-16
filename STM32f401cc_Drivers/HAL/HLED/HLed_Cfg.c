/*
 * H_Led_Cnfg.c
 *
 *  Created on: Apr 1, 2022
 *      Author: MahmoudElImbabi
 */
#include <../../include/SERVCIES/Std_types.h>
#include <HAL/HLED/HLed.h>
#include <HAL/HLED/HLed_Cfg.h>
#include <MCAL/GPIO/Gpio.h>
//////////////////////////////////////////////////////////////////////////////

/* GLOBAL CONSTANT ARRAY TYPE OF Hled_LedCfg  WITH SIZE NUMBER OF LED */
const Hled_LedCfg LED_Configurations[LED_NUM] =
{

[START_LED]=
{ .HLed_Port = GPIO_PORT_B, .HLed_Pin = GPIO_PIN_12, .HLed_ActiveState =
		LED_ACTIVE_STATE_HIGH },

//[READY_LED]=
//{ .HLed_Port = GPIO_PORT_C, .HLed_Pin = GPIO_PIN_01, .HLed_ActiveState =
//		ACTIVE_STATE_LOW, },

//[STOP_LED]=
//{ .Hled_Port = GPIO_PORT_A, .Hled_Pin = GPIO_PIN_02, .Hled_ActiveState =
//		ACTIVE_STATE_HIGH, }

		};

