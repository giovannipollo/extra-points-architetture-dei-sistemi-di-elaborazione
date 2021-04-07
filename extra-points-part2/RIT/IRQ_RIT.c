/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../gioco/gioco.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


void RIT_IRQHandler (void)
{					
	joystick_select();
	joystick_est();
	joystick_sud();
	joystick_ovest();
	joystick_nord();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	disable_RIT();
	reset_RIT();
	if(vittoria == 0)
		enable_RIT();
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
