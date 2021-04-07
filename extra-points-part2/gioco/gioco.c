#include "gioco.h"
#include "lpc17xx.h"
#include "../GLCD/GLCD.h"
#include "../RIT/RIT.h"


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
					
//inizializzazione distanza dagli ostacoli																
uint8_t distanza;
uint8_t old_player_x, old_player_y, old_direzione;
uint16_t colore;															
																
uint8_t calcola_distanza(void){
	uint8_t count = 0;
	uint8_t i = 0;
	uint8_t j = 0;
	int finito = 0;
	/* Conteggio distanza ostacolo */
	switch(direzione){
			case 'e':  //est
				//scorrere la riga verso dx
				if(player_y < 14)
					j = player_y+1;
				else
					finito = 1;
				count = 0;
				while (finito == 0 && j != 15 && map[player_x][j] == 0) /* finch? trovi le caselle vuote e non sei alla fine vai avanti */
				{
					count++;
					if(j < 14)
						j++;
					else
						finito = 1;
				}
				if(map[player_x][j] == 2 || finito == 1) /* se aveva contato la distanza da un'uscita come ostacolo, resetta */
					count=6; /* 6 perch? essendo l'uscita deve smettere di lampeggiare e quindi deve entrare nel default case (serve un valore maggiore di 5) */
				
				break;
			
			case 's': /* sud */
				/* scorrere la colonna verso il basso */
				if(player_x < 12)
					i = player_x+1;
				else
					finito = 1;
				count = 0;
				while (finito == 0 && i != 13 && map[i][player_y] == 0) /* finch? trovi le caselle vuote e non sei alla fine vai avanti */
				{
					count++;
					if(i < 12)
						i++;
					else
						finito = 1;
				}
				if(map[i][player_y] == 2 || finito == 1) /* se aveva contato la distanza da un'uscita come ostacolo, resetta */
					count=6; /* 6 perch? essendo l'uscita deve smettere di lampeggiare e quindi deve entrare nel default case (serve un valore maggiore di 5) */
				break;
			
			case 'o':
				/* scorrere la riga verso sx */
				if(player_y > 0)
					j = player_y-1;
				else
					finito = 1;  
				count = 0;
				while (finito == 0 && map[player_x][j] == 0) /* finch? trovi le caselle vuote e non sei alla fine vai avanti */
				{
					count++;
					if(j > 0)
						j--;
					else
						finito = 1;
				}
				if(map[player_x][j] == 2 || finito == 1) /* se aveva contato la distanza da un'uscita come ostacolo, resetta */
					count=6; /* 6 perch? essendo l'uscita deve smettere di lampeggiare e quindi deve entrare nel default case (serve un valore maggiore di 5) */
				break;
			
			case 'n':
				/* scorrere la colonna verso l'alto */
				if(player_x > 0)
					i = player_x-1;
				else
					finito = 1;
				count = 0;
				while (finito == 0 && map[i][player_y] == 0) /* finch? trovi le caselle vuote e non sei alla fine vai avanti */
				{
					count++;
					if(i > 0)
						i--;
					else
						finito = 1;
				}
				if(map[i][player_y] == 2 || finito == 1) /* se aveva contato la distanza da un'uscita come ostacolo, resetta */
					count=6; /* 6 perch? essendo l'uscita deve smettere di lampeggiare e quindi deve entrare nel default case (serve un valore maggiore di 5) */
				break;
			
			default:
				break;
		}
	
	return count;
}

void muovi(void){
			old_player_x = player_x;
			old_player_y = player_y;
			switch(direzione){
			case 'e':
				if(player_y != 14)
					player_y++; /* Muovi il player incrementando il valore di player_y. */
				break;
			case 's':
				if(player_x != 12)
				player_x++;
				break;
			case 'o':
				if(player_y != 0)
					player_y--;
				break;
			case 'n':
				if(player_x != 0)
					player_x--;
				break;
	}
}

