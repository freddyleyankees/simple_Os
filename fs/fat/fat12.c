#include "fat12.h"


unsigned int memPostion =  KERNELSEGMENT;

struct rootDirTable*  findFile(struct rootTable, unsigned char*);

struct buffer_data* getBuffer(void);
struct buffer_data* getBuffer(void){
    unsigned char i =0;
    for(i=0;i<256;i++)
        if(!queue[i].flag){
            queue[i].flag = 1;
            break;
        }
}

static void* read_cluster(unsigned int sector, unsigned int size){
    unsigned char* buf;
    buf = fdc_read(sector);
    return buf;
}


struct rootDirTable*  findFile(struct rootTable rootseg, unsigned char* file){ 
    int i;
    unsigned short cluster;
    struct rootDirTable* p = rootseg.start;
    
    char file_flag = 0;
    for(int k=0;k<224;k++){
        if(memcmp((unsigned char*) p[k].filename,(unsigned char*)file,11)){
            i=k;
            file_flag = 1;
            break;
        }
    }
    struct rootDirTable* chunk = (struct rootDirTable*) &p[i];
    if(file_flag){
        writeText("\n - file found ok\n");
        return chunk;
    }

    // writeText("\n - root table loading complet ok\n");
    // i = 0;
    // struct rootDirTable* tmp = fileTable;
    // unsigned char* chunk = fileNameTable;
    // while(*(tmp->filename)){
    //     if(i>=224)
    //         break;
    //     memcpy(chunk,(unsigned char*) tmp->filename,11);
    //     tmp += 32;
    //     chunk += 12;
    //     ++i;
    //     writeText(chunk);
    //     writeText(" ok \n");
    // }   
    writeText(" file not found\n");
    return (struct rootDirTable*) 0;
}

void initFat12(struct configSys* sys){
    // init fat 
    fatseg.start = sys->memFat;
    fatseg.size = blockParamBoot.SectorsPerFat;
    // copy 16 first rootTable entries on memory
    root.size = ROOTENTRYSIZE * blockParamBoot.EntriesRootDir/blockParamBoot.SectorSize;
    root.entry = blockParamBoot.SectorsPerFat*blockParamBoot.NFat + blockParamBoot.SectorsHidden + blockParamBoot.SectorsRsvd;;
    root.start = (struct rootDirTable*) sys->memRooTable;
    

}

unsigned short getNextCluster(unsigned short n){
    unsigned short offset = (n*3)/2;
    unsigned short* fat = (unsigned short*) (fatseg.start + offset);
    
    if(n%2==0)
        return *fat & 0x0FFF;
    else
        return *fat >> 4;
}

int readfile(unsigned char* filename, unsigned int seg){
    struct rootDirTable* file = findFile(root, filename);
    if(!file)
        return 0;
    unsigned short cluster = file->cluster;
    unsigned char* buf;
    do{
        unsigned char entrysector = root.entry+root.size+cluster*blockParamBoot.SectorsPerCluster-2;
        // do chunck data
        buf = fdc_read(entrysector);
        // read data
        memcpy((unsigned char*) seg, buf, 512);
        cluster = getNextCluster(cluster);
        writeText(" . ");
        seg += 512;
    }while(cluster < 0xff0);
    return 1;
}

/** ======================== [ end ] ======================== */