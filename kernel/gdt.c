#define __GDT__
#include "../include/i386/gdt.h"
#include "../include/string.h"
#include "../include/kernel.h"
#include "../include/io.h"



void set_gdt(unsigned int e, unsigned int a, unsigned int l, unsigned char t, unsigned char o){
	gdt[e].limit0_15 = (l & 0xFFFF);
	gdt[e].address0_15 = (a & 0xFFFF);
	gdt[e].address16_23 = (a & 0xFF0000) >> 16;
	gdt[e].access = t;
	gdt[e].limit16_19 = (l & 0xF0000) >> 16;
	gdt[e].other = (o & 0xF);
	gdt[e].address24_31 = (a & 0xFF000000) >> 24;
}

void init_gdt(){
	memset(&tss, 0,0x67);
	tss.debug = 0;
	tss.iomap = 0;
	tss.esp0 = 0x00101000;
    tss.ss0 = 0x10;								
	tss.cs = 0x0B;
    tss.ss = tss.ds = tss.es = tss.fs = tss.gs = 0x13;
	set_gdt(3, (unsigned int)&tss, 0x67, 0xE9, 0x00);
	set_gdt(4, 0x00000, 0xFFF, 0xFA, 0x0D);
	set_gdt(5, 0x00000, 0xFFF, 0xF2, 0x0D);
	ltr(0x18);
}