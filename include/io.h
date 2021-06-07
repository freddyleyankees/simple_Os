/**
* Nadia OS descriptor Header
*
* this file describe input output port
*
* @author : kabong freddy
*/

#ifndef IO_H
#define IO_H
#ifndef asm
#define asm __asm__
#endif
/*fdesactive les interruptions */
#define cli asm volatile("cli"::)

/* reactive les interruptions */
#define sti asm volatile("sti"::)
#define hlt asm volatile("hlt"::)

#define intx86(x) asm volatile ("int %0" :: "i" (x))

/* ecrit un octet sur un port */
#define outb(port,value) \
	asm volatile ("outb %%al, %%dx" :: "d" (port), "a" (value));

/* lit un octet sur un port */
#define inb(port) ({    \
	unsigned char _v;       \
	asm volatile ("inb %%dx, %%al" : "=a" (_v) : "d" (port)); \
        _v;     \
})

/* ecrit un octet sur un port et marque une temporisation  */
#define outbp(port,value) \
	asm volatile ("outb %%al, %%dx; jmp 1f; 1:jmp 1f; 1:" :: "d" (port), "a" (value));

#define inbp(port) ({    \
	unsigned char _v;       \
	asm volatile ("inb %%dx, %%al; jmp 1f; 1:jmp 1f; 1:" : "=a" (_v) : "d" (port)); \
        _v;     \
})

/* ecrit un mot de 16 bits sur un port */
#define outw(port,value) \
	asm volatile ("outw %%ax, %%dx" :: "d" (port), "a" (value));

/* lit un mot de 16 bits sur un port */
#define inw(port) ({		\
	unsigned short _v;			\
	asm volatile ("inw %%dx, %%ax" : "=a" (_v) : "d" (port));	\
        _v;			\
})

/* ecrit un octet sur un port et marque une temporisation  */
#define outwp(port,value) \
	asm volatile ("outw %%ax, %%dx; jmp 1f; 1:jmp 1f; 1:" :: "d" (port), "a" (value));
	
#define inwp(port) ({    \
	unsigned short _v;       \
	asm volatile ("inw %%dx, %%ax; jmp 1f; 1:jmp 1f; 1:" : "=a" (_v) : "d" (port)); \
        _v;     \
})

/* ecrit un mot de 16 bits sur un port */
#define outl(port,value) \
	asm volatile ("outl %%eax, %%dx" :: "d" (port), "a" (value));

/* lit un mot de 32 bits sur un port */
#define inl(port) ({		\
	unsigned int _v;			\
	asm volatile ("inl %%dx, %%eax" : "=a" (_v) : "d" (port));	\
        _v;			\
})

/* ecrit un octet sur un port et marque une temporisation  */
#define outlp(port,value) \
	asm volatile ("outl %%eax, %%dx; jmp 1f; 1:jmp 1f; 1:" :: "d" (port), "a" (value));

#define inlp(port) ({    \
	unsigned int _v;       \
	asm volatile ("inl %%dx, %%eax; jmp 1f; 1:jmp 1f; 1:" : "=a" (_v) : "d" (port)); \
        _v;     \
})
#define nop() asm volatile ("nop"::);

#endif