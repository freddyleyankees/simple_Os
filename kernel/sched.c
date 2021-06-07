/**
 *  sched.c
 *  nadia operating system
 *  
 *  Create by Freddy kabong
 *  Copyright © 2021 FREDDYKABONG. All rights reserved.
 */

//   simple process scheduling

#include "../include/kernel.h"

void scheduler(void){
    if(running->status == READY)
        enqueue(&readyqueue, running);
    running = dequeue(&readyqueue);
}

void *getrunning(void){
    return running;
}