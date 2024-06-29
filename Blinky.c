/**********************************************************************************************************
@file												 			  : Blinky.c
																	  : Lab04 / Section 1
																	  : Blinky LED using a defined delay_cycles function
@author							         		 	  : Sara Ghorbani Dehkordi
@role								         			  : Student - A01428439
@company							       			  : BCIT
*---------------------------------------------------------------------------------------------------------*
@system														  : STM32F429I DISCOVERY board
																	  : STM32F429ZITx MCU
																	  : This MCU has 2 USER LED  GREEN and ORANGE that are in port G
																	  : In this project we want to blink LED GREEN in some intervals
*---------------------------------------------------------------------------------------------------------*
@brief						  								: (1) Device_Init()
																		:		  Device Initialization
																		:     Initializes the PORTG, its clock and setting its MODE Register as Output
																		:			LED Initialization and Reset it  
																		: (2) delay_cycles is a delay function that we write it by assembly code
																		:			delay_ms is a c function. If the delay is more than a threshold
																		:			By partitioning this huge delay, prevent from overflow
																		:			a function to create delays in delay intervals
																		:	(3) Toggling LED on each delay_ms function
*------------------------------------------------------------------------------------------------------------*/
/*--------------------------------* Including Header Files *-------------------------------------------------*/
#include 							"stdio.h"
#include 							"stdint.h"
#include 							"delay_cycles.h"
#include 							"stm32f429xx.h"
/*--------------------------------* Define Constants *------------------------------------------------------*/
#define 							PORT_G 				6U
#define 							PING_LED_G 		13U
#define 							PING_LED_O 		14U
#define 							CLK_FREQ     	16000000
#define             	LED_DELAY 			500 																							/* milliseconds */
/*----------------------------------* Declare Functions and Global variables *-------------------------------*/
extern 								void 					delay_cycles(uint32_t cycles); 		 															          /* Using assembly delay_cycles function */
GPIO_TypeDef *p       =           	(GPIO_TypeDef*)(AHB1PERIPH_BASE + 0x0400 * PORT_G); /* Defining a pointer to access GPIOG & Enable clock for GPIOG */
																																												/* Based on startup MCU file, Stack_Size is EQU 0x00000400 */
static 								void 					toggle_pin(GPIO_TypeDef *p,uint32_t pin)
{
	p->ODR 						 ^= 						(1U<<pin); 																	/* Toggle the state of a pin by using Bitwise XOR Assignment */
	
}
static 								void 					delay_ms (uint32_t ms)
{
	uint32_t max_step   = 						1000 * (UINT32_MAX / CLK_FREQ) ; 										/* [ms] We define max step cycles to avoid overflow */
	uint32_t max_sleep_cycles = 			UINT32_MAX ; 																				/* equal to uint32_MAX*/
	while (ms > max_step){ 																																/* this part attempts to adjust for partial delays and improve accuracy */ 
		ms 							 -= 						max_step ;
		delay_cycles (max_sleep_cycles) ; 																									/* a sub 250 ms delay */
		}
	delay_cycles (ms * (CLK_FREQ/1000)) ; 																								/* converting remaining ms to cycles */
}
static								void					GPIO_Init(void)
{
		/*-------------------------------* Enable the clock of GPIO PORT G: Reset and Clock Control *--------------*/
	RCC->AHB1ENR 		 	|= 					1U << PORT_G; 																					/* AHB1 Peripheral Clock Enable Register */
																																												/* Instead, RCC_AHB1ENR_GPIOGEN_Msk, Based on stm32f429xx.h */
	/*-------------------------------* configuring the GPIOG PINs *---------------------------------------------*/
	p->MODER 						 = 					(p->MODER & 0x00FFFFFF) | 0x14000000;									/* mode of the pines of PORTG as output */
}
static 								void 					LED_Init()
{
	p->ODR 					 		|= 					(1U << PING_LED_G); 																	/* TURN ON LED GREEN by making its register set */
	p->ODR 					 		|= 					(1U << PING_LED_O); 																	/* TURN ON LED ORANGE by making its register set */
}
static 								void 					Device_Init (void)	
{
	GPIO_Init();
	LED_Init();
}

/*--------------------------------* Main Function *------------------------------------------------------*/
int 									main 					(void) 
{
	uint32_t pin_G = (uint32_t)PING_LED_G,pin_O=(uint32_t)PING_LED_O;
	Device_Init() ;
	while(1){
		delay_ms(LED_DELAY) ;
		toggle_pin(p,pin_G);
		toggle_pin(p,pin_O);
	}

}
