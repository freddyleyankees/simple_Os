#include "include/syscall.h"
#include "../include/stdarg.h"
static char buffer[1024] = {0,};
extern int v_printf(const char* s,va_list arg, char* buf);
static inline int syscall(int a, int b, int c, int d){
	int _a;
	asm volatile("movl %1, %%eax; movl %2, %%ebx; movl %3, %%ecx; movl %%edx, %4;int $0x60;movl %%eax, %0":"=r"(_a):"r"(a),
		"r"(b),"r"(c), "r"(d));
	return _a;
}

int getpid(){
	return syscall(0,0,0,0);
}

int getppid(){
	return syscall(1,0,0,0);
}

int fork(){
	return syscall(3,0,0,0);;
}

int wait(){
	int child, status;
	child = syscall(9,(int)&status,0,0);
	if(child>=0)
		printf("status=%d\n", status);
}

int wakeup(int evt){
	syscall(8,evt,0,0);
}

int sleep(int evt){
	syscall(6,evt,0,0);
}

int exit(){
	printf("enter value exit: ");
	int exitValue =  geti();
}
int stop(int pid){
	syscall(4,0,0,0);
}

int continu(){
	syscall(5,0,0,0);
}

int tswitch(){
	syscall(11,0,0,0);
}

int lock(){
	syscall(12,0,0,0);
}
int chpri(){
	syscall(9,0,0,0);
}

char getc(){
	return syscall(13,0,0,0);
}

char *gets(char* s){
	syscall(16,s,0,0);
	return s;
}

int geti(){
	return syscall(14,0,0,0);
}

int getcs(){
	return syscall(15,0,0,0);
}

int getesp(){
	return syscall(17,0,0,0);
}
int printf(const char *fmt, ...){
	va_list arg;
	int i;
	start_arg(arg, fmt);
	i = v_printf(fmt, arg, buffer);
	if(!i)
		return 0;
	end_arg(arg);
	syscall(2,(int)buffer,0,0);
	return i;
}
