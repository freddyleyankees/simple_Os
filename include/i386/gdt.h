#define GDTSIZE 5
struct gdt_desc										// segment _descriptor gdt
{
	unsigned short limit0_15;
	unsigned short address0_15;
	unsigned char address16_23;
	unsigned char access;
	unsigned char limit16_19:4;
	unsigned char other:4;
	unsigned char address24_31;

} __attribute__ ((packed));

struct gdtr
{
	unsigned short limit;
	unsigned int address;
};

struct tss_desc {
    unsigned short    p_task, u_p_task;
    unsigned int    esp0;
    unsigned short    ss0, u_ss0;
    unsigned int    esp1;
    unsigned short    ss1, u_ss1;
    unsigned int    esp2;
    unsigned short    ss2, u_ss2;
    unsigned int    cr3;
    unsigned int    eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    unsigned short    es, u_es;
    unsigned short    cs, u_cs;
    unsigned short    ss, u_ss;
    unsigned short    ds, u_ds;
    unsigned short    fs, u_fs;
    unsigned short    gs, u_gs;
    unsigned short    ldt, u_ldt;
    unsigned short    debug, iomap;
} __attribute__ ((packed));

#ifdef GDT
	unsigned int entryStart = 0;
	struct gdt_desc *gdt;
	struct tss_desc tss;
#else
	extern unsigned int entryStart;
	extern struct gdt_desc *gdt;
	extern struct tss_desc tss;
#endif

#define ltr(a) asm volatile("movl %0, %%eax; ltr %%ax"::"r"(a));
void set_gdt(unsigned int, unsigned int, unsigned int, unsigned char, unsigned char);
void set_gdt_size(unsigned int, unsigned int);
void get_gdt_size(unsigned int, unsigned int);
void init_gdt();