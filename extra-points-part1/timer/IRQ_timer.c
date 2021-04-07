/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern unsigned char led_value;					/* defined in funct_led								*/
extern uint8_t direzione;					/* defined in IRQ_RIT								*/
extern uint8_t uscita;
void TIMER0_IRQHandler (void)
{

	switch(direzione){
			case 0: //accendi led est
				//se è in pressione il tasto start
				if((LPC_GPIO2->FIOPIN & (1<<2)) == 4)
					LED_Off(2);
				else
					LED_On(2);
	
				break;
			case 1: //accendi led sud
				//se è in pressione il tasto start
				if((LPC_GPIO2->FIOPIN & (1<<1)) == 2)
				LED_Off(1);
				else
				LED_On(1);
	
				break;
			case 2: //accendi led ovest
				//se è in pressione il tasto start
				if((LPC_GPIO2->FIOPIN & (1<<0)) == 1)
				LED_Off(0);
				else
				LED_On(0);
	
				break;
			case 3: //accendi led nord
				//se è in pressione il tasto start
				if((LPC_GPIO2->FIOPIN & (1<<3)) == 8)
				LED_Off(3);
				else
				LED_On(3);
	
				break;	
			
			default:
				break;
		}

  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	switch(uscita){
		case 0:
			if((LPC_GPIO2->FIOPIN & (1<<5)) == 32)
				LED_Off(5);
			else
				LED_On(5);
			break;
			
		case 1:
			if((LPC_GPIO2 -> FIOPIN & (1 <<7)) == 128)
				LED_Out(0);
			else
				LED_Out(255);
	}

	
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
