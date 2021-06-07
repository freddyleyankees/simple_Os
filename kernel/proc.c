/**
 *  proc.c
 *  nadia operating system
 *  
 *  Create by Freddy kabong
 *  Copyright © 2021 FREDDYKABONG. All rights reserved.
 */

#define CPU_PROC
#include "../include/kernel.h"
#include "../include/io.h"

/**
 * initializing all process 
 * @module init_proc
 * 
*/
int init_proc(void) {
	process *p;
	int i,j;
	for(i=0;i<NPROC;i++){
		p = &proc[i];	// specifie le processus en cours de traitement
		p->pid = i;		// marquer la valeur i pour le ieme processus
		p->next = &proc[i+1];	// pointer sur le prochain processus 
		p->status = FREE;
		p->priority = 0;
	}
	proc[NPROC - 1].next = 0; // revient sur le processus lorsqu'on parcouru tous les processus!
	freequeue = &proc[0]; // on active le processus 0 comme 1er processus
	readyqueue = 0;
	p = get_proc(&freequeue);
	p->status = READY;
	p->ppid = 0;
	running = p;
	kprintf("init complete ... !\n");
}
