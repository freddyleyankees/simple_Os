;
[BITS 16]
[ORG 0x0]

%define ROOTSEG     0x8B20
%define FILE1SEG     0x1000
%define FILE2SEG     0x3000
%define FILE3SEG     0x4000
%define SETUPSEG    0x9020
%define INITSEG     0x9000
%define FATSEG      0x8B40
%define GDTSEG  0x8B28
%define IDTSEG  0x8AA8
; read boot module on /boot/boot
jmp setup
%include 'lib.inc'
getSize:
    push bp
    mov bp, sp
    mov ax, word [bp+4]
    mov di, ax
    mov ax, ROOTSEG
    mov es, ax
    xor bx, bx
    mov bx, word [rootEntryfile]
    mov ax, word [es:bx+0x1C]
    mov word [di], ax
    mov ax, word [es:bx+0x1E]
    mov word [di+2], ax
    mov sp, bp
    pop bp
ret

getCluster:
    push bp
    mov bp, sp
    mov ax, word [bp+4]
    mov di, ax
    mov ax, ROOTSEG
    mov es, ax
    xor bx, bx
    mov bx, word [rootEntryfile]
    mov ax, word [es:bx+0x1A]
    mov word [di], ax
    mov sp, bp
    pop bp
ret
setup:
    ;# end init segment
    mov ax, SETUPSEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0xFDFF
    
    mov si, SecondbootMsg
    call print

    call resetDiskSystem
    ;# Find filename into ROOT DIRECTORY TABLE
;=============================================
    push word filename1
    push ROOTSEG
    call findFile

    ;# Copy FAT into memory
    
    ;# Read file into memory
    push FATSEG
    push FILE1SEG
    call readFile

    mov si, filename1
    call print
    push fileSize
    call getSize
    push es
    mov ax, INITSEG
    mov es, ax
    xor di, di
    mov ax, word [fileSize]
    mov [es:di], ax
    mov ax, word [fileSize+2]
    add di, 2
    mov [es:di], ax
    pop es

;===================================
    push word filename3
    push ROOTSEG
    call findFile

    ;# Read file into memory
    push FATSEG
    push FILE2SEG
    call readFile
    

    mov si, filename3
    call print

    push fileSize
    call getSize

    push es
    mov ax, INITSEG
    mov es, ax
    xor di, di
    add di, 4
    mov ax, word [fileSize]
    mov [es:di], ax
    mov ax, word [fileSize+2]
    add di, 2
    mov [es:di], ax
    pop es

;========================================

push word filename2
    push ROOTSEG
    call findFile

    ;# Read file into memory
    push FATSEG
    push FILE3SEG
    call readFile
    

    mov si, filename2
    call print

    push fileSize
    call getSize

    push es
    mov ax, INITSEG
    mov es, ax
    xor di, di
    add di, 8
    mov ax, word [fileSize]
    mov [es:di], ax
    mov ax, word [fileSize+2]
    add di, 2
    mov [es:di], ax
    pop es
;========================================
    jmp EA20 ;disable interrupt

; Setup Global Descriptor Table
gdtSetup:
    ;# Get size
    mov ax, 24
    mov word [GDTSTRUCT], ax
    ;# Get linear address
    xor eax, eax
    mov ax, GDTSEG
    shl eax, 4
    mov dword [GDTSTRUCT+2], eax

    ;# Init each segment descriptor
    xor eax, eax
    mov ax, GDTSEG
    mov es, ax
    xor ax, ax
    xor di, di
    ;# NULL segment Descriptor
    mov cx, 4
    cld
    rep stosw

    ;# Code segment descriptor

    mov word [es:di], 0x0FFF         ;# limit 15:0 [2 bytes]
    mov word [es:di+2], 0x0000       ;# base 15:0 [2 bytes]
    mov byte [es:di+4], 0x00         ;# base 23:16 [1 byte]
    mov byte [es:di+5], 0x9A         ;# type [1 byte]
    mov byte [es:di+6], 0xC0         ;# granilarity [1 byte]
    mov byte [es:di+7], 0x00         ;# base 31:24 [1 byte]
    add di, 8

    ;# Data segment descriptor

    mov word [es:di], 0x0FFF         ;# limit 15:0 [2 bytes]
    mov word [es:di+2], 0x0000       ;# base 15:0 [2 bytes]
    mov byte [es:di+4], 0x00         ;# base 23:16 [1 byte]
    mov byte [es:di+5], 0x92         ;# type [1 byte]
    mov byte [es:di+6], 0xC0         ;# granilarity [1 byte]
    mov byte [es:di+7], 0x00         ;# base 31:24 [1 byte]
    ret