void joystick_select(void){
	static int select = 0;
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){	
		/* Joytick Select pressed */
		select++;
		switch(select){
			case 1:
				if(mode == 'e'){
					mode = 'm';
					colore = Black;
				}
				else{
					mode = 'e';
					colore = Magenta;
				}
				draw_player();
				break;
			default:
				break;
		}
	}
	else{
			select=0;
	}
}
void joystick_sud(void){
	static int sud = 0;
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0 && (LPC_GPIO1->FIOPIN & (1<<27)) != 0 && (LPC_GPIO1->FIOPIN & (1<<28)) != 0){	
	/* Joytick down pressed */
	sud++;
	switch(sud){
		case 1:
			old_direzione = direzione;
			direzione = 's';
			distanza = calcola_distanza();
			old_player_x = player_x;
			old_player_y = player_y;
			if(old_direzione != 's'){
				draw_player();
				if(distanza < 5){
					draw_obstacle(distanza + 1);
				}
			}
			break;
		case 20:
			sud = 0;
			if(mode == 'm'){
				direzione = 's'; // se mi trovo in un'altra direzione, dopo aver premuto per 1s il tasto sud, la direzione diventa sud
				distanza = calcola_distanza();
				if(distanza != 0){
					old_player_x = player_x;
					old_player_y = player_y;
					muovi();
					draw_player();
					distanza = calcola_distanza();
					if(distanza < 5){
							draw_obstacle(distanza+1);
					}
					if(map[player_x][player_y] == 2){
						win();
						disable_RIT();
						reset_RIT();
					}
				}
			}
			break;
		default:
			break;
		}
	}else{
		sud=0;
	}
}
void joystick_nord(void){
	static int nord = 0;
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0 && (LPC_GPIO1->FIOPIN & (1<<27)) != 0 && (LPC_GPIO1->FIOPIN & (1<<28)) != 0){	
		/* Joytick up pressed */
		nord++;
		switch(nord){
			case 1:
				old_direzione = direzione;
				direzione = 'n';
				distanza = calcola_distanza();
				old_player_x = player_x;
				old_player_y = player_y;
				
				if(old_direzione != 'n'){
					draw_player();
					if(distanza < 5){
						draw_obstacle(distanza + 1);
					}
				}
				break;
			case 20:
				nord = 0;
				if(mode == 'm'){
					direzione = 'n'; // se mi trovo in un'altra direzione, dopo aver premuto per 1s il tasto sud, la direzione diventa sud
					distanza = calcola_distanza();
					if(distanza != 0){
						old_player_x = player_x;
						old_player_y = player_y;
						muovi();
						draw_player();
						distanza = calcola_distanza();
						if(distanza < 5){
							draw_obstacle(distanza+1);
						}
						if(map[player_x][player_y] == 2){
							win();
						}
					}
				}
				break;
			default:
				break;
		}
	}
	else{
			nord=0;
	}
}
void joystick_est(void){
	static int est = 0;
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0 && (LPC_GPIO1->FIOPIN & (1<<26)) != 0 && (LPC_GPIO1->FIOPIN & (1<<29)) != 0){	
		/* Joytick right pressed */
		est++;
		switch(est){
			case 1:
				old_direzione = direzione;
				direzione = 'e';
				distanza = calcola_distanza();
				old_player_x = player_x;
				old_player_y = player_y;
				
				if(old_direzione != 'e'){
					draw_player();
					if(distanza < 5){
						draw_obstacle(distanza + 1);
					}
				}
				break;
			case 20:
				est = 0;
				if(mode == 'm'){
					direzione = 'e'; // se mi trovo in un'altra direzione, dopo aver premuto per 1s il tasto sud, la direzione diventa sud
					distanza = calcola_distanza();
					if(distanza != 0){
						old_player_x = player_x;
						old_player_y = player_y;
						muovi();
						draw_player();
						distanza = calcola_distanza();
						if(distanza < 5){
							draw_obstacle(distanza+1);
						}
						if(map[player_x][player_y] == 2){
							win();
						}
					}
				}
				break;
			default:
				break;
		}
	}
	else{
			est=0;
	}
}
void joystick_ovest(void){
	static int ovest = 0;
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0 && (LPC_GPIO1->FIOPIN & (1<<26)) != 0 && (LPC_GPIO1->FIOPIN & (1<<29)) != 0){	
		/* Joytick left pressed */
		ovest++;
		switch(ovest){
			case 1:
				old_direzione = direzione;
				direzione = 'o';
				distanza = calcola_distanza();
				old_player_x = player_x;
				old_player_y = player_y;
				
				if(old_direzione != 'o'){
					draw_player();
					if(distanza < 5){
						draw_obstacle(distanza + 1);
					}
				}
				break;
			case 20:
				ovest = 0;
				if(mode == 'm'){
					direzione = 'o'; // se mi trovo in un'altra direzione, dopo aver premuto per 1s il tasto sud, la direzione diventa sud
					distanza = calcola_distanza();
					if(distanza != 0){
						old_player_x = player_x;
						old_player_y = player_y;
						muovi();
						draw_player();
						distanza = calcola_distanza();
						if(distanza < 5){
							draw_obstacle(distanza+1);
						}
						if(map[player_x][player_y] == 2){
							win();
						}
					}
				}
				break;
			default:
				break;
		}
	}
	else{
			ovest=0;
	}
}


