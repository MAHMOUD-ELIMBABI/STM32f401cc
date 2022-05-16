/*
 * 	H_Led.c
 *  Created on: Apr 1, 2022
 *  Author: MahmoudElImbabi
 */

#include <../../include/SERVCIES/Std_types.h>

#include <../../include/SERVCIES/Bit_utils.h>

#include <HAL/HLED/HLed.h>

#include <HAL/HLED/HLed_Cfg.h>

#include <../../include/MCAL/RCC/rcc.h>

#include <../../include/MCAL/GPIO/Gpio.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*INTIALIZING THE PORT OF THE LED AND OPENNING THE RCC ENABLE CLK PERIPHERAL FOR THIS PORT &  INTIALIZE THE PIN */
extern void Led_VidInit(void)
{
	u16 LocLedIteration = 0;  //LOCAL VARIAVBLE FOR THE FOR LOOP ITERATION
	u32 Rcc_Cfg = 0;		 //LOCAL VARIABLE FOR STORING THE AHB1_Periphral_bitmask

	/*FOR LOOP TO  LOOP ON THE LEDS  FOR ENABLING THE CLK FOR EACH PERIPHERAL*/
	for (LocLedIteration = 0; LocLedIteration < LED_NUM; LocLedIteration++)
	{
		/*SWITCH ON WHICH PORT  PERIPHRAL THE LED PIN IS CONNECTED ON*/
		switch (LED_Configurations[LocLedIteration].HLed_Port)
		{
		/*CASE IF THE LED IS ON PERIPHRAL PORT A*/
		case GPIO_PORT_A:
			/*STORING THE ENABLE BITMASK FOR PERIPHRAL GPIOA ON BUS AHB1  IN LOCAL VARIABLE Rcc_Cfg*/
			Rcc_Cfg |= AHB1_GPIOA_EN;

			break;
		/*CASE IF THE LED IS ON PERIPHRAL PORT B*/
		case GPIO_PORT_B:
			/*STORING THE ENABLE BITMASK FOR PERIPHRAL GPIOB ON BUS AHB1  IN LOCAL VARIABLE Rcc_Cfg*/
			Rcc_Cfg |= AHB1_GPIOB_EN;

			break;
		/*CASE IF THE LED IS ON PERIPHRAL PORT C*/
		case GPIO_PORT_C:
			/*STORING THE ENABLE BITMASK FOR PERIPHRAL GPIOC ON BUS AHB1  IN LOCAL VARIABLE Rcc_Cfg*/
			Rcc_Cfg |= AHB1_GPIOC_EN;

			break;
		/*CASE IF THE LED IS ON PERIPHRAL PORT D*/
		case GPIO_PORT_D:
			/*STORING THE ENABLE BITMASK FOR PERIPHRAL GPIOD ON BUS AHB1  IN LOCAL VARIABLE Rcc_Cfg*/
			Rcc_Cfg |= AHB1_GPIOD_EN;

			break;
		/*CASE IF THE LED IS ON PERIPHRAL PORT E*/
		case GPIO_PORT_E:
			/*STORING THE ENABLE BITMASK FOR PERIPHRAL GPIOE ON BUS AHB1  IN LOCAL VARIABLE Rcc_Cfg*/
			Rcc_Cfg |= AHB1_GPIOE_EN;

			break;
		/*CASE IF THE LED IS ON PERIPHRAL PORT H*/
		case GPIO_PORT_H:
			/*STORING THE ENABLE BITMASK FOR PERIPHRAL GPIOH ON BUS AHB1  IN LOCAL VARIABLE Rcc_Cfg*/
			Rcc_Cfg |= AHB1_GPIOH_EN;

			break;

		}

	}
/*CALLING THE ENABLE PERIPHRAL FUNCTION IN RCC DRIVER TO ENABLE THE PERIPHRAL CLK */
	Rcc_enuEnablePeriphral( RCC_REGISTER_AHB1, Rcc_Cfg); //Rcc_Cfg IS A LOCAL VARIABLE THAT STORING ALL ENABLE PERIPHRALS BIT MASKS FOR AHB1 BUS

	/*CREATING OBJECT CALLED Cfg FROM TYPE GpioPinCfg_t */
	GpioPinCfg_t Cfg =
	{
	/*CONFIGURING THE MODE & PULLUP OR DOWN OR FLOATING  & THE SPEED FOR THE PIN */
	.gpio_mode_x = GPIO_MODE_u64_OUTPUT_PP, .gpio_Pull_x =
			GPIO_PUPD_u32_FLOATING, .gpio_speed_x = GPIO_SPEED_VHIGH,

	};
    /*FOR LOOP TO INTIALIZE THE PORT AND PIN FOR ALL LEDS  	*/
	for (LocLedIteration = 0; LocLedIteration < LED_NUM; LocLedIteration++)
	{
		/*INTIALIZING THE PORT FOR THE OBJECT WE CREATED FROM TYPE GpioPinCfg_t WITH THE VALUE OF HLed_Port IN THE CONSTANT ARRAY  LED_Configurations */
		Cfg.gpio_port_x = LED_Configurations[LocLedIteration].HLed_Port;
		/*INTIALIZING THE PIN FOR THE OBJECT WE CREATED FROM TYPE GpioPinCfg_t WITH THE VALUE OF HLed_Port IN THE CONSTANT ARRAY  LED_Configurations */
		Cfg.gpio_pin_x = LED_Configurations[LocLedIteration].HLed_Pin;
		/*CALLING THE INTIALIZATION FUNCTION OF GPIO AND GIVE IT THE VARIABLE Cfg WITH TYPE GpioPinCfg_t* TO INIATLIZE THE LED PORT AND PIN */
		Gpio_init(&Cfg);

	}

}
/////////////////////////////////////////////////////////////////////////////////////////////
/* FUNCTION FOR SETTING THE STATE OF THE LED BY GIVING THE LED NAME AND LED STATE*/
 Hled_enuErrorStatus Led_SetState(u16 CopyLed_Name, u8 CopyLedState)
{
	/*LOC VAIRIABLE FOR THE ERROR STATUS WITH TYPE Hled_enuErrorStatus*/
	Hled_enuErrorStatus ErrorStatus = Hled_enuOk;


	/*CHECK IF THE INPUT PARAMETERS ARE VALID OR NOT*/
	if ((CopyLed_Name >= LED_NUM) || (CopyLedState > LED_STATES_NUM))
		{
		/*IF THE INPUT PARAMETER ARE NOT VALID RETURN ERROR STATUS CALLED  Hled_WrongParamter*/
			ErrorStatus = Hled_WrongParamter;

		}
	else{
	/*CALLLING THE Gpio_setPinnValue FROM THE GPIO DRIVER TO SET THE VALUE OF THE PIN BY GIVING THE FUNCTION
	 * THE PORT AND THE PIN NUMBER AND THE STATE IF HIGH OR LOW*/
	Gpio_setPinnValue(LED_Configurations[CopyLed_Name].HLed_Port,

			LED_Configurations[CopyLed_Name].HLed_Pin,
			/*TAKE THE IPUT PARAMETER CopyLedState AND XOR IT WITH THE HLed_ActiveState TO ABSTRACT THE HARDWARE CHANGE OF
			 * LED PINS */
			CopyLedState ^ LED_Configurations[CopyLed_Name].HLed_ActiveState);

	}
	/*RETURN ERROR STATUS FOR THE FUNCTION Led_SetState */
	return ErrorStatus;

}

//////////////////////////////////////////////////////////////////////////////////
