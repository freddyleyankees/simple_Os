#define IRQ
#include "../include/i386/int.h"
#include "../include/kernel.h"
#include "../include/io.h"

static void timer(reg_t* reg){
	static unsigned int tic = 0;
	return;
}

void initTimer(unsigned int freq){
	unsigned int divid = 1193180/freq;

	outb(0x43, 0x36);/* channel 0 -> 00 11 x11 0*/
	outb(0x43, 0x54);/* channel 1 -> 01 10 x10 0*/
	outb(0x43, 0xB6);/* channel 2 -> */

	outb(0x40, 0x00);
	outb(0x40, 0x00);

	outb(0x41, 0x12);

	outb(0x40, 0xd1);
	outb(0x40, 0x11);

	unsigned char l = (unsigned char)(divid & 0xFF);
	unsigned char h = (unsigned char)((divid>>8) & 0xFF); 

	outb(0x43, 0x36);/* channel 0 -> 00 11 x11 0*/
	outb(0x40, l);
	outb(0x40, h);
	reg_handler(IRQ0,&timer);
}