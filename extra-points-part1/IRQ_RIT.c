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
#include "../led/led.h"
#include "../timer/timer.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
uint8_t direction;
uint8_t down, down1, down2;
uint8_t distance_calc (uint8_t dir, uint8_t x, uint8_t y, uint8_t map[13][15]);

void RIT_IRQHandler (void)
{			
	static int down=0;
	static int down1=0;	
	static uint8_t click_rotate=0;
	//codifica mappa: 0->libero, 1->ostacolo, 2->uscita
	static uint8_t map[13][15] = {	{2,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
																	{0,0,1,1,1,1,0,0,0,0,0,0,0,0,0},
																	{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
																	{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
																	{0,0,0,0,0,0,0,1,0,0,0,0,0,0,0},
																	{1,1,1,1,1,0,0,1,0,1,0,0,0,1,0},
																	{0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
																	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
																	{1,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
																	{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
																	{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
																	{1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
																	{2,0,1,0,0,0,0,0,0,0,0,0,0,0,2}		
																};
	

	//inizializzazione cordinate di partenza del giocatore
  static uint8_t player_x = 7;
	static uint8_t player_y = 7;	
									
	//inizializzazione distanza dagli ostacoli alla partenza del giocatore																
	static uint8_t distance = 0;
	
																
	down++;
												
	if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
		reset_RIT();
		
		switch(down){
			case 1:
				//se è in pressione il tasto start
				//codifica direzione: 0->E, 1->S, 2->O, 3->N													
				direction = 0;	
				//fai partire il lampeggio est
				enable_timer(0);
				//distance = distance_calc(direction,12,7,map);
			
				break;
			default:
				break;
		}
	}
	else {	/* button released */
		down=0;			
		disable_RIT();
		reset_RIT();
		NVIC_EnableIRQ(EINT0_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
	}
		
	down1++;
	if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){ //tastto rotate
		reset_RIT();
		
		switch(down1){
			case 1:
				//se è in pressione il tasto ROTATE
				click_rotate = 1;
			
				break;
			default:
				break;
		}
	}
	else {	/* button released */
		if(click_rotate==1)
		{
			
			LED_Off(0);
			LED_Off(1);
			LED_Off(2);
			LED_Off(3);
			
			if(direction ==3)
				direction=0;

			else
				direction++;
			
			distance = distance_calc(direction,player_x,player_y,map);
			//TO DO -> Far lampeggiare correttamente i led in base alla distanza		
		}
		
	
		
		down1=0;			
		disable_RIT();
		reset_RIT();
		NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
	}
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

uint8_t distance_calc (uint8_t dir, uint8_t x, uint8_t y, uint8_t map[13][15])
{
	uint8_t j,i,count = 0;
	
	//Conteggio distanza ostacolo
	switch(dir){
			case 0:  //est
				//dobbiamo scorrere la riga verso dx
			  j = y+1;
				count = 0;
				while (j!=15 && map[x][j] == 0) //finché trovi le caselle vuote e non sei alla fine vai avanti
				{
					count++;
					j++;	
				}
				if(map[x][j] == 2) //se aveva contato la distanza da un'uscita come ostacolo, resetta
					count=0;
				
				break;
			
			case 1: //sud
				//dobbiamo scorrere la colonna verso il basso
			  i = x+1;
				count = 0;
				while (i!=13 && map[i][y] == 0) //finché trovi le caselle vuote e non sei alla fine vai avanti
				{
					count++;
					i++;	
				}
				
				if(map[i][y] == 2) //se aveva contato la distanza da un'uscita come ostacolo, resetta
					count=0;
				break;
			
			case 2:
				//dobbiamo scorrere la riga verso sx
			  j = y-1;
				count = 0;
				while (j>=0 && map[x][j] == 0) //finché trovi le caselle vuote e non sei alla fine vai avanti
				{
					count++;
					j--;	
				}
				
				if(map[x][j] == 2) //se aveva contato la distanza da un'uscita come ostacolo, resetta
					count=0;
				break;
			
			case 3:
				//dobbiamo scorrere la colonna verso su
				i = x-1;
				count = 0;
				while (i>=0 && map[i][y] == 0) //finché trovi le caselle vuote e non sei alla fine vai avanti
				{
					count++;
					i--;	
				}
				
				if(map[i][y] == 2) //se aveva contato la distanza da un'uscita come ostacolo, resetta
					count=0;
				break;
			
			default:
				break;
		}
	
	return count;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
