#define IDT
#define INTR
#define ISR
#define IRQ
#include "../include/i386/int.h"
#include "../include/io.h"

void set_idt(unsigned char num,unsigned int base,unsigned short sel, unsigned char flags){
	idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].select = sel;
    idt[num].type = 0;
    idt[num].flags = flags;
}

void inidt(){
	cli;
	set_idt(ISR0,(unsigned int) asm_isr_0,0x08, INTGATE);
	set_idt(ISR1,(unsigned int) asm_isr_1,0x08, INTGATE);
	set_idt(ISR2,(unsigned int) asm_isr_2,0x08, INTGATE);
	set_idt(ISR3,(unsigned int) asm_isr_3,0x08, INTGATE);
	set_idt(ISR4,(unsigned int) asm_isr_4,0x08, INTGATE);
	set_idt(ISR5,(unsigned int) asm_isr_5,0x08, INTGATE);
	set_idt(ISR6,(unsigned int) asm_isr_5,0x08, INTGATE);
	set_idt(ISR7,(unsigned int) asm_isr_7,0x08, INTGATE);
	set_idt(ISR8,(unsigned int) asm_isr_8,0x08, INTGATE);
	set_idt(ISR9,(unsigned int) asm_isr_9,0x08, INTGATE);
	set_idt(ISR10,(unsigned int) asm_isr_10,0x08, INTGATE);
	set_idt(ISR11,(unsigned int) asm_isr_11,0x08, INTGATE);
	set_idt(ISR12,(unsigned int) asm_isr_12,0x08, INTGATE);
	set_idt(ISR13,(unsigned int) asm_isr_13,0x08, INTGATE);
	set_idt(ISR14,(unsigned int) asm_isr_14,0x08, INTGATE);
	set_idt(ISR15,(unsigned int) asm_isr_15,0x08, INTGATE);
	set_idt(ISR16,(unsigned int) asm_isr_16,0x08, INTGATE);
	set_idt(ISR17,(unsigned int) asm_isr_17,0x08, INTGATE);
	set_idt(ISR18,(unsigned int) asm_isr_18,0x08, INTGATE);
	set_idt(ISR19,(unsigned int) asm_isr_19,0x08, INTGATE);
	set_idt(ISR20,(unsigned int) asm_isr_20,0x08, INTGATE);
	set_idt(ISR21,(unsigned int) asm_isr_21,0x08, INTGATE);
	set_idt(ISR22,(unsigned int) asm_isr_22,0x08, INTGATE);
	set_idt(ISR23,(unsigned int) asm_isr_23,0x08, INTGATE);
	set_idt(ISR24,(unsigned int) asm_isr_24,0x08, INTGATE);
	set_idt(ISR25,(unsigned int) asm_isr_25,0x08, INTGATE);
	set_idt(ISR26,(unsigned int) asm_isr_26,0x08, INTGATE);
	set_idt(ISR27,(unsigned int) asm_isr_27,0x08, INTGATE);
	set_idt(ISR28,(unsigned int) asm_isr_28,0x08, INTGATE);
	set_idt(ISR29,(unsigned int) asm_isr_29,0x08, INTGATE);
	set_idt(ISR30,(unsigned int) asm_isr_30,0x08, INTGATE);
	set_idt(ISR31,(unsigned int) asm_isr_31,0x08, INTGATE);

	set_idt(IRQ0,(unsigned int) asm_irq_0,0x08, INTGATE);
	set_idt(IRQ1,(unsigned int) asm_irq_1,0x08, INTGATE);
	set_idt(IRQ2,(unsigned int) asm_irq_2,0x08, INTGATE);
	set_idt(IRQ3,(unsigned int) asm_irq_3,0x08, INTGATE);
	set_idt(IRQ4,(unsigned int) asm_irq_4,0x08, INTGATE);
	set_idt(IRQ5,(unsigned int) asm_irq_5,0x08, INTGATE);
	set_idt(IRQ6,(unsigned int) asm_irq_6,0x08, INTGATE);
	set_idt(IRQ7,(unsigned int) asm_irq_7,0x08, INTGATE);
	set_idt(IRQ8,(unsigned int) asm_irq_8,0x08, INTGATE);
	set_idt(IRQ9,(unsigned int) asm_irq_9,0x08, INTGATE);
	set_idt(IRQ10,(unsigned int) asm_irq_10,0x08, INTGATE);
	set_idt(IRQ11,(unsigned int) asm_irq_11,0x08, INTGATE);
	set_idt(IRQ12,(unsigned int) asm_irq_12,0x08, INTGATE);
	set_idt(IRQ13,(unsigned int) asm_irq_13,0x08, INTGATE);
	set_idt(IRQ14,(unsigned int) asm_irq_14,0x08, INTGATE);
	set_idt(IRQ15,(unsigned int) asm_irq_15,0x08, INTGATE);
	set_idt(IRQ16,(unsigned int) asm_irq_16,0x08, SYSTEMGATE);
	sti;
}