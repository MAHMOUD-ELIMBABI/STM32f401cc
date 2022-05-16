/*
 * HLcd.c
 *
 *  Created on: Apr 14, 2022
 *      Author: MahmoudElImbabi
 */
#include<SERVCIES/Std_types.h>
#include<SERVCIES/Bit_utils.h>
#include<MCAL/GPIO/Gpio.h>
#include<MCAL/RCC/rcc.h>
#include<HAL/HLCD/HLcd.h>
#include<HAL/HLCD/HLcd_Cfg.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*MACRO LIKE FUNCTION TO SET THE PIN VALUE OF  RW TO LOW , PINS FROM D0 TO D7 TO EACH DATA_BYTE , ENABLE PIN TO HIGH*/
#define LCD_SET_DATABUS(DATA_BYTE)     	Gpio_setPinnValue(HLcd_Configurations[Lcd_RW].HLcd_Port , HLcd_Configurations[Lcd_RW].HLcd_Pin , GPIO_ODR_LOW);\
										for(LocDataBusIterator = Lcd_D0; LocDataBusIterator <= Lcd_D7 ; LocDataBusIterator++ ){\
										Gpio_setPinnValue(HLcd_Configurations[LocDataBusIterator].HLcd_Port , HLcd_Configurations[LocDataBusIterator].HLcd_Pin , GET_BIT(DATA_BYTE , (LocDataBusIterator - Lcd_D0)));\
										}\
										Gpio_setPinnValue(HLcd_Configurations[Lcd_E].HLcd_Port , HLcd_Configurations[Lcd_E].HLcd_Pin , GPIO_ODR_HIGH);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**/
typedef enum
{

	Lcd_pause,
	Lcd_Start,
	Lcd_FunctionSet,
	Lcd_DisplayControl,
	Lcd_DisplayClear,
	Lcd_EnteryMode,
	Lcd_End

} Lcd_InitState_t;

typedef enum
{
	Lcd_NotBusy, Lcd_WritingString, Lcd_ClearLcd,

} Lcd_State_t;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static u8 InitState;

static u8 State;

static Buffer_t * Save_Buffer;

static u8 Position;

static void HLcd_vidInitProcess(void);

static void HLcd_ClearProcess(void);

