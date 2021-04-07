/*----------------------------------------------------------------------------
 * Name:    sample.c
 * Purpose: to control led through debounced buttons and Joystick
 *        	- key1 switches on the led at the left of the current led on, 
 *					- it implements a circular led effect,
 * 					- joystick select function returns to initial configuration (led11 on) .
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
#include "TouchPanel/TouchPanel.h"
#include "GLCD/GLCD.h"
#include "timer/timer.h"
#include "RIT/RIT.h"
#include "gioco/gioco.h"
#include "joystick/joystick.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif


uint8_t direzione;
uint8_t mode;
uint8_t player_x, player_y;
uint8_t start;

void init_game(void);
/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  	
	init_game();
		
  while (1) {                           /* Loop forever                       */	
		__ASM("wfi");
  }
}


void init_game(void){
	uint8_t i;
	SystemInit();  												/* System Initialization (i.e., PLL)  */
	joystick_init();											/* Joystick Initialization            */
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	enable_RIT();													/* RIT enabled												*/
	
	/* Init display*/
	LCD_Initialization();
	TP_Init();
	//TouchPanel_Calibrate();
	GUI_Text(60, 20, (uint8_t *) "BLIND LABYRINTH", White, Blue);
	
	//DISEGNO MAPPA
	LCD_DrawRectangle(15, 50, 210, 182, Cyan , 1);
	
	//disegno righe
	for(i=0; i<14; i++)
	{
		LCD_DrawLine( 15, (i*14)+50 , 225, (i*14)+50 , Black );
	}
	//disegno colonne
		for(i=0; i<16; i++)
	{
		LCD_DrawLine( (i*14)+15, 50 , (i*14)+15, 232 , Black );
	}
	
	//DISEGNO TASTI
	LCD_DrawRectangle(5, 260, 110, 50, Magenta , 1);
	LCD_DrawRectangle(125, 260, 110, 50, Magenta , 1);
	GUI_Text(20, 270, (uint8_t *) "Restart", Red, Magenta);
	GUI_Text(140, 270, (uint8_t *) "Clear", Yellow, Magenta);
	
	
	/* Init position of the player and mode */
	direzione = 'e';
	player_x = 7;
	player_y = 7;
	mode = 'm';
	start = 1;
	draw_player();
	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
}


