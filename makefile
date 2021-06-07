RAMDISK = ramdisk.img
build: builds
	sudo rm -f ./builds/*.img;
	make all -C ./boot
	make start
	#(killall virtualbox && sleep 1) || true
	#virtualbox --startvm nadia &
	qemu-system-x86_64 -boot a -fda $(IMAGE) ;#-no-fd-bootchk;
	
start: 
	make all -C ./builds 
		

