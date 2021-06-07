#define EXECINT
#define ISR
#include "../include/i386/int.h"
#include "../include/kernel.h"
#include "../include/io.h"

static void divideExcep(){
	reg_handler(ISR0, &divideZero);
}

static void debugExcep(){
	reg_handler(ISR1, &debug);
}

static void nmExcep(){
	reg_handler(ISR2, &nonMask);
}

static void breakExcep(){
	reg_handler(ISR3, &int3);
}

static void overflowExcep(){
	reg_handler(ISR4, &overflow);
}

static void boundsExcep(){
	reg_handler(ISR5, &bounds);
}

static void invalidExcep(){
	reg_handler(ISR6, &invalidop);
}

static void invalidCoproExcep(){
	reg_handler(ISR7, &noCop);
}

static void doubleExcep(){
	reg_handler(ISR8, &doubleFault);
}

static void segFPExcep(){
	reg_handler(ISR9, &coSeg);
}

static void tssExcep(){
	reg_handler(ISR10, &invalidTSS);
}

static void segExcep(){
	reg_handler(ISR11, &noSeg);
}

static void stackExcep(){
	reg_handler(ISR12, &stackFault);
}

static void gpExcep(){
	reg_handler(ISR13, &GP);
}

static void pfExcep(){
	reg_handler(ISR14, &pageFault);
}

static void copExcep(){
	reg_handler(ISR16, &copError);
}

static void resrvedExcep(){
	reg_handler(ISR15, &reserve);
	for(unsigned int i=17;i<32;i++)
		reg_handler(i, &reserve);
}

void trap_init(){
	divideExcep();
	debugExcep();
	nmExcep();
	breakExcep();
	overflowExcep();
	boundsExcep();
	invalidExcep();
	invalidCoproExcep();
	doubleExcep();
	segFPExcep();
	tssExcep();
	segExcep();
	stackExcep();
	gpExcep();
	pfExcep();
	copExcep();
	resrvedExcep();
}