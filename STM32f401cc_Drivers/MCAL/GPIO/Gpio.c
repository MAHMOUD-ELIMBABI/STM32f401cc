/*
 * Gpio.c
 *
 *  Created on: Mar 13, 2022
 *      Author: MahmoudElImbabi
 */

#include<SERVCIES/Std_types.h>
#include<SERVCIES/Bit_utils.h>
#include <MCAL/GPIO/Gpio.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct{

	volatile u32 GPIOx_MODER;
	volatile u32 GPIOx_OTYPER;
	volatile u32 GPIOx_OSPEEDER;
	volatile u32 GPIOx_PUPDR;
	volatile u32 GPIOx_IDR;
	volatile u32 GPIOx_ODR;
	volatile u32 GPIOx_BSRR;
	volatile u32 GPIOx_LCKR;
	volatile u32 GPIOx_AFRL;
	volatile u32 GPIOx_AFRH;


} GPIO_t ;
////////////////////////////////////BASE_ADDRSESS///////////////////////////////////////////////////////
#define GPIO_A_BASEADDRESS        0x40020000
#define GPIO_B_BASEADDRESS        0x40020400
#define GPIO_C_BASEADDRESS        0x40020800
#define GPIO_D_BASEADDRESS        0x40020C00
#define GPIO_E_BASEADDRESS        0x40021000
#define GPIO_H_BASEADDRESS        0x40021C00
////////////////////////////////MACROS////////////////////////////////////////////////////////////////////
#define GPIO_HIGH_PINS                 8
#define GPIO_DOUBLE_BIT                2
#define GPIO_QUADRO_BIT                4
#define GPIO_NUM_OF_PINS               16
#define GPIO_MAX_NUM_OF_PINS		   15
#define GPIO_MAX_PORT_NUM              5
////////////////////////////////MASK MACROS///////////////////////////////////////////////////////////////
#define GPIO_CLR_MSK		      0x00000003
#define GPIO_TYPE_MASK_PP		  0x00000001
#define GPIO_TYPE_MASK_OD		  0x00000001
#define GPIO_SET_RESET_MASK		  0x00000001
#define GPIO_AF_CLR_MASK 		  0x0000000F
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define GPIO_A    ((GPIO_t*)(GPIO_A_BASEADDRESS))
#define GPIO_B    ((GPIO_t*)(GPIO_B_BASEADDRESS))
#define GPIO_C    ((GPIO_t*)(GPIO_C_BASEADDRESS))
#define GPIO_D    ((GPIO_t*)(GPIO_D_BASEADDRESS))
#define GPIO_E    ((GPIO_t*)(GPIO_E_BASEADDRESS))
#define GPIO_H    ((GPIO_t*)(GPIO_H_BASEADDRESS))
//////////////////////////////////////////////////////////////////////////////////////////////////////////
Gpio_tenuErrorStatus Gpio_init(GpioPinCfg_t  *Add_CnfgStruct){

	Gpio_tenuErrorStatus ErrorStatus = Gpio_enuOk;
	GPIO_t * LOC_pstArrPORT[6]={GPIO_A ,GPIO_B  ,GPIO_C  ,GPIO_D  ,GPIO_E  ,GPIO_H };
	u8 LOC_u8Port= Add_CnfgStruct->gpio_port_x;
	u8 LOC_u8Pin= Add_CnfgStruct->gpio_pin_x;


	if(LOC_u8Pin > GPIO_MAX_NUM_OF_PINS){

		ErrorStatus = Gpio_WrongPinNumError;

	}

	else if(LOC_u8Port > GPIO_MAX_PORT_NUM){

		ErrorStatus = Gpio_WrongPortNumError;

	}


	else{


	switch(Add_CnfgStruct->gpio_mode_x){

	case GPIO_MODE_u64_OUTPUT_PP:

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_MODER  &= ~ (GPIO_CLR_MSK<<LOC_u8Pin*GPIO_DOUBLE_BIT);
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_MODER  |=   (GPIO_MODE_u64_OUTPUT_PP<<LOC_u8Pin*GPIO_DOUBLE_BIT);
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_OTYPER &= ~ (GPIO_TYPE_MASK_PP<<LOC_u8Pin);

		break;

	case  GPIO_MODE_u64_OUTPUT_OD:

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_MODER  &= ~ (GPIO_CLR_MSK<<LOC_u8Pin*GPIO_DOUBLE_BIT);
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_MODER  |=   (GPIO_MODE_u64_OUTPUT_OD<<LOC_u8Pin*GPIO_DOUBLE_BIT);
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_OTYPER |=   (GPIO_TYPE_MASK_OD<<LOC_u8Pin);

		break;

	case GPIO_MODE_u64_INPUT :

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_MODER &= ~ (GPIO_CLR_MSK<< LOC_u8Pin*GPIO_DOUBLE_BIT);

		break;

	case GPIO_MODE_u64_AF_PP:

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_MODER  &= ~ (GPIO_CLR_MSK<< LOC_u8Pin*GPIO_DOUBLE_BIT);
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_MODER  |=   (GPIO_MODE_u64_AF_PP<< LOC_u8Pin*GPIO_DOUBLE_BIT);
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_OTYPER &= ~ (GPIO_TYPE_MASK_PP<< LOC_u8Pin);

		break;

	case GPIO_MODE_u64_AF_OD:

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_MODER  &= ~ (GPIO_CLR_MSK<< LOC_u8Pin*GPIO_DOUBLE_BIT);
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_MODER  |=   (GPIO_MODE_u64_AF_OD<< LOC_u8Pin*GPIO_DOUBLE_BIT);
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_OTYPER |=   (GPIO_TYPE_MASK_OD<< LOC_u8Pin);

		break;

	case GPIO_MODE_u64_ANALOG:

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_MODER  &= ~ (GPIO_CLR_MSK<< LOC_u8Pin*GPIO_DOUBLE_BIT);
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_MODER  |=   (GPIO_MODE_u64_ANALOG<< LOC_u8Pin*GPIO_DOUBLE_BIT);

		break;

	default :

		ErrorStatus = Gpio_WrongModeError;

		break;
	}
/******************************************************************************************************/

switch(Add_CnfgStruct->gpio_Pull_x){

	case GPIO_PUPD_u32_FLOATING:

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_PUPDR &= ~ (GPIO_PUPD_u32_FLOATING<<LOC_u8Pin*GPIO_DOUBLE_BIT);

	break;

	case GPIO_PUPD_u32_PULL_UP:

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_PUPDR &= ~ (GPIO_PUPD_u32_FLOATING<<LOC_u8Pin*GPIO_DOUBLE_BIT);
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_PUPDR |=   (GPIO_PUPD_u32_PULL_UP<<LOC_u8Pin*GPIO_DOUBLE_BIT);

	break;

	case GPIO_PUPD_u32_PULL_DOWN:

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_PUPDR &= ~ (GPIO_PUPD_u32_FLOATING<<LOC_u8Pin*GPIO_DOUBLE_BIT);
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_PUPDR |=   (GPIO_PUPD_u32_PULL_DOWN<<LOC_u8Pin*GPIO_DOUBLE_BIT);

	break;

	default:

		ErrorStatus = Gpio_WrongPuPdError;

		break;
}
/****************************************************************************************************/

switch(Add_CnfgStruct->gpio_speed_x){

	case GPIO_SPEED_LOW:

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_OSPEEDER &= ~ (GPIO_CLR_MSK << LOC_u8Pin*GPIO_DOUBLE_BIT );

	break;

	case GPIO_SPEED_MEDIUM :

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_OSPEEDER &= ~ (GPIO_CLR_MSK << LOC_u8Pin*GPIO_DOUBLE_BIT );
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_OSPEEDER |=   (GPIO_SPEED_MEDIUM << LOC_u8Pin*GPIO_DOUBLE_BIT );

	break;

	case GPIO_SPEED_HIGH:

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_OSPEEDER &= ~ (GPIO_CLR_MSK << LOC_u8Pin*GPIO_DOUBLE_BIT );
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_OSPEEDER |=   (GPIO_SPEED_HIGH << LOC_u8Pin*GPIO_DOUBLE_BIT );

		break;

	case GPIO_SPEED_VHIGH:

		LOC_pstArrPORT[LOC_u8Port]->GPIOx_OSPEEDER &= ~ (GPIO_CLR_MSK << LOC_u8Pin*GPIO_DOUBLE_BIT );
		LOC_pstArrPORT[LOC_u8Port]->GPIOx_OSPEEDER |=   (GPIO_SPEED_VHIGH << LOC_u8Pin*GPIO_DOUBLE_BIT );

		break;

	default:

	ErrorStatus = Gpio_WrongSpeedError;

		break;

}
	}
/*********************************************************************************************************/
return ErrorStatus ;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
Gpio_tenuErrorStatus Gpio_setPinValue(GpioPinCfg_t  *Add_pu8CnfgStatus , u8 Copyu8PinValue)
{

		Gpio_tenuErrorStatus ErrorStatus = Gpio_enuOk;
		GPIO_t * LOC_pstArrPORT[6]={GPIO_A ,GPIO_B  ,GPIO_C  ,GPIO_D  ,GPIO_E  ,GPIO_H };
		u8 LOC_u8Port= Add_pu8CnfgStatus->gpio_port_x;
		u8 LOC_u8Pin= Add_pu8CnfgStatus->gpio_pin_x;


		if(LOC_u8Pin > GPIO_MAX_NUM_OF_PINS){

			ErrorStatus = Gpio_WrongPinNumError;

		}

		else if(LOC_u8Port > GPIO_MAX_PORT_NUM){

			ErrorStatus = Gpio_WrongPortNumError;

		}


		else{


		switch(Copyu8PinValue){

			case GPIO_ODR_HIGH :

					if((Add_pu8CnfgStatus->gpio_mode_x) == GPIO_MODE_u64_OUTPUT_PP){

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_BSRR &= ~(GPIO_SET_RESET_MASK<<LOC_u8Pin);

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_BSRR |=  (GPIO_SET_RESET_MASK<<LOC_u8Pin);

					}
					else if((Add_pu8CnfgStatus->gpio_mode_x)  == GPIO_MODE_u64_OUTPUT_OD){

						LOC_pstArrPORT[LOC_u8Port]->GPIOx_BSRR &= ~(GPIO_SET_RESET_MASK<<LOC_u8Pin);

						LOC_pstArrPORT[LOC_u8Port]->GPIOx_BSRR |=  (GPIO_SET_RESET_MASK<<LOC_u8Pin);
					}

					else{

					ErrorStatus =  Gpio_WrongValueError ;

					}
						break;



			case GPIO_ODR_LOW :

					if((Add_pu8CnfgStatus->gpio_mode_x)  == GPIO_MODE_u64_OUTPUT_PP){

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_BSRR &= ~(GPIO_SET_RESET_MASK<<LOC_u8Pin);

					}
					else if((Add_pu8CnfgStatus->gpio_mode_x)  == GPIO_MODE_u64_OUTPUT_OD){

						LOC_pstArrPORT[LOC_u8Port]->GPIOx_BSRR &= ~(GPIO_SET_RESET_MASK<<LOC_u8Pin);

					}

					else{

					ErrorStatus =  Gpio_WrongValueError ;

					}
						break;

		}

		}
return ErrorStatus = Gpio_enuOk;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
Gpio_tenuErrorStatus Gpio_setPinnValue(u8 Copyu8Port ,u8 Copyu8Pin ,u8 Copyu8Value){

	Gpio_tenuErrorStatus ErrorStatus = Gpio_enuOk;
	GPIO_t * LOC_pstArrPORT[6]={GPIO_A ,GPIO_B  ,GPIO_C  ,GPIO_D  ,GPIO_E  ,GPIO_H };


	if(Copyu8Pin > GPIO_MAX_NUM_OF_PINS){

		ErrorStatus = Gpio_WrongPinNumError;

	}

	else if(Copyu8Port > GPIO_MAX_PORT_NUM){

		ErrorStatus = Gpio_WrongPortNumError;

	}


	else{



	switch(Copyu8Value){

		case GPIO_ODR_HIGH :

       if(GET_BIT(LOC_pstArrPORT[Copyu8Port]->GPIOx_MODER , (Copyu8Pin*GPIO_DOUBLE_BIT)) == 1 && GET_BIT(LOC_pstArrPORT[Copyu8Port]->GPIOx_MODER , ((Copyu8Pin*GPIO_DOUBLE_BIT)+1))==0 ){

    	   LOC_pstArrPORT[Copyu8Port]->GPIOx_BSRR =  (GPIO_SET_RESET_MASK<<Copyu8Pin);

       }

       else{

    	   ErrorStatus = Gpio_WrongReadError;

       }
						break;



		case GPIO_ODR_LOW :

			if(GET_BIT(LOC_pstArrPORT[Copyu8Port]->GPIOx_MODER , (Copyu8Pin*GPIO_DOUBLE_BIT)) == 1 && GET_BIT(LOC_pstArrPORT[Copyu8Port]->GPIOx_MODER , ((Copyu8Pin*GPIO_DOUBLE_BIT)+1))==0 ){


					LOC_pstArrPORT[Copyu8Port]->GPIOx_BSRR = (GPIO_SET_RESET_MASK<<Copyu8Pin + GPIO_NUM_OF_PINS );

			}
			else{

				ErrorStatus = Gpio_WrongReadError;
			}
					break;

	}

	}

	return ErrorStatus = Gpio_enuOk;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Gpio_tenuErrorStatus Gpio_readPinValue(GpioPinCfg_t  *Add_CnfgStruct , pu32 Add_pu8PinValue){

			Gpio_tenuErrorStatus ErrorStatus = Gpio_enuOk;
			GPIO_t * LOC_pstArrPORT[6]={GPIO_A ,GPIO_B  ,GPIO_C  ,GPIO_D  ,GPIO_E  ,GPIO_H };
			u8 LOC_u8Port= Add_CnfgStruct->gpio_port_x;
			u8 LOC_u8Pin= Add_CnfgStruct->gpio_pin_x;


		if(LOC_u8Pin > GPIO_MAX_NUM_OF_PINS){

				ErrorStatus = Gpio_WrongPinNumError;

			}

		else if(LOC_u8Port > GPIO_MAX_PORT_NUM){

				ErrorStatus = Gpio_WrongPortNumError;

			}


		else{


			if(Add_CnfgStruct->gpio_mode_x  == GPIO_MODE_u64_INPUT ){

				*Add_pu8PinValue  =  GET_BIT(LOC_pstArrPORT[LOC_u8Port]->GPIOx_IDR , LOC_u8Pin);

			}
			else{

				ErrorStatus =  Gpio_WrongReadError ;

			}

		}

	return ErrorStatus = Gpio_enuOk;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Gpio_tenuErrorStatus Gpio_SetAlternateFunction(GpioPinCfg_t  *Add_pu8CnfgStatus , u8 CopyAF){

	Gpio_tenuErrorStatus ErrorStatus = Gpio_enuOk;
			GPIO_t * LOC_pstArrPORT[6]={GPIO_A ,GPIO_B  ,GPIO_C  ,GPIO_D  ,GPIO_E  ,GPIO_H };
			u8 LOC_u8Port= Add_pu8CnfgStatus->gpio_port_x;
			u8 LOC_u8Pin= Add_pu8CnfgStatus->gpio_pin_x;


			if(LOC_u8Pin > GPIO_MAX_NUM_OF_PINS){

				ErrorStatus = Gpio_WrongPinNumError;

			}

			else if(LOC_u8Port > GPIO_MAX_PORT_NUM){

				ErrorStatus = Gpio_WrongPortNumError;

			}


			else{





	if(((Add_pu8CnfgStatus->gpio_mode_x)  == (GPIO_MODE_u64_AF_PP)) | ((Add_pu8CnfgStatus->gpio_mode_x)  == (GPIO_MODE_u64_AF_OD))){

	if(LOC_u8Pin < GPIO_HIGH_PINS){
		switch(CopyAF){

			case GPIO_AF_0:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_0 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_1:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_1 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_2:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_2 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_3:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_3 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_4:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_4 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_5:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_5 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_6:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_6 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_7:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_7 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_8:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_8 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_9:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_9 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_10:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_10 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_11:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_11 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_12:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_12 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_13:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_13 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_14:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_14 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;
			case GPIO_AF_15:

					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
					LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRL |= (GPIO_AF_15 << LOC_u8Pin*GPIO_QUADRO_BIT);

			break;

			default:
				ErrorStatus =Gpio_WrongAFError;
				break;
		}
	}

		else {

			switch(CopyAF){

			case GPIO_AF_0:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_0 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_1:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_1 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_2:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_2 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_3:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_3 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_4:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_4 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_5:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_5 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_6:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_6 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_7:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_7 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_8:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_8 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_9:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_9 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_10:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_10 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_11:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_11 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_12:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_12 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_13:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_13 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_14:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_14 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;
			case GPIO_AF_15:

							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH &= ~(GPIO_AF_CLR_MASK<< GPIO_PIN_00*GPIO_QUADRO_BIT);
							LOC_pstArrPORT[LOC_u8Port]->GPIOx_AFRH |= (GPIO_AF_15 << LOC_u8Pin*GPIO_QUADRO_BIT);

					break;

			default:
				ErrorStatus =Gpio_WrongAFError;
				break;


			}


		}

	}


}


	return ErrorStatus = Gpio_enuOk;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
