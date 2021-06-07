#include "../include/kernel.h"
#include "../include/string.h"
#include "../include/io.h"
#include "../include/i386/gdt.h"
static unsigned char bufkbd[1024] = {0,};
static inline int syscall(int a, int b, int c, int d){
	int _a;
	asm volatile("movl %1, %%eax; movl %2, %%ebx; movl %3, %%ecx; movl %%edx, %4;int $0x60;movl %%eax, %0":"=r"(_a):"r"(a),
		"r"(b),"r"(c), "r"(d));
	return _a;
}

void dotswitch(){
	syscall(1,0,0,0);
}
void kfork(){
	syscall(2,0,0,0);
}
char getc(void){
	return (char) syscall(2,0,0,0);
}

process *getcurrentproc(){
    return running;
}

/**
 * 
 * return the free element from list
 */
process* get_proc(process**list){
	process *head = *list, *p, * tmp;
	if(!head)
		return 0;
	for(process* i = head;i;i=i->next){
		if(i->status == FREE){
			p = i;
			if(i != head)
				tmp->next = i->next;
			else
				*list = i->next;
			return p;
		}
		tmp = i;
	}
	return 0;
}

/**
 * 
 * return the first element removed from queue
 */

process* dequeue(process**list){
	process* head = *list,*p;
	if (!head)
		return 0;
	p = head;
	*list = head->next;
	return p;
}
/**
 * 
 * put a process into the list
 */
int put_proc(process**list, process*p){
	process* head = *list;
	*list=p;
	p->next = head;
	return p->pid;
}
/**
 * 
 * put a element by priority into the list
 */
int enqueue(process**list, process*p){
	process *head = *list, *tmp;
	if(!head){
		*list = p;
		p->next = 0;
	}
	else{
		for(process* i=head; i; i=i->next)
		{
			if(p->priority >= i->priority){
				p->next = i;
				if(i != head)
					tmp->next = p;
				else
					*list = p;
				return p->pid;
			}
			tmp = i;
		}
	}
	tmp->next = p;
	p->next = 0;
	return p->pid;
}