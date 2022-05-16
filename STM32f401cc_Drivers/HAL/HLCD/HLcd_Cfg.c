/*
 * HLcd_Cfg.c
 *
 *  Created on: Apr 14, 2022
 *      Author: MahmoudElImbabi
 */
#include<SERVCIES/Std_types.h>
#include<HAL/HLCD/HLcd.h>
#include<HAL/HLCD/HLcd_Cfg.h>
#include<MCAL/GPIO/Gpio.h>
////////////////////////////////////////////////////////////////////////
const HLcd_LcdCfg  HLcd_Configurations[Num_Of_Lcd_Pins]=
{		[Lcd_RS]={

				.HLcd_Port=GPIO_PORT_A , .HLcd_Pin=GPIO_PIN_00


		},

		[Lcd_RW]={

				.HLcd_Port=GPIO_PORT_A , .HLcd_Pin=GPIO_PIN_01


		},

		[Lcd_E]={

				.HLcd_Port=GPIO_PORT_A , .HLcd_Pin=GPIO_PIN_02


		},

		[Lcd_D0]={

				.HLcd_Port=GPIO_PORT_A , .HLcd_Pin=GPIO_PIN_03


		},

		[Lcd_D1]={

				.HLcd_Port=GPIO_PORT_A , .HLcd_Pin=GPIO_PIN_04


		},

		[Lcd_D2]={

				.HLcd_Port=GPIO_PORT_A , .HLcd_Pin=GPIO_PIN_05


		},

		[Lcd_D3]={

				.HLcd_Port=GPIO_PORT_A , .HLcd_Pin=GPIO_PIN_06


		},

		[Lcd_D4]={

				.HLcd_Port=GPIO_PORT_A , .HLcd_Pin=GPIO_PIN_07


		},

		[Lcd_D5]={

				.HLcd_Port=GPIO_PORT_A , .HLcd_Pin=GPIO_PIN_08


		},

		[Lcd_D6]={

				.HLcd_Port=GPIO_PORT_A , .HLcd_Pin=GPIO_PIN_09


		},

		[Lcd_D7]={

				.HLcd_Port=GPIO_PORT_A , .HLcd_Pin=GPIO_PIN_10


		},


};
////////////////////////////////////////////////////////////////////////
