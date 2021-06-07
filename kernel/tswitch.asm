extern running, scheduler
global tswitch
tswitch: 
SAVE: 
	pushad
	pushf
	mov ax, 0x10
	mov ds, ax
	mov ebx, dword [running] ; ebx -> proc
	mov dword [ebx+2], esp ; save esp to proc.kesp
	push ebx
FIND: call scheduler ; call scheduler() in C
RESUME: mov ebx, dword [running] ; ebx -> running proc
	mov esp, dword [ebx+2] ; load sp with proc.kesp
	popf
	popad
	iret