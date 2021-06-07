
NASMPARAM = -f elf -o
GCCPARAM = -m32 -nostdlib -nostdinc -fno-pie -fno-builtin -fno-stack-protector \
-nostartfiles  -nodefaultlibs -ffreestanding -c
ULDPARAM = -melf_i386 --oformat binary --entry=_main0 -Ttext=200000

UOBJECT = us.o uc.o syscall.o v_printf.o string.o

START = task.bin

%.o: %.c
	gcc $(GCCPARAM) $<
%.o: %.asm
	nasm $(NASMPARAM) $@ $<
%.o: ../lib/%.c
	gcc $(GCCPARAM) $<


task.bin: $(UOBJECT)
	ld $(ULDPARAM) $^ -o $@

all : $(START)
		sudo mv *.bin ../builds;
		sudo rm -f *.o
