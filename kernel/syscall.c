#define IRQ
#include "../include/i386/int.h"
#include "../include/kernel.h"
#include "../include/syscall.h"
#include "../include/con.h"
#include "../include/io.h"
#include "../include/stdarg.h"

static char buffer[1024] = {0,};
extern int v_printf(const char* s,va_list arg, char* buf);

void print(char* name, process**list){
	kprintf("%s : ",name);
	for(process* i = *list; i;i=i->next)
		kprintf("p%d -> ", i->pid);
	kprintf("null\n");
}

int body(){ 
	char c;
	int pid = running->pid;
	kprintf("\n======  process %d resume to body ========\n", pid);
	while (1)
	{
		kprintf("porc %d running, enter a key : ", pid);
		// system call getc, wait to press any key
		c = eventKeypress();
		kprintf("keyboard pressed %c", c);
		dotswitch();
	}
	
}
static inline void init0(void){
	kprintf("execute process 0");

}

static int __fork(void){
	int i;
	process *p = get_proc(&freequeue);
	if(!p){
		kprintf("aucune processus disponible!");
		return 0;
	}
	p->status = READY;
	p->priority = 1;
	p->ppid = running->pid;
	for(i=6;i<16;i++)
		p->kstack[SSIZE - i] = 0;
	p->kstack[SSIZE - 5] = (int) &body; // specifie l'adresse de retour sur le code!
	p->kstack[SSIZE - 4] = 0x8;			// specifie l'adresse segment de code!
	p->kstack[SSIZE - 3] = 0x200;		// specifie le registre de status(flag)!
	p->kstack[SSIZE - 2] = 0x10;		// specifie l'adresse segment de pile (stack)!
	p->kstack[SSIZE - 1] = 0x10;		// specifie l'adresse segment du pointeur de pile (stack pointer)!
	p->kstack[SSIZE - 16] = 0x10;
	p->kstack[SSIZE - 17] = 0x10;
	p->kstack[SSIZE - 18] = 0x10;
	p->kstack[SSIZE - 19] = 0x10;
	p->kesp = &(p->kstack[SSIZE - 19]); // le pointer de pile pointe la dernière valeur du contexte de la pile du processus
	enqueue(&readyqueue, p);
	return p;
}

static int dosyscall(reg_t* reg){
	unsigned int r = 0;
	kprintf(" == [ CALL SYSTEM NUM %x ] == \n", reg->eax);
	switch (reg->eax)
	{
	case 1:
		scheduler();
		break;
	case 2:
		__fork();
		break;
	case 3:
		r = (int) eventKeypress();
		break;
	default:
		break;
	}
	return reg->eax = r;
}

void i_syscall(){
	reg_handler(IRQ16, &dosyscall);
	return;
}