static void HLcd_WritingProcess(void);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void HLcd_vidInit(void)
{
	/*
	 * ENABLE CLOCK FOR EACH PORT
	 * port&pins------>GPIO Array
	 * INIT_STATE=START;
	 *
	 * */

	u8 LocLcdIteration = 0;

	u32 Rcc_Cfg = 0;

	for (LocLcdIteration = 0; LocLcdIteration < Num_Of_Lcd_Pins;
			LocLcdIteration++)
	{

		switch (HLcd_Configurations[LocLcdIteration].HLcd_Port)
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

	Rcc_enuEnablePeriphral(RCC_REGISTER_AHB1, Rcc_Cfg);

	GpioPinCfg_t Cfg =
	{

	.gpio_mode_x = GPIO_MODE_u64_OUTPUT_PP, .gpio_Pull_x =
	GPIO_PUPD_u32_FLOATING

	, .gpio_speed_x = GPIO_SPEED_VHIGH

	};

	for (LocLcdIteration = 0; LocLcdIteration < Num_Of_Lcd_Pins;
			LocLcdIteration++)
	{

		Cfg.gpio_port_x = HLcd_Configurations[LocLcdIteration].HLcd_Port;

		Cfg.gpio_pin_x = HLcd_Configurations[LocLcdIteration].HLcd_Pin;

		Gpio_init(&Cfg);
	}

	InitState = Lcd_Start;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HLcd_vidInitProcess(void)
{

	static u8 LocTimeMs;
	u8 LocDataBusIterator;
	LocTimeMs++;

	switch (InitState)
	{

	case Lcd_Start:
		if (LocTimeMs > 30)
		{

			InitState = Lcd_FunctionSet;
			LocTimeMs = 0;

		}
		break;

	case Lcd_FunctionSet:
		if (LocTimeMs == 1)
		{
			Gpio_setPinnValue(HLcd_Configurations[Lcd_RS].HLcd_Port,
					HLcd_Configurations[Lcd_RS].HLcd_Pin, GPIO_ODR_LOW);
			LCD_SET_DATABUS(LCD_FUNCTIONSET);
		}
		else if (LocTimeMs == 2)
		{
			Gpio_setPinnValue(HLcd_Configurations[Lcd_E].HLcd_Port,
					HLcd_Configurations[Lcd_E].HLcd_Pin, GPIO_ODR_LOW);
			InitState = Lcd_DisplayControl;
			LocTimeMs = 0;
		}
		break;

	case Lcd_DisplayControl:
		if (LocTimeMs == 1)
		{
			Gpio_setPinnValue(HLcd_Configurations[Lcd_RS].HLcd_Port,
					HLcd_Configurations[Lcd_RS].HLcd_Pin, GPIO_ODR_LOW);
			LCD_SET_DATABUS(LCD_DISPLAYCONTROL);
		}
		else if (LocTimeMs == 2)
		{
			Gpio_setPinnValue(HLcd_Configurations[Lcd_E].HLcd_Port,
					HLcd_Configurations[Lcd_E].HLcd_Pin, GPIO_ODR_LOW);
			InitState = Lcd_DisplayClear;
			LocTimeMs = 0;
		}

		break;

	case Lcd_DisplayClear:

		if (LocTimeMs == 1)
		{
			Gpio_setPinnValue(HLcd_Configurations[Lcd_RS].HLcd_Port,
					HLcd_Configurations[Lcd_RS].HLcd_Pin, GPIO_ODR_LOW);
			LCD_SET_DATABUS(LCD_DISPLAYCLEAR);
		}

		else if (LocTimeMs == 2)
		{

			Gpio_setPinnValue(HLcd_Configurations[Lcd_E].HLcd_Port,
					HLcd_Configurations[Lcd_E].HLcd_Pin, GPIO_ODR_LOW);

		}
		else if (LocTimeMs == 3)
		{

			InitState = Lcd_EnteryMode;
			LocTimeMs = 0;

		}
		break;

	case Lcd_EnteryMode:
		if (LocTimeMs == 1)
		{
			Gpio_setPinnValue(HLcd_Configurations[Lcd_RS].HLcd_Port,
					HLcd_Configurations[Lcd_RS].HLcd_Pin, GPIO_ODR_LOW);
			LCD_SET_DATABUS(LCD_ENTERYMODE);
		}
		else if (LocTimeMs == 2)
		{

			InitState = Lcd_End;
			LocTimeMs = 0;

		}


		break;

	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void HLcd_vidTask(void)
{

	if (Lcd_pause < InitState && InitState < Lcd_End)
	{
		HLcd_vidInitProcess();
	}
	else if (InitState == Lcd_End)
	{
		switch (State)
		{

		case Lcd_WritingString:

			HLcd_WritingProcess();

			break;

		case Lcd_ClearLcd:

			HLcd_ClearProcess();

			break;

		}

	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern HLcd_tenuErrorStatus HLcd_enuWriteStringZCopy(Buffer_t * Add_pstrBuffer,
		u8 PositionX, u8 PositionY)
{

	HLcd_tenuErrorStatus LocErrorStatus = HLcd_enuOk;

	if (Add_pstrBuffer == NULL)
	{

		LocErrorStatus = HLcd_enuNullPointer;

	}

	else if (PositionX != LCD_ROW_00 || PositionX != LCD_ROW_01)
	{

		LocErrorStatus = HLcd_enuOutOfRange;

	}

	else if (PositionY > LCD_COL_15)
	{

		LocErrorStatus = HLcd_enuOutOfRange;

	}

	if (State == Lcd_NotBusy)
	{

		Save_Buffer->Used = 0;

		Save_Buffer = Add_pstrBuffer;

		Position = PositionX + PositionY;

		State = Lcd_WritingString;

	}

	else
	{

		LocErrorStatus = HLcd_enuErrorBusy;

	}

	return LocErrorStatus;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HLcd_WritingProcess(void)
{
	static u8 TimeMs;
	u8 LocDataBusIterator;
	if(TimeMs == 0)
	{
		Gpio_setPinnValue(HLcd_Configurations[Lcd_RS].HLcd_Port,HLcd_Configurations[Lcd_RS].HLcd_Pin, GPIO_ODR_LOW);
		LCD_SET_DATABUS(Position);
		TimeMs++;
	}
	else if(TimeMs == 1)
	{
		Gpio_setPinnValue(HLcd_Configurations[Lcd_E].HLcd_Port,HLcd_Configurations[Lcd_E].HLcd_Pin, GPIO_ODR_LOW);

		TimeMs++;
	}
	else if(TimeMs == 2)
	{
		if(Save_Buffer->Used < Save_Buffer->Size )
		{
			Gpio_setPinnValue(HLcd_Configurations[Lcd_RS].HLcd_Port,HLcd_Configurations[Lcd_RS].HLcd_Pin, GPIO_ODR_HIGH);
			LCD_SET_DATABUS(Save_Buffer->Data[Save_Buffer->Used]);
			Save_Buffer->Used++;
			TimeMs--;
		}
		else{
			State = Lcd_NotBusy;
			TimeMs =0;
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern HLcd_tenuErrorStatus HLcd_vidClearLcd(void)
{

	HLcd_tenuErrorStatus LocErrorStatus = HLcd_enuOk;

	if (State == Lcd_NotBusy)
	{

		State = Lcd_ClearLcd;

	}
	else
	{

		LocErrorStatus = HLcd_enuErrorBusy;

	}

	return LocErrorStatus;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void HLcd_ClearProcess(void)
{
	static u8 TimeMs ;

	u8 LocDataBusIterator;

	if(TimeMs==0){

		Gpio_setPinnValue(HLcd_Configurations[Lcd_RS].HLcd_Port , HLcd_Configurations[Lcd_RS].HLcd_Pin , GPIO_ODR_LOW);
		LCD_SET_DATABUS(LCD_DISPLAYCLEAR);
	}

	else if(TimeMs==1){

		Gpio_setPinnValue(HLcd_Configurations[Lcd_E].HLcd_Port , HLcd_Configurations[Lcd_E].HLcd_Pin , GPIO_ODR_LOW);

		State = Lcd_NotBusy;

		TimeMs = 0;


	}

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
