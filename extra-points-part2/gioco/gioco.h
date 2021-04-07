#include "lpc17xx.h"

extern uint8_t direzione;
extern uint8_t mode;
extern uint8_t down, down1, down2;
extern uint8_t uscita, vittoria;
extern uint8_t player_x, player_y, old_player_x, old_player_y, old_direzione;
extern uint8_t start;
extern uint16_t colore;	

void joystick_select(void);
void joystick_sud(void);
void joystick_nord(void);
void joystick_est(void);
void joystick_ovest(void);
void draw_player(void);
void draw_arrow(void);
void draw_obstacle(uint8_t offset);
void win(void);
uint8_t calcola_distanza(void);
void muovi(void);
