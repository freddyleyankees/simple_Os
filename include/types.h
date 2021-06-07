#ifndef asm
#define asm __asm__
#endif

typedef struct boot_desc
{
	unsigned int startMemoryKernel;
	unsigned int startMemoryInitProc;
	unsigned int startMemoryInitrd;
	unsigned int gdt;
	unsigned int idt;
} __attribute__ ((packed)) boot_desc;