NASMPARAM = -f elf -o

NASMBINPARAM = -f bin -o

GCCPARAM = -g -m32 -nostdlib -nostdinc -fno-pie -fno-builtin -fno-stack-protector \
-nostartfiles  -nodefaultlibs -ffreestanding -std=c99 -c

LDPARAM = -melf_i386 --oformat binary --entry=_start -Ttext=100000
ULDPARAM = -melf_i386 --oformat binary --entry=_main -Ttext=200000

OBJECT = setup.o main.o string.o con.o v_printf.o kprintf.o pic.o gdt.o idt.o int.o intr.o \
kbd.o ramdisk.o disk.o ehandler.o syscall.o pit.o kernel.o proc.o sched.o

O_FBOOT = booter.bin
O_SBOOT = inibtos.bin

RAMDISK = ramdisk.img
KERNEL = setup.bin
IMAGE = nadia.img
UTASK = task.bin

%.o:../init/%.c
	gcc $(GCCPARAM) $<

%.o:../kernel/%.c
	gcc $(GCCPARAM) $<

%.o:../kernel/%.asm
	nasm $(NASMPARAM) $@ $<

%.o:../lib/%.c
	gcc $(GCCPARAM) $<

%.o:../drivers/kbd/%.c
	gcc $(GCCPARAM) $<

%.o:../drivers/ramdisk/%.c
	gcc $(GCCPARAM) $<

%.o:../boot/setup/%.asm
	nasm $(NASMPARAM) $@ $<

setup.bin: $(OBJECT)
	ld $(LDPARAM) $^ -o $@

task.bin:
	make all -C ../usr
ramdisk.img:
	#create disk floppy
	mkdosfs -C $@ 144
	mkdosfs -F 12 $@
#add file system ramdisk as root

all:  $(RAMDISK) $(KERNEL) $(UTASK)
	mkdosfs -C $(IMAGE) 1440;
	mkdosfs -F 12 $(IMAGE);
	sudo losetup /dev/loop0 $(IMAGE);
	sudo mount -t vfat /dev/loop0 /mnt;
	sudo cp $(O_SBOOT) $^ /mnt;
	sudo umount /mnt;
	sudo losetup -d /dev/loop0;
	dd if=$(O_FBOOT) of=$(IMAGE) bs=512 count=1 conv=notrunc
	cp $(IMAGE) ..