#include "types.h"
#define SSIZE 1024
#define NPROC 9
#define FREE 	1
#define READY 	2
#define STOP 	3
#define DEAD 	4
typedef struct process
{
	struct process* next;
	unsigned int *kesp;
	unsigned int pid;
	unsigned int status;
	unsigned int priority;
	unsigned int ppid;
	unsigned int kstack[SSIZE];
} process;
#ifdef CPU_PROC
int procSize = sizeof(process);
process proc[NPROC], *running, *readyqueue, *freequeue;
#else
extern int procSize;
extern process proc[NPROC], *running, *readyqueue, *freequeue;
#endif
int init_proc(void);
void scheduler(void);
void initTimer(unsigned int freq);
int kprintf(const char* fmt, ...);
extern unsigned char eventKeypress();
char getc(void);
process *getcurrentproc();
void *getrunning(void);
void dotswitch();
process* get_proc(process**);
process* dequeue(process**);
int put_proc(process**, process*);
int enqueue(process**, process*);
void kfork();
void print(char* name, process**list);