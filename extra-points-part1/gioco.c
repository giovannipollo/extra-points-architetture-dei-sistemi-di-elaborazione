#include "gioco.h"
#include "lpc17xx.h"
#include "led/led.h"
#include "timer/timer.h"
#include "RIT/RIT.h"

//codifica mappa: 0 -> libero, 1 -> ostacolo, 2 -> uscita
uint8_t map[13][15] = {{2,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
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
uint8_t player_x; // coordinata delle righe
uint8_t player_y;	// coordinata delle colonne
uint8_t prima_volta;
//inizializzazione distanza dagli ostacoli																
uint8_t distanza; 

uint8_t calcola_distanza (void)
{
	uint8_t i,j,count = 0;
	int finito = 0;
	/* Conteggio distanza ostacolo */
	switch(direzione){
			case 0:  //est
				//dobbiamo scorrere la riga verso dx
			  j = player_y+1;
				count = 0;
				while (finito == 0 && j != 15 && map[player_x][j] == 0) /* finché trovi le caselle vuote e non sei alla fine vai avanti */
				{
					count++;
					if(j < 15)
						j++;
					else
						finito = 1;
				}
				if(map[player_x][j] == 2) /* se aveva contato la distanza da un'uscita come ostacolo, resetta */
					count=6; /* 6 perchè essendo l'uscita deve smettere di lampeggiare e quindi deve entrare nel default case (serve un valore maggiore di 5) */
				
				break;
			
			case 1: /* sud */
				/* dobbiamo scorrere la colonna verso il basso */
			  i = player_x+1;
				count = 0;
				while (finito == 0 && i != 13 && map[i][player_y] == 0) /* finché trovi le caselle vuote e non sei alla fine vai avanti */
				{
					count++;
					if(i < 13)
						i++;
					else
						finito = 1;
				}
				if(map[i][player_y] == 2) /* se aveva contato la distanza da un'uscita come ostacolo, resetta */
					count=6; /* 6 perchè essendo l'uscita deve smettere di lampeggiare e quindi deve entrare nel default case (serve un valore maggiore di 5) */
				break;
			
			case 2:
				/* dobbiamo scorrere la riga verso sx */
				if(player_y > 0)
					j = player_y-1;
				else
					finito = 1;  
				count = 0;
				while (finito == 0 && map[player_x][j] == 0) /* finché trovi le caselle vuote e non sei alla fine vai avanti */
				{
					count++;
					if(j > 0)
						j--;
					else
						finito = 1;
				}
				if(map[player_x][j] == 2) /* se aveva contato la distanza da un'uscita come ostacolo, resetta */
					count=6; /* 6 perchè essendo l'uscita deve smettere di lampeggiare e quindi deve entrare nel default case (serve un valore maggiore di 5) */
				break;
			
			case 3:
				/* dobbiamo scorrere la colonna verso su */
				if(player_x > 0)
					i = player_x-1;
				else
					finito = 1;
				count = 0;
				while (finito == 0 && map[i][player_y] == 0) /* finché trovi le caselle vuote e non sei alla fine vai avanti */
				{
					count++;
					if(i > 0)
						i--;
					else
						finito = 1;
				}
				if(map[i][player_y] == 2) /* se aveva contato la distanza da un'uscita come ostacolo, resetta */
					count=6; /* 6 perchè essendo l'uscita deve smettere di lampeggiare e quindi deve entrare nel default case (serve un valore maggiore di 5) */
				break;
			
			default:
				break;
		}
	
	return count;
}



void accendi_direzione (void){
	switch(direzione){
		case 0: /* est */
			LED_On(2); /* Siccome punto verso est, tengo fisso acceso il LED di direzione*/
			break;
		case 1: /* sud */
			LED_On(1);
			break;
		case 2: // ovest
			LED_On(0);
			break;
		default: /* nord */
			LED_On(3);
			break;
	}
	
}

void muovi(void){
		switch(direzione){
			case 0:
				if(player_y != 14)
					player_y++;
				break;
			case 1:
				if(player_x != 12)
				player_x++;
				break;
			case 2:
				if(player_y != 0)
					player_y--;
				break;
			case 3:
				if(player_x != 0)
					player_x--;
				break;
	}
}

void accendi_led(void){
	if(uscita == 0){
		switch(distanza){
			case 0: 
				reset_timer(0);
				LPC_TIM0->MR0 = 0x0017D784; /* Lampeggio a 8 Hz */
				enable_timer(0);
			
				
				LED_Off(5);
				reset_timer(1);
				LPC_TIM1->MR0 = 0x002625A0; /* lampeggio a 5 Hz */
				enable_timer(1);
			
				break;
			case 1:
			case 2:
				if(down2 == 0){ /* Il bottone di running è stato rilasciato */
					LED_Off(5); /* spengo il LED di running */
					disable_timer(1);
					reset_timer(1);
				}

				reset_timer(0);
				LPC_TIM0->MR0 = 0x002FAF08; /* Lampeggio 4 Hz */
				enable_timer(0);
				break;
			case 3:
			case 4:
			case 5:
				if(down2 == 0){ /* Il bottone di running è stato rilasciato */
					LED_Off(5); /* spengo il LED di running */
					disable_timer(1);
					reset_timer(1);
				}
				
				// 2 Hz												
				reset_timer(0);
				LPC_TIM0->MR0 = 0x005F5E10; /* blink 2 Hz */
				enable_timer(0);
				break;
			
			default:
				/* Il led di direzione deve rimanere acceso fisso senza lampeggiare */
				if(down2 == 0){ /* Il bottone di running è stato rilasciato */
					LED_Off(5);
					disable_timer(1);
					reset_timer(1);
				}
				
				disable_timer(0);
				reset_timer(0);
				accendi_direzione();
				break;
		}
	}
}


void clear_button(void){
		/* Pulisco gli interrupt pendenti dei bottoni EINT1 e EINT2 e riabilito l'interrupt per la pressione dei bottoni*/
	NVIC_ClearPendingIRQ(EINT1_IRQn);
	LPC_SC->EXTINT &= (1 << 1);
	NVIC_EnableIRQ(EINT1_IRQn);
	LPC_PINCON->PINSEL4    |= (1 << 22);   
	NVIC_ClearPendingIRQ(EINT2_IRQn);
	LPC_SC->EXTINT &= (1 << 2);
	NVIC_EnableIRQ(EINT2_IRQn);
	LPC_PINCON->PINSEL4    |= (1 << 24);
}

void win(void){
	
	uscita = 1; 
	disable_timer(0);
	reset_timer(0);
	reset_timer(1);
	LPC_TIM1->MR0 = 0x00BEBC20; /* Blink 1 Hz */
	enable_timer(1);
	
	NVIC_DisableIRQ(EINT0_IRQn); /* Disable interrupt bottone 0 */
	NVIC_DisableIRQ(EINT1_IRQn); /* Disable interrupt bottone 1 */
	NVIC_DisableIRQ(EINT2_IRQn); /* Disable interrupt bottone 2 */
}

void button0_RIT(void){
	    if(((LPC_GPIO2->FIOPIN & (1<<10)) == 0)){ /* Bottone premuto */
      reset_RIT();
      enable_RIT();
      if (down < 2) { /* Per evitare incrementi infiniti di down */ 
        first_setup();
				down++;
      }
    }
    else {    /* button released */
      down=0;           
      disable_RIT();
      reset_RIT();
      NVIC_EnableIRQ(EINT0_IRQn);                             /* disable Button interrupts            */
      LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
    }
}
void button1_RIT(void){
	if(((LPC_GPIO2->FIOPIN & (1<<11)) == 0) ){ /* Bottone premuto */
      reset_RIT(); 
      enable_RIT();
      if (down1 < 2) { /* Per evitare incrementi infiniti di down */ 
        down1++;
      }
    }
    else {    /* Bottone rilasciato */
      if (down1 > 1) {    /* Così da essere sicuro che il bottone sia stato premuto */				
				LED_Off(0);
				LED_Off(1);
				LED_Off(2);
				LED_Off(3);
					
				direzione++;
				direzione = direzione%4;
				
				distanza = calcola_distanza(); /* Calcolo la distanza dall'ostacolo nella direzione in cui punto */
				accendi_led();
					
      }
      down1=0;
			prima_volta = 0;
      disable_RIT();
      reset_RIT();
      NVIC_EnableIRQ(EINT1_IRQn);                             /* disable Button interrupts            */
      LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
    }
}

void button2_RIT(void){
	if(((LPC_GPIO2->FIOPIN & (1<<12)) == 0) ){ 
      reset_RIT();
      enable_RIT();
      down2++;
			distanza = calcola_distanza();
			
			if(prima_volta == 0){
				prima_volta = 1;
				accendi_led();
				if(distanza != 0){
					reset_timer(1);
					LPC_TIM1->MR0 = 0x00BEBC20;
					enable_timer(1);
				}
			}
			
			if(down2 == 21){ /* 21 perchè contando ogni 50ms per fare 1s ho bisogno di 20 iterazioni. Siccome down2 parte da 1, devo arrivare a 21 */
				down2 = 1;
				
				if (distanza != 0){
					muovi();
					NVIC_DisableIRQ(EINT1_IRQn); /* Disabilito l'interrupt del bottone 1 */
					if(map[player_x][player_y] == 2){
						/* Devo uscire dal gioco e far lampeggiare tutti i LED a 1Hz  */
						win();
					}else{
						distanza = calcola_distanza();
						accendi_led();
					}
				}
			}
    }
    else {    /* button released */
      down2=0;
			if(uscita == 0){
				if(distanza != 0){
					prima_volta = 0;
					LED_Off(5);
					disable_timer(1);
				}
				
				NVIC_ClearPendingIRQ(EINT1_IRQn);
				LPC_SC->EXTINT &= (1 << 1);
				NVIC_EnableIRQ(EINT1_IRQn);
				
				NVIC_ClearPendingIRQ(EINT2_IRQn);
				LPC_SC->EXTINT &= (1 << 1);
				NVIC_EnableIRQ(EINT2_IRQn);
				
			}
			disable_RIT();
      reset_RIT();
      
      LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
    }
}

void first_setup(void){
	
	/* codifica direzione: 0->E, 1->S, 2->O, 3->N */													
	direzione = 0;
	distanza = 0;
	prima_volta = 0;
	/* fai partire il lampeggio est */
	reset_timer(0); /* Reset del timer così da azzerare il match register  */
	LPC_TIM0->MR0 = 0x005F5E10; /* valore del match register blink 2 Hz */
	enable_timer(0); /* Abilito il timer */
	
	disable_timer(1);
	reset_timer(1);
	uscita = 0;
	LED_Out(0); /* Spengo tutti i LED */
	
	/* Inizializzo la posizione del player al centro della mappa */
	player_x = 7;
	player_y = 7;
	clear_button();
}
