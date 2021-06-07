
%macro .STARTUP 0
    global _start
    _start:
%endmacro

%macro .DATA 0
    segment .data
%endmacro

%macro .UDATA 0
    segment .bss
%endmacro

%macro .EXIT 0
    jmp $
%endmacro

%macro .CODE 0
    segment .data
    segment .bss
    segment .text
%endmacro

%macro SAVE_REGS 0
   pushad
   mov ax, ds
   push eax
   
   mov ax,0x10
   mov ds,ax
   mov es,ax
   mov fs,ax
   mov gs,ax
%endmacro

%macro RESTORE_REGS 0
   pop eax
   mov ds,ax
   mov es,ax
   mov fs,ax
   mov gs,ax
   popad
%endmacro

%define IRQCLK 32*8
%define IRQKBD 33*8

extern map_i386_irq, map_i386_mask_irq, map_i386_active_irq, main, kprintf, procSize, proc
.DATA
    %define IRQCLK 32*8
    %define IRQKBD 33*8
    startmsg db "let's go",10,0
.UDATA
.STARTUP
    .CODE
    _idt: times 0x800 db 0
    _gdt: times 8*5 db 0
    jmp _startup
;// ============ default interrupt vector routine
ignore_int:
nop
ret

handler_int:
    SAVE_REGS
    call ignore_int
    mov al,0x20
    out 0x20,al
    RESTORE_REGS
    iret

_startup:
    pop edi
    mov dword [stack_ptr], edi
  
    ;# configure pic

    call map_i386_irq

    ;# mask interrupt IRQ

    call map_i386_mask_irq

    ; configure IDT

idtsetup:
    ;# get linear address idt

    mov edi, idtptr
    mov word [edi], 0x800
    mov esi, _idt
    mov dword [edi+2], esi

    ;# Init IDT
    
    mov edi, _idt

    ;# Set address int handler

    mov ebx, handler_int

    ;# Set segment selector

    mov eax, 0x00080000 ;# cs=0x0008 and offset=0x0000
    mov ax, bx  ;# 0-15 offset

    ;# Set type
    mov bx, 0x8E00
    mov ecx, 0xFF

    ;# Load IDTR
.f:
    mov dword [edi], eax
    mov dword [edi+4], ebx
    add edi, 8
    dec ecx
    jne .f
    mov edi, idtptr
    lidt [edi]

   ;# configure GDT
gdtsetup:
    ;# get linear address gdt
    mov edi, gdtptr
    mov word [edi], 0x30                                    ; set limit gdt
    mov esi, _gdt
    mov dword [edi+2], esi                                  ; set address gdt

    mov edi, _gdt
    ;# NULL segment Descriptor
    mov cx, 4
    cld
    rep stosw

    ;# Code segment descriptor [size: 16Mo]

    mov word [edi], 0x0FFF         ;# limit 15:0 [2 bytes]
    mov word [edi+2], 0x0000       ;# base 15:0 [2 bytes]
    mov byte [edi+4], 0x00         ;# base 23:16 [1 byte]
    mov byte [edi+5], 0x9A         ;# type [1 byte]
    mov byte [edi+6], 0xC0         ;# granilarity [1 byte]
    mov byte [edi+7], 0x00         ;# base 31:24 [1 byte]
    add edi, 8

    ;# Data segment descriptor [size: 16Mo]

    mov word [edi], 0x0FFF         ;# limit 15:0 [2 bytes]
    mov word [edi+2], 0x0000       ;# base 15:0 [2 bytes]
    mov byte [edi+4], 0x00         ;# base 23:16 [1 byte]
    mov byte [edi+5], 0x92         ;# type [1 byte]
    mov byte [edi+6], 0xC0         ;# granilarity [1 byte]
    mov byte [edi+7], 0x00         ;# base 31:24 [1 byte]
    add edi, 8

    
    ;# Load GDTR
    mov edi, gdtptr
    lgdt [edi]

    ;# init segments registers
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x0009FFFF

    ;# fill 0 into 4Ko for stack
    mov edi, esp
    std
    xor eax, eax
    mov ecx, 0x800
    rep stosw

;# active Interrupt Request Query

    
    mov edi, dword[stack_ptr]
    mov dword[edi+12], _gdt
    mov dword[edi+16], _idt
    push startmsg
    call kprintf
    pop eax
    mov esp, proc
    add esp, dword [procSize]
    push edi
    call main
    pop eax
    .EXIT
;# define IDT and GDT structure
    idtptr:
        dw 0
        dd 0
    gdtptr:
        dw 0
        dd 0
stack_ptr: dd 0
val db 10,"la valeur de proc0 %x",10,0