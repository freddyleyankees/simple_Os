/**
* Nadia OS descriptor file .c
*
* This file describe 
*
* @author : kabong freddy
#define	I8259A_PORTA0	0x20
#define	I8259A_PORTB0	0x21
#define	I8259A_PORTA1	0xA0
#define	I8259A_PORTB1	0xA1
*/

#define I8259A
#include "../include/io.h"
#include "../include/i386/int.h"


void map_i386_mask_irq(void){

	outbp(I8259A_PORTB0, 0xFF);
	outbp(I8259A_PORTB1, 0xFF);
}

void map_i386_active_irq(void){

	outbp(I8259A_PORTB0, 0x0);
	outbp(I8259A_PORTB1, 0x0);
}

void map_i386_irq(void){

	outbp(I8259A_PORTA0, 0x11);
	outbp(I8259A_PORTA1, 0x11);

	outbp(I8259A_PORTB0, 0x20);
	outbp(I8259A_PORTB1, 0x28);

	outbp(I8259A_PORTB0, 0x04);
	outbp(I8259A_PORTB1, 0x02);

	outbp(I8259A_PORTB0, 0x01);
	outbp(I8259A_PORTB1, 0x01);
}
