#include "ramdisk.h"
#include "../../include/disk.h"
#include "../../include/kernel.h"


void readSectorRmdsk(unsigned int sector, unsigned char *buf){
	kprintf(" disk read function\n");
}

void writeSectorRmdsk(unsigned int sector, unsigned char *buf){
	kprintf(" disk write function\n");
}