
/** ======================== [ file system FAT12 ] ======================== */
struct bpb /* Bios Parameters Block */
{
    unsigned char    OEM[8];               // OEM string
    unsigned short   SectorSize;        // Bytes per sector
    unsigned char    SectorsPerCluster; // Sectors per cluster
    unsigned short   SectorsRsvd;       // #of reserved sectors
    unsigned char    NFat;              // #of fat copies
    unsigned short   EntriesRootDir;    // size of root directory
    unsigned short   TotalSector;       // total # of sectors if below 32 MB
    unsigned char    MediaType;         // Media Descriptor [floppy or hdd]
    unsigned short   SectorsPerFat;     // Size of each FAT
    unsigned short   SectorsPerTrack;   // Sectors per track
    unsigned short   NHead;             // number of read-write heads
    unsigned int     SectorsHidden;     // number of hidden sectors [sectors incompatible]
    unsigned int    TotalSector32;      // #sectors if over 32 MB [if size > 32MB]
    unsigned char    BootDriver;        // holds drive that the boot sector came from
    unsigned char    Flags;             // reserved, empty
    unsigned char    ExtendedBootSign;  // 0x29         ; extended boot sector signature
    unsigned int    VolumeID;           // 0x12060a12   ; disk serial
    unsigned char    VolumeName[11];    // just placeholder. We don't yet use volume labels.
    unsigned char    TypeFat[8];        // file system type
} __attribute__((packed));

struct rootDirTable {
    unsigned char filename[8]; // Bytes 0-7 : DOS File name (Padded with spaces)
    unsigned char ext[3]; // Bytes 8-10 : DOS File extension (Padded with spaces)
    // Bytes 11 : File attributes. This is a bit pattern:
    unsigned char attributes; // Bit 0 : Read Only // Bit 1 : Hidden // Bit 2 : System // Bit 3 : Volume Label // Bit 4 : This is a subdirectory // Bit 5 : Archive // Bit 6 : Device (Internal use) // Bit 7 : Unused
    unsigned char unused; // Bytes 12 : Unused
    unsigned char c_times; // Bytes 13 : Create time in ms
    // Bytes 14-15 : Created time, using the following format:
    unsigned short time; // Bit 0-4 : Seconds (0-29) // Bit 5-10 : Minutes (0-59) // Bit 11-15 : Hours (0-23)
    // Bytes 16-17 : Created year in the following format:
    unsigned short c_date; // Bit 0-4 : Year (0=1980; 127=2107) // Bit 5-8 : Month (1=January; 12=December) // Bit 9-15 : Hours (0-23)
    // Bytes 18-19 : Last access date (Uses same format as above)
    unsigned short a_date; // Bit 0-4 : Year (0=1980; 127=2107) // Bit 5-8 : Month (1=January; 12=December) // Bit 9-15 : Hours (0-23)
    unsigned short index; // Bytes 20-21 : EA Index (Used in OS/2 and NT, dont worry about it)
    // Bytes 22-23 : Last Modified time (See byte 14-15 for format)
    unsigned short m_time; // Bit 0-4 : Seconds (0-29) // Bit 5-10 : Minutes (0-59) // Bit 11-15 : Hours (0-23)
    // Bytes 24-25 : Last modified date (See bytes 16-17 for format)
    unsigned short m_date; // Bit 0-4 : Year (0=1980; 127=2107) // Bit 5-8 : Month (1=January; 12=December) // Bit 9-15 : Hours (0-23)
    unsigned short cluster; // Bytes 26-27 : First Cluster
    unsigned int size; // Bytes 28-31 : File Size
} __attribute__ ((packed));


struct rootTable {
    struct rootDirTable* start;
    int entry;
    int size;
};

struct FAT {
    unsigned int start;
    int size;
};

struct FAT fatseg;
struct rootTable root;
struct buffer_data queue[256];
struct rootDirTable* fileTable = 0;
unsigned char* fileNameTable = 0;
struct bpb blockParamBoot = {{'N','A','D','I','A',' ','O','S'},
                            512,1,1,2,224,2880,0xf0,9,18,2,0,0,
                            0,0,0x29,0x12060a12,
                            {'F','r','e','d','d','y','l','e','y','a','n'},
                            {'F','A','T','1','6',' ',' ',' '}};