void draw_player(void){
	if(start == 1){
		draw_arrow();
		start = 0;
	}
	else{
		LCD_DrawRectangle(old_player_y*14 + 15 + 1, old_player_x*14 + 50 + 1, 12, 12, Cyan, 1);
		draw_arrow();
	}
}

void draw_arrow(void){
	
	int length = 13;
	int offset_x, offset_y;
	switch(direzione){
		case 'e':
			offset_x = 0;
			offset_y = 0;
			while(length>1)
			{ 
				length -= 2;
				LCD_DrawLine( player_y*14+offset_x+15+5, player_x*14 + offset_y+50+1 , player_y*14 + offset_x +15 +5 ,player_x*14 + offset_y + length +50 +1 , colore );
				offset_x += 1;
				offset_y += 1;
			}
			break;
		case 's':
			offset_x = 0;
			offset_y = 0;
			while(length>1)
			{ 
				length -= 2;
				LCD_DrawLine( player_y*14 + offset_x + 15 + 2, player_x*14 + offset_y + 50 + 4, player_y*14+ offset_x + length + 15 + 2 , player_x*14 + offset_y + 50 + 4, colore );
				offset_x += 1;
				offset_y += 1;
			}
			break;
		case 'o':
			offset_x = 18;
			offset_y = 0;
			while(length>1)
				{ 
					length -= 2;
					LCD_DrawLine( player_y*14+offset_x + 5, player_x*14 + offset_y + 50+1 , player_y*14+offset_x + 5 , player_x*14 + offset_y +length +50+1 , colore );
					offset_x -= 1;
					offset_y += 1;
}
			break;
		case 'n':
			offset_x = 0;
			offset_y = 19; 
			while(length>1)
			{ 
				length -= 2;
				LCD_DrawLine( player_y*14 + offset_x + 15 + 2, player_x*14 + offset_y + 50 - 10 , player_y*14 + offset_x + length + 15 + 2, player_x*14 +offset_y + 50 - 10 , colore );
				offset_x += 1;
				offset_y -= 1;
			}
			break;
	}
}


void draw_obstacle(uint8_t offset){
	switch(direzione){
		case 'e':
			LCD_DrawRectangle(player_y*14 + 15 + offset*14 + 1, player_x*14 + 50 + 1, 12, 12, Red, 1);
			break;
		case 's':
			LCD_DrawRectangle(player_y*14 + 15 + 1, player_x*14 + offset*14 + 50 + 1, 12, 12, Red, 1);
			break;
		case 'o':
			LCD_DrawRectangle(player_y*14 + 15 - offset*14 + 1, player_x*14 + 50 + 1, 12, 12, Red, 1);
			break;
		case 'n':
			LCD_DrawRectangle(player_y*14 + 15 + 1, player_x*14 - offset*14 + 50 + 1, 12, 12, Red, 1);
			break;
		default:
			break;
	}
}

void win(void){
	start_abilitato = 0;
	vittoria = 1;
	disable_RIT();
	reset_RIT();
	LCD_DrawRectangle(15, 50, 210, 182, Cyan , 1);
	GUI_Text(90, 120, (uint8_t *) "YOU WON", Black, Cyan);
}
