/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through EINT buttons and manage the bouncing effect
 *        	- key1 switches on the led at the left of the current led on, 
 *					- it implements a circular led effect. 	
  * Note(s): this version supports the LANDTIGER Emulator
 * Author: 	Paolo BERNARDI - PoliTO - last modified 15/12/2020
 *----------------------------------------------------------------------------
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2017 Politecnico di Torino. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>
#include "LPC17xx.H"                    /* LPC17xx definitions                */
#include "led/led.h"
#include "button_EXINT/button.h"
#include "timer/timer.h"
#include "RIT/RIT.h"

/* Led external variables from funct_led */
extern unsigned char led_value;					/* defined in funct_led								*/
extern uint8_t direzione;					/* defined in IRQ_RIT								*/
#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif



/* Prototipi funzioni */
void game_init(void);


int main (void) {

	game_init();
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }

}


void game_init(void){
	SystemInit();  												/* System Initialization (i.e., PLL)  */
  LED_init();                           /* LED Initialization                 */
  BUTTON_init();												/* BUTTON Initialization              */
	NVIC_DisableIRQ(EINT1_IRQn);					/* disable Button interrupts			 */
	NVIC_DisableIRQ(EINT2_IRQn);					/* disable Button interrupts			 */
	init_RIT(0x004C4B40);									/* RIT works at 100MHz -------- 50ms       */
																				/* 100MHz * 0.05 = 5*10^6 = 0x004C4B40 */
	
	init_timer(0,0x0017D784);							/* TIMER0 Initialization. It works ar 25MHz              */
																				/* Usato per il lampeggio a 8Hz, 4Hz, 2Hz*/
																				/* count = 0.0625 * 25*10^6 = 1562500 = 0x0017D784 */
	
	init_timer(1,0x002625A0);							/* TIMER1 Initialization. It works at 25MHz              */
																				/* Usato per il lampeggio a 5Hz*/
																				/* count = 0.10 * 25*10^6 = 2.5x10^6 = 0x002625A0 */
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
}
