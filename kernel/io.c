#include "../include/kernel.h"
#include "../drivers/kbd/kbd.h"


int geti(){}
char getc(){
	int b = 1, a;
	asm volatile("movl %1, %%ebx; int $0x60;movl %%eax, %0":"=r"(a):"r"(b));
}