; Check A20 is Actived
checkA20:
    pushf
    push ds
    push es
    push di
    push si
    
    xor ax, ax                         ;# Set es:di = 0000:0500
    mov es, ax
    mov di, 0x0500

    not ax                             ;# Set ds:si = ffff:0510
    mov ds, ax
    mov si, 0x0510

    mov al, byte [es:di]           ;# Save byte at es:di on stack.
    push ax

    mov al, byte [ds:si]           ;# Save byte at ds:si on stack.
    push ax

    mov byte [es:di], 0x00         ;# [es:di] = 0x00
    mov byte [ds:si], 0xFF         ;# [ds:si] = 0xff

    cmp byte [es:di], 0xFF         ;# Did memory wrap around?

    pop ax
    mov byte [ds:si], al           ;# Restore byte at ds:si
 
    pop ax
    mov byte [es:di], al           ;# Restore byte at es:di

    mov ax, 0
    je checkA20exit                 ;# If memory wrapped around, return 0.

    mov ax, 1                          ;# else return 1.
    
checkA20exit:
    pop si
    pop di
    pop es
    pop ds
    popf
    ret

;# Actived A20 by BIOS
setA20Bios:
    mov ax, 0x2401
    int 0x15
    ret

;# read Command keyboard
wait8042Command:
    in      al,0x64
    test    al,2
    jnz     wait8042Command
    ret
;# read data keyboard
wait8042Data:
    in      al,0x64
    test    al,1
    jz      wait8042Data
    ret

setA20Keyboard:
    cli     ;# disable interrupt
    call    wait8042Command  ;# When controller ready for command
    mov     al,0xAD            ;# Send command 0xad (disable keyboard).
    out     0x64,al

    call    wait8042Command  ;# When controller ready for command
    mov     al,0xD0            ;# Send command 0xd0 (read from input)
    out     0x64,al

    call    wait8042Data     ;# When controller has data ready
    in      al,0x60            ;# Read input from keyboard
    push    eax                ;# ... and save it

    call    wait8042Command  ;# When controller is ready for command
    mov     al,0xD1            ;# Set command 0xd1 (write to output)
    out     0x64,al

    call    wait8042Command  ;# When controller is ready for command  
    pop     eax                ;# Write input back, with bit #2 set
    or      al,2
    out     0x60,al

    call    wait8042Command  ;# When controller is ready for command
    mov     al,0xAE            ;# Write command 0xae (enable keyboard)
    out     0x64,al

    call    wait8042Command  ;# Wait until controller is ready for command
    sti
    ret

setA20FastGate:
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

EA20:
    call checkA20
    cmp ax, 0
    jne enableA20Done
    call setA20Bios
    call checkA20
    cmp ax, 0
    jne enableA20Done
    call setA20Keyboard
    call checkA20
    cmp ax, 0
    jne enableA20Done
    call setA20FastGate
    call checkA20
    xchg bx, bx
    cmp ax, 0
    jne enableA20Done
enableA20Fail:
    mov si, errorA20Msg
    call print
    call reboot
enableA20Done:

pm:
    cli     ;# disable interrupt
    call gdtSetup
    lgdt [GDTSTRUCT]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp j
j:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x9FFFF
    
    mov edi, INITSEG
    shl edi, 4
;== copy kernel 
    push edi
    mov ebx, edi
    mov esi, FILE1SEG
    shl esi, 4
    mov edi, 0x100000
    mov edx, edi
    mov ecx, dword [ebx]
    call Copy
    pop edi
    mov dword[edi], edx
;== copy ramdisk
    mov ebx, edi
    push edi
    mov esi, FILE3SEG
    shl esi, 4
    mov edi, 0x007FFFFF
    sub edi, dword [ebx+8]
    push edi
    mov ecx, dword [ebx+8]
    call Copy
    pop esi
    pop edi
    mov dword[edi+8], esi
;== copy utask
    mov ebx, edi
    push edi
    mov esi, FILE2SEG
    shl esi, 4
    mov edi, 0x00200000
    push edi
    mov ecx, dword [ebx+4]
    call Copy
    pop esi
    pop edi
    mov dword[edi+4], esi

    push edi
end: jmp dword 0x8:0x00100828
Copy:
    
    mov eax, dword[esi]
    mov dword[edi], eax
    add edi, 4
    add esi, 4

    loopnz Copy
    ret
; BootFailure 
bootFailure:
    mov si, diskError
    call print
    call reboot

    endsms             db 'End interrupt.',13,10,0
    oksms             db 'ok.',13,10,0
    errorA20Msg             db 'A20 Unavailable.',13,10,0
    SecondbootMsg           db "startup.bin load!", 13, 10, 0
    filename1                db "SETUP   BIN", 13, 10, 0
    filename2                db "RAMDISK IMG", 13, 10, 0
    filename3                db "TASK    BIN", 13, 10, 0
    no_found_file           db "File missing : ", 0
    diskError               db "Disk error. ", 13, 10, 0
    rebootMsg               db "Press any key to reboot. ", 13, 10, 0
    rootDirectoryTableSize  dw 0
    rootDirectoryTable      dw 0
    fileStart               dw 0
    fileSize                dd 0
    rootEntryfile           dw 0
    buf           db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0

GDTSTRUCT:
    dw 0
    dd 0
IDTSTRUCT:
    dw 0
    dd 0

    times 512*4-($-$$) db 0