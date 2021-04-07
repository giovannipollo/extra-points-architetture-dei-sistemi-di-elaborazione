/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "lpc17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {
	/* joystick select functionality */
  LPC_PINCON->PINSEL3 &= ~(3<<18);	//PIN mode GPIO (00b value per P1.25)
	LPC_GPIO1->FIODIR   &= ~(1<<25);	//Select
	LPC_PINCON->PINSEL3 &= ~(3<<20);	//PIN mode GPIO (00b value per P1.25)
	LPC_GPIO1->FIODIR   &= ~(1<<26);	//Down
	LPC_PINCON->PINSEL3 &= ~(3<<22);	//PIN mode GPIO (00b value per P1.25)
	LPC_GPIO1->FIODIR   &= ~(1<<27);	//Left
	LPC_PINCON->PINSEL3 &= ~(3<<24);	//PIN mode GPIO (00b value per P1.25)
	LPC_GPIO1->FIODIR   &= ~(1<<28);	//Right
	LPC_PINCON->PINSEL3 &= ~(3<<26);	//PIN mode GPIO (00b value per P1.25)
	LPC_GPIO1->FIODIR   &= ~(1<<29);	//Up
}
