/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <stdio.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../gioco/gioco.h"
#include "../TouchPanel/TouchPanel.h"
#include "../GLCD/GLCD.h"
#include "../RIT/RIT.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
uint8_t player_x, player_y, direzione, mode, start, vittoria, start_abilitato, clear_abilitato;

void TIMER0_IRQHandler (void)
{
	uint8_t i, j;
	getDisplayPoint(&display, Read_Ads7846(), &matrix ); 
	
	/* Reset */
	if(display.x >= 5 && display.x <= 115){
		if(display.y >= 260 && display.y <= 310){
			start_abilitato = 1;
			disable_RIT();
			reset_RIT();
			player_x = 7;
			player_y = 7;
			direzione = 'e';
			mode = 'm';
			start = 1;
			colore = Black;
			clear_abilitato = 0;
			LCD_DrawRectangle(15, 50, 210, 182, Cyan , 1);
			GUI_Text(65, 120, (uint8_t *) "TOUCH TO START", White, Blue);
			GUI_Text(83, 140, (uint8_t *) "A NEW GAME", White, Blue);
		}
	}
	
	/* Clear */
	if(display.x >= 125 && display.x <= 235){
		if(display.y >= 260 && display.y <= 310){
			if(vittoria == 0){
				if(clear_abilitato != 0){
					for(i = 0; i < 13; i++){
						for(j = 0; j < 15; j++){
							if(i != player_x || j != player_y){
								LCD_DrawRectangle(j*14 + 15 + 1, i*14 + 50 + 1, 12, 12, Cyan, 1);
							}
						}
					}
					calcola_distanza();
					if(distanza < 5){
						draw_obstacle(distanza + 1);
					}
				}
			}
		}
	}

	/* Start */
	if(display.x >= 15 && display.x <= 225){
		if(display.y >= 50 && display.y <= 232){
			if(start_abilitato != 0){
				vittoria = 0;
				clear_abilitato = 1;
				player_x = 7;
				player_y = 7;
				direzione = 'e';
				mode = 'm';
				start = 1;
				colore = Black;
				
				LCD_DrawRectangle(50, 105, 130 , 50, Cyan , 1);
				
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
				draw_player();
				enable_RIT();
			}
		}
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
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
