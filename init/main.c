/**
 *  main.c
 *  nadia operating system
 *  
 *  Create by Freddy kabong
 *  Copyright © 2021 FREDDYKABONG. All rights reserved.
 */

#include "../include/kernel.h"
#include "../include/io.h"
#include "../include/string.h"
#include "../include/con.h"
#include "../include/syscall.h"
#include "../include/i386/gdt.h"
#include "../include/i386/int.h"
#include "../include/disk.h"
#include "../drivers/kbd/kbd.h"

void main(struct boot_desc* sys)
{
    
    cursorEnable(0xE,0xF);
    clearCon();
    idt = (struct idt_desc*) sys->idt;
    gdt = (struct gdt_desc*) sys->gdt;
    init_gdt();
    inidt();
    trap_init();
    initTimer(100);
    init_kbd();
    i_syscall();
    map_i386_active_irq();
    cursorEnable(0xE,0xF);
    clearCon();
    kprintf("=========== NADIA OS ===========\n");
    kprintf("SHEDULING PROCESS\n");
    init_proc();
    kfork();
    kprintf("call tswitch()\n");
    if(readyqueue)
        dotswitch();
    kprintf("go back to main\n");
    kprintf("\nterminer return to main %x %x\n", tss.ss0,  tss.esp0);
  
    return;
} 