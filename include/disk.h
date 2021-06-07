#define SECSIZE 512
struct partition{
	unsigned char booter;
	unsigned char shead;
	unsigned char ssec;
	unsigned short scyl;
	unsigned char fsID;
	unsigned char ehead;
	unsigned char esec;
	unsigned short ecyl;
	unsigned int slba;
	unsigned int length;
}__attribute__ ((packed));

struct mbr{
	unsigned char bootcode[440];
	unsigned int signature;
	unsigned char unused;
	struct partition part[4];
	unsigned short magic_num;	
}__attribute__ ((packed));

struct parthd {
	unsigned int num;
	unsigned int start;
	unsigned int end;
};

struct dsk_info{
	unsigned char name[11];
};

struct disk_st{
	unsigned int num;
	unsigned int master;
	unsigned int sectors;
	struct parthd part[4]; 
	struct dsk_info* info;
};
#ifdef DISK
struct disk_st hd[2];
#else
extern struct disk_st hd[2];
#endif
void initDisk();
void readMbr (unsigned int n);