#include "lpc17xx.h"

extern uint8_t direzione;
extern uint8_t down, down1, down2;
extern uint8_t uscita;
																
																
uint8_t calcola_distanza(void);
void accendi_direzione(void);
void muovi(void);
void accendi_led(void);
void clear_button(void);
void win(void);
void button0_RIT(void);
void button1_RIT(void);
void button2_RIT(void);
void first_setup(void);
