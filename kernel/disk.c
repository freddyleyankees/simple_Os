#define DISK
#include "../include/disk.h"
#include "../drivers/ramdisk/ramdisk.h"
typedef void (*op_r)(unsigned int, unsigned char*);
struct disk{
	struct disk_st* dsk;
	op_r read;
	op_r write;
};

struct disk* initrd,  *run_disk;

void initDisk(){

	for(int i = 0;i<2;i++){
		hd[i].num = i;
		hd[i].master = !i?1:0;
		hd[i].sectors = 0;
		hd[i].info = 0;
		for(int j=0;j<4;j++){
			struct parthd* p= &(hd[i].part[j]);
			p->num = j;
			p->start = 0;
			p->end = 0;
		}
	}
	initrd->dsk = &hd[0];
	initrd->dsk->sectors = 288;
	initrd->dsk->info = 0;
	initrd->read = &readSectorRmdsk;
	initrd->write = &writeSectorRmdsk;
	run_disk = &initrd;

	// read first sector
	struct mbr mbr;
	run_disk->read(1, (unsigned char*)&mbr);

}