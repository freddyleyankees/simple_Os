#ifndef _INTR_H
#define _INTR_H
#ifdef I8259A
#define	I8259A_PORTA0	0x20
#define	I8259A_PORTB0	0x21
#define	I8259A_PORTA1	0xA0
#define	I8259A_PORTB1	0xA1
#endif
void map_i386_mask_irq(void);
void map_i386_irq(void);
void map_i386_active_irq(void);
#endif
#define INTGATE	  		0x8E		
#define TRAPGATE 		0x8F	
#define SYSTEMGATE	 	0xEF
#define INTMAX	 		256
#ifdef ISR
#define ISR0	0
#define ISR1	1
#define ISR2	2
#define ISR3	3
#define ISR4	4
#define ISR5	5
#define ISR6	6
#define ISR7	7
#define ISR8	8
#define ISR9	9
#define ISR10	10
#define ISR11	11
#define ISR12	12
#define ISR13	13
#define ISR14	14
#define ISR15	15
#define ISR16	16
#define ISR17	17
#define ISR18	18
#define ISR19	19
#define ISR20	20
#define ISR21	21
#define ISR22	22
#define ISR23	23
#define ISR24	24
#define ISR25	25
#define ISR26	26
#define ISR27	27
#define ISR28	28
#define ISR29	29
#define ISR30	30
#define ISR31	31
#endif
#ifdef IRQ
#define IRQ0	32
#define IRQ1	33
#define IRQ2	34
#define IRQ3	35
#define IRQ4	36
#define IRQ5	37
#define IRQ6	38
#define IRQ7	39
#define IRQ8	40
#define IRQ9	41
#define IRQ10	42
#define IRQ11	43
#define IRQ12	44
#define IRQ13	45
#define IRQ14	46
#define IRQ15	47
#define IRQ16	96
#endif

struct idt_desc
{
	unsigned short base_low;
	unsigned short select;
	unsigned char type;
	unsigned char flags;
	unsigned short base_high;
} __attribute__ ((packed));

struct idtr
{
	unsigned short limit;
	unsigned int address;
};

typedef struct reg
{
	unsigned int gs,fs, es, ds;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int num, err;
	unsigned int eip, cs, eflags, uesp, uss;
} __attribute__ ((packed)) reg_t;

typedef void (*isr_t)(reg_t*);
void reg_handler(unsigned int, isr_t);
void int_handler(reg_t*);
void inidt();
#ifdef IDT
struct idt_desc* idt;
void set_idt(unsigned char num,unsigned int base,unsigned short sel, unsigned char flags);
#else
extern struct idt_desc* idt;
#endif
#ifdef INTR
extern void asm_isr_0(void);
extern void asm_isr_1(void);
extern void asm_isr_2(void);
extern void asm_isr_3(void);
extern void asm_isr_4(void);
extern void asm_isr_5(void);
extern void asm_isr_6(void);
extern void asm_isr_7(void);
extern void asm_isr_8(void);
extern void asm_isr_9(void);
extern void asm_isr_10(void);
extern void asm_isr_11(void);
extern void asm_isr_12(void);
extern void asm_isr_13(void);
extern void asm_isr_14(void);
extern void asm_isr_15(void);
extern void asm_isr_16(void);
extern void asm_isr_17(void);
extern void asm_isr_18(void);
extern void asm_isr_19(void);
extern void asm_isr_20(void);
extern void asm_isr_21(void);
extern void asm_isr_22(void);
extern void asm_isr_23(void);
extern void asm_isr_24(void);
extern void asm_isr_25(void);
extern void asm_isr_26(void);
extern void asm_isr_27(void);
extern void asm_isr_28(void);
extern void asm_isr_29(void);
extern void asm_isr_30(void);
extern void asm_isr_31(void);

extern void asm_irq_0(void);
extern void asm_irq_1(void);
extern void asm_irq_2(void);
extern void asm_irq_3(void);
extern void asm_irq_4(void);
extern void asm_irq_5(void);
extern void asm_irq_6(void);
extern void asm_irq_7(void);
extern void asm_irq_8(void);
extern void asm_irq_9(void);
extern void asm_irq_10(void);
extern void asm_irq_11(void);
extern void asm_irq_12(void);
extern void asm_irq_13(void);
extern void asm_irq_14(void);
extern void asm_irq_15(void);
extern void asm_irq_16(void);
#endif
#ifdef EXECINT
extern void divideZero(reg_t*);
extern void debug(reg_t*);
extern void nonMask(reg_t*);
extern void int3(reg_t*);
extern void overflow(reg_t*);
extern void bounds(reg_t*);
extern void invalidop(reg_t*);
extern void noCop(reg_t*);
extern void doubleFault(reg_t*);
extern void coSeg(reg_t*);
extern void invalidTSS(reg_t*);
extern void noSeg(reg_t*);
extern void stackFault(reg_t*);
extern void GP(reg_t*);
extern void pageFault(reg_t*);
extern void reserve(reg_t*);
extern void copError(reg_t*);
#endif
void trap_init();