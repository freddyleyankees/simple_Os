extern  main, exit

global _main0

_main0:
	push esp
	call main
	pop eax
	jmp $