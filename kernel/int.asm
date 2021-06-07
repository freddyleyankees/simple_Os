extern  int_handler, kprintf, running, scheduler, getrunning

global asm_irq_ign, asm_isr_def, divideZero, debug, nonMask, int3, overflow, bounds, invalidop, noCop, doubleFault, \
coSeg, invalidTSS, noSeg, stackFault, GP, pageFault, reserve, copError,

%macro   SAVE 0
   pushad
   push ds
   push es
   push fs
   push gs
   
   mov ax,0x10
   mov ds,ax
   mov es,ax
   mov ss,ax
   mov fs,ax
   mov gs,ax
%endmacro

%macro RESTORE 0

   pop gs
   pop fs
   pop es
   pop ds
   popad
   add esp, 8
   iret
%endmacro

common_int:
   SAVE
   mov ebx, dword [running] ; ebx -> proc
	mov dword [ebx+4], esp ; save esp to proc.kesp
   push esp
   call int_handler
   pop esp
   mov ebx, dword [running] ; ebx -> running proc
	mov esp, dword [ebx+4] ; load sp with proc.kesp
   RESTORE


;routine exception

%macro ISR_HANDLER_NOERRCODE 1  
   [GLOBAL asm_isr_%1]
   asm_isr_%1:
      push byte 0
      push byte %1
      jmp common_int
%endmacro

%macro ISR_HANDLER_ERRCODE 1
   [GLOBAL asm_isr_%1]
   asm_isr_%1:
      push byte %1
      jmp common_int
%endmacro


%macro IRQ_HANDLER 2  
   [GLOBAL asm_irq_%1]
   asm_irq_%1:
      push byte 0
      push byte %2
      jmp common_int
%endmacro

ISR_HANDLER_NOERRCODE 0
ISR_HANDLER_NOERRCODE 1
ISR_HANDLER_NOERRCODE 2
ISR_HANDLER_NOERRCODE 3
ISR_HANDLER_NOERRCODE 4
ISR_HANDLER_NOERRCODE 5
ISR_HANDLER_NOERRCODE 6
ISR_HANDLER_NOERRCODE 7
ISR_HANDLER_ERRCODE 8
ISR_HANDLER_NOERRCODE 9
ISR_HANDLER_ERRCODE 10
ISR_HANDLER_ERRCODE 11
ISR_HANDLER_ERRCODE 12
ISR_HANDLER_ERRCODE 13
ISR_HANDLER_ERRCODE 14
ISR_HANDLER_NOERRCODE 15
ISR_HANDLER_NOERRCODE 16
ISR_HANDLER_NOERRCODE 17
ISR_HANDLER_NOERRCODE 18
ISR_HANDLER_NOERRCODE 19
ISR_HANDLER_NOERRCODE 20
ISR_HANDLER_NOERRCODE 21
ISR_HANDLER_NOERRCODE 22
ISR_HANDLER_NOERRCODE 23
ISR_HANDLER_NOERRCODE 24
ISR_HANDLER_NOERRCODE 25
ISR_HANDLER_NOERRCODE 26
ISR_HANDLER_NOERRCODE 27
ISR_HANDLER_NOERRCODE 28
ISR_HANDLER_NOERRCODE 29
ISR_HANDLER_NOERRCODE 30
ISR_HANDLER_NOERRCODE 31

IRQ_HANDLER 0, 32
IRQ_HANDLER 1, 33
IRQ_HANDLER 2, 34
IRQ_HANDLER 3, 35
IRQ_HANDLER 4, 36
IRQ_HANDLER 5, 37
IRQ_HANDLER 6, 38
IRQ_HANDLER 7, 39
IRQ_HANDLER 8, 40
IRQ_HANDLER 9, 41
IRQ_HANDLER 10, 42
IRQ_HANDLER 11, 43
IRQ_HANDLER 12, 44
IRQ_HANDLER 13, 45
IRQ_HANDLER 14, 46
IRQ_HANDLER 15, 47
IRQ_HANDLER 16, 96

divideZero:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err0
   call kprintf
   pop eax
   jmp excepptr
debug:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err1
   call kprintf
   pop eax
   jmp excepptr
nonMask:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err2
   call kprintf
   pop eax
   jmp excepptr
int3:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err3
   call kprintf
   pop eax
   jmp excepptr
overflow:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err4
   call kprintf
   pop eax
   jmp excepptr
bounds:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err5
   call kprintf
   pop eax
   jmp excepptr
invalidop:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err6
   call kprintf
   pop eax
   jmp excepptr
noCop:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err7
   call kprintf
   pop eax
   jmp excepptr
doubleFault:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err8
   call kprintf
   pop eax
   jmp excepptr
coSeg:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err9
   call kprintf
   pop eax
   jmp excepptr
invalidTSS:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err10
   call kprintf
   pop eax
   jmp excepptr
noSeg:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err11
   call kprintf
   pop eax
   jmp excepptr
stackFault:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err12
   call kprintf
   pop eax
   jmp excepptr
GP:
   cli
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err13
   call kprintf
   pop eax
   jmp excepptr
pageFault:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err14
   call kprintf
   pop eax
   jmp excepptr
reserve:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err15_31
   call kprintf
   pop eax
   jmp excepptr
copError:
   push ebp
   mov ebp, esp
   mov edi, dword[ebp+8]
   push err16
   call kprintf
   pop eax
   jmp excepptr
excepptr:
   push dword[edi+11*4]
   push errcode
   call kprintf
   pop eax
   pop eax
   hlt
   mov esp, ebp
   pop ebp
ret

err0: db 10,"EXECPTION DIVIDE BY ZERO",10,0
err1: db 10,"EXECPTION DEBUG PROGRAM",10,0
err2: db 10,"EXECPTION RESERVED NON MASK",10,0
err3: db 10,"EXECPTION BREAK POINT",10,0
err4: db 10,"EXECPTION OVERFLOW",10,0
err5: db 10,"EXECPTION BOUNDS CHECK",10,0
err6: db 10,"EXECPTION OPCODE",10,0
err7: db 10,"EXECPTION DEVICE NOT AVAILABLE",10,0
err8: db 10,"EXECPTION DIOUBLE FAULT",10,0
err9: db 10,"EXECPTION coprocessor segment overRUN",10,0
err10: db 10,"EXECPTION invalid TSS",10,0
err11: db 10,"EXECPTION segment not present",10,0
err12: db 10,"EXECPTION stack segment",10,0
err13: db 10,"EXECPTION general protection",10,0
err14: db 10,"EXECPTION page fault",10,0
err15_31: db 10,"EXECPTION RESERVED INTEL",10,0
err16: db 10,"EXECPTION FLoating point error)",10,0
errcode: db 10,"ERROR please debug code %x",10,0
errt: db 10,"tswitch ERROR please debug",10,0
val: db 10,"int %x",10,0
pid: db 10,"proc pid %x",10,0
stack: db 10,"kstack interrupt %x",10,0
reg: times 20 dd 0