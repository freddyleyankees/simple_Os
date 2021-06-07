;--------------------- booter code -------------------------
;------------------------ bs.asm ---------------------------
[BITS 16]
[ORG 0x0]
%define ROOTSEG     0x8B20
%define FILESEG     0x1000
%define SETUPSEG    0x9020
%define INITSEG     0x9000
%define FATSEG      0x8B40

start:
    jmp loader
;structure of BiosParameterBlock : 16
dataBootsect:
        OEM:                 db 'NADIA OS'   ; OEM string
        SectorSize:          dw 512          ; Bytes per sector
        SectorsPerCluster:   db 1            ; Sectors per cluster
        SectorsRsvd:         dw 1            ; #of reserved sectors
        NFat:                db 2            ; #of fat copies
        EntriesRootDir:      dw 224          ; size of root directory
        TotalSector:         dw 2880         ; total # of sectors if below 32 MB
        MediaType:           db 0xF0         ; Media Descriptor [floppy or hdd]
        SectorsPerFat:       dw 9            ; Size of each FAT
        SectorsPerTrack:     dw 18           ; Sectors per track
        NHead:               dw 2            ; number of read-write heads
        SectorsHidden:       dd 0            ; number of hidden sectors [sectors incompatible]
        TotalSector32:       dd 0            ; #sectors if over 32 MB [if size > 32MB]
        BootDriver:          db 0            ; holds drive that the boot sector came from
        Flags:               db 0            ; reserved, empty
        ExtendedBootSign:    db 0x29         ; extended boot sector signature
        VolumeID:            dd 0x12060a12   ; disk serial
        VolumeName:          db 'Freddyleyan'; just placeholder. We don`t yet use volume labels.
        TypeFat:             db 'FAT12   '   ; file system type

;----------------------------------------------------
;   disk utility
;----------------------------------------------------

;----------------------------------------------------
;   Functions reset disk 
;----------------------------------------------------
resetDiskSystem:
    mov dl, [BootDriver]                                       ; Get Boot driver
    xor ax, ax                                                 ; reset disk = ax = 0
    int 0x13                                                   ; Call bios disk I/O routine
    jc bootFailure
ret

;----------------------------------------------------
;   Functions Read sector disk 
;----------------------------------------------------

;# Allow to read one sector in disk
readSectorDiskSystem:
    xor cx, cx
    .read:
        push ax     ;# store start sector in LBA
        push cx     ;# store number count for error
        push bx     ;# Store data buffer offset
        ;# Convert LBA to CHS
        ;# Calculate cylinder, head and sector:
        ;# Cylinder = (LBA / SectorsPerTrack) / NumHeads
        ;# Sector   = (LBA mod SectorsPerTrack) + 1
        ;# Head     = (LBA / SectorsPerTrack) mod NumHeads

        mov bx, word [SectorsPerTrack]
        xor dx, dx
        div bx      ;# AX = LBA / SectorsPerTrack , DX = sector
        ;# get sector number
        inc dx
        mov cl, dl      ;# CL <= DL start sector
        
        mov bx, word [NHead]
        xor dx, dx
        div bx      ;# AX = cylinder , DX = head
        ;# get cylinder number
        mov ch, al       ;# CH <= AL cylinder number
        ;# get head number
        xchg dl, dh      ;# DL <= DH && DH <= DL [DH] head number and [DL] boot driver
        ;# get sectors total to read
        ;# We read a sector
        mov ax, 0x0201
        mov dl, byte [BootDriver]
        pop bx  ;# restore data buffer offset
        int 0x13
        jc .readFailure
        pop cx
        pop ax
        jmp .done
    ;# The read has failed
    ;# We will retry five times total, then jump to boot failure.
    .readFailure:  
        pop cx
        inc cx
        cmp cx, 0x5
        je bootFailure
        ;# Reset the disk system
        xor ax, ax
        int 0x13
        ;# Get logical block from stack and retry
        pop ax
        jmp .read

    .done:
    
ret

print:
    lodsb
    cmp al, 0
    jz .done
    mov ah, 0x0E
    mov bx, 0x07
    int 0x10
    jmp print
    .done:
        ret

reboot:
    mov si, rebootMsg
    call print
    xor ax, ax
    int 0x16
    ;# reboot force only in 16bit
    int 0x19    
ret


;-------------------------------------------------------
;   file system
;-------------------------------------------------------

;-------------------------------------------------------
;   Function find File in root directory
;   - Prepare segment to load ROOT DIRECTORY TABLE
;   - Copy ROOT DIRECTORY TABLE in memory []
;   - Find filename in ROOT DIRECTORY TABLE
;   - Get offset sector this file begin and size in disk
;--------------------------------------------------------

;--------------------------------------------------------
;   ROOT DIRECTORY TABLE
;       Offset      Size       Description
;       [0x00]        8         File name
;       [0x08]        3         File Extension
;       [0x0B]        1         Attribute
;         |_________  Bit 0:        Read only
;         |_________  Bit 1:        Hidden
;         |_________  Bit 2:        System
;         |_________  Bit 3:        Volume Label
;         |_________  Bit 4:        This is Subdirectory
;         |_________  Bit 5:        Archive
;         |_________  Bit 6:        Device
;         |_________  Bit 7:        Unused
;       [0x0C]        1         Reserved [Unused]
;       [0x0D]        1         Creation timestamp
;       [0x0E]        2         Creation time
;         |_________  Bit 0-4:      Seconds/2 (0-29)
;         |_________  Bit 5-10:     Minutes (0-59)
;         |_________  Bit 11-15:    Hours   (0-23)
;       [0x10]        2         Creation date
;         |_________  Bit 0-4:      Day (1-31)
;         |_________  Bit 5-8:      Month (1=January - 12=December)
;         |_________  Bit 9-15:     Year (1=1980 - 127=2107)
;       [0x12]        2         Last access date
;         |_________  Bit 0-4:      Day (1-31)
;         |_________  Bit 5-8:      Month (1=January - 12=December)
;         |_________  Bit 9-15:     Year (1=1980 - 127=2107)
;       [0x14]        2         Reserved [EA Index (Used in OS/2 and NT, dont worry about it)]
;       [0x16]        2         Last modified time
;         |_________  Bit 0-4:      Seconds/2 (0-29)
;         |_________  Bit 5-10:     Minutes (0-59)
;         |_________  Bit 11-15:    Hours   (0-23)    
;       [0x18]        2         Last modified date
;         |_________  Bit 0-4:      Seconds/2 (0-29)
;         |_________  Bit 5-10:     Minutes (0-59)
;         |_________  Bit 11-15:    Hours   (0-23)    
;       [0x1A]        2         First Cluster
;       [0x1C]        4         File size (bytes)
;--------------------------------------------------------

findFile: ;# Take 2 parameters ROOTSEGMENT, filename
    ;# Set ES of segment where root directory should be loaded
    push bp
    mov bp, sp

    mov ax, word [bp+4]  ;# first parameter
    mov es, ax
    ;# Get size of root directory from disk
    ;# the formule is (32byte * EntriesRootDir)/SectorSize + (32byte * EntriesRootDir)%SectorSize
    ;# Calculate this total and store it in rootDirectoryTableSize
    mov ax, 32
    xor dx, dx
    xor cx, cx
    mul word [EntriesRootDir]
    div word [SectorSize]
    xchg ax, cx
    mov word [rootDirectoryTableSize], cx

    ;# Calculate start sector root directory
    ;# the formular is (SectorsPerFat * NFat) + SectorsHidden + SectorsRsvd
    ;# Calculate this total and store it in rootDirectoryTable
    
    mov al, byte [NFat]
    mul word [SectorsPerFat]
    add ax, word [SectorsRsvd]
    mov word [rootDirectoryTable], ax
   
    ;# Load a cluster [sector] from root directory 
    .read_next_cluster:
        push cx ;# number of sectors
        push ax ;# start sector root directory
        xor bx, bx
        ;# read a sector
        
        call readSectorDiskSystem
       
    .check_entry:
        
        mov cx, 11 ;# filename Extension [11 bytes]
        
        mov di, bx
        mov si, word [bp+6]  ;# second parameter
        repz cmpsb  ;# ZF=1 if equal 
        je .found_file  ;# jump if ZF=1
        add bx, 32
        cmp bx, word [SectorSize]
        jne .check_entry    ;# jump if ZF=0

    ;# Load next cluster [sector]
        pop ax
        inc ax
        pop cx
        loopnz .read_next_cluster
        mov si, no_found_file
        call print
        mov si, word [bp+6]
        call print
        call reboot
    .found_file:
        xor ax, ax
        mov word [rootEntryfile], bx
        mov ax, word [es:bx+0x1A]   ;# Get entry file in FAT
        mov word [fileStart], ax   ;# store entry file into fileStart
    mov sp, bp
    pop bp
ret 

;-------------------------------------------------------
;   Function read FAT
;   - Prepare FAT segment into memory 
;   - Get start offset FAT from disk
;   - Formular is (SectorsRsvd)
;   - Copy each sector FAT from disk in memory [0000:0EE0]
;--------------------------------------------------------

readFAT: ;# Take 1 parameter FATSEGMENT
    ;# Prepare memory to receive FAT
    push bp
    mov bp, sp
    mov ax, word [bp+4]  ;# first parameter FATSEGMENT
    mov es, ax

    xor bx, bx
    ;# Calculate start sector of the FAT
    mov ax, word [SectorsRsvd]
    ;# Calculate size of all FAT sectors
    mov cx, word [SectorsPerFat]
    ;# Copy all FAT sectors into memory
    .read_next_cluster_fat:
        push cx ;# number of sectors
        push ax ;# start sector root directory
        call readSectorDiskSystem
        pop ax
        pop cx
        inc ax
        add bx, word [SectorSize]
        loopnz .read_next_cluster_fat
    mov sp, bp
    pop bp
ret

;-------------------------------------------------------
;   Function read File from start cluster
;   - Prepare FILE segment into memory 
;   - Get entry file in FAT 
;   - get size of file
;--------------------------------------------------------

readFile: ;# Take 2 parameters FILESEGMENT, FATSEGMENT
    ;# Prepare segment memory to receive File 
    push bp
    mov bp, sp
    mov ax, word [bp+4]  ;# first parameter FILESEGMENT
    mov es, ax

    xor bx, bx
    ;# Get first entry in FAT
    mov cx, word [fileStart]   ;# Cluster number on the disk
    ;# Read each sector data file in memory
    .read_file_next_cluster:

    ;# calculate sector start of the file into disk
    ;# addr = fileStart(cluster)*SectorsPerCluster+rootDirectoryTable+root_sect ; SectorsPerCluster = 1
    ; -------------------------------------------------------------------------------------------------
    ; | bootsect | reserved | FAT1 | FAT2 | ROOT DIR TABLE | cluster0 | cluster1 | cluster2 | cluster2 |
    ; -------------------------------------------------------------------------------------------------
    ; |          | optionnal|   FAT size  |    root_sect   |                  DATA sectors           
    ; -------------------------------------------------------------------------------------------------
    ; |                 FAT start     rootDirectoryTable   fileStart
    ; -------------------------------------------------------------------------------------------------
        
        mov ax, cx      ;# get next cluster
        sub ax, 2       ;# cluster value start 0x002 because 0x001 and 0x000 are reserved
        add ax, word [rootDirectoryTable]
        add ax, word [rootDirectoryTableSize] ;# store start file in AX

        ;# Read sector file into disk
        push cx                 ;# save entry cluster
        call readSectorDiskSystem   ;# read sector file and store in es:bx
        pop cx                  ;# restore entry cluster

        add bx, word [SectorSize] 
    ;--------------------------------------------------------------------------------------------------
    ;   FILE ALLOCATION TABLE   [12 bits]
    ;       Cluster      FAT-VALUE       
    ;       [0x000]  =>    0xFF0
    ;       [0x001]  =>    0xFFF
    ;       [0x002]  =>    0x000
    ;       [0x003]  =>    0x004
    ;       [0x004]  =>    0xFFF
    ;       [0x005]  =>    0x000
    ;       [0x006]  =>    0x000
    ;       [0x007]  =>    .....
    ;       [0x008]  =>    .....
    ;       [0x009]  =>    .....
    ;       [0x00A]  =>    .....
    ;       [0x000]  =>    .....
    ;       [0x000]  =>    .....
    ;       Directory entry for file 1 points to cluster index 0 in FAT
    ;       3 => 0x004 [next entry in FAT]
    ;       4 => 0xFFF [End]
    ;       Each entry FAT occupy 12 bits in memory [1.5 of 16 bits]
    ;       if 12 bits => 4096 cluster 
    ;           then in memory we can store 4096/1.5
    ;   FAT [MEMORY : 0x8B400]
    ;   [byte]       0        1        2        3        4        5        6        7        8        9       10        11
    ;               -.--------.--------.--------.--------.--------.--------.--------.--------.--------.--------.--------.-
    ;   [Cluster]   |||     0     ||     1     |||     2     ||     3     |||     4     ||     5     |||     6     ||---||
    ;   [FAT-VALUE] ||00001111|11111111|11111111|00000000|00000001|00000000|11111111|11110000|00000000|00000000|00000000||
    ;   [ES:BX]     ||0x8B40:0|0x8B40:1|0x8B40:2|0x8B40:3|0x8B40:4|0x8B40:5|0x8B40:6|0x8B40:7|0x8B40:8|0x8B40:9|0x8B40:a||
    ;               -.--------.--------.--------.--------.--------.--------.--------.--------.--------.--------.--------.-
    ;               ||      word       |       word      |       word      |       word      |       word      |--------||                
    ;               
    ;               Read cluster into FAT: 
    ;               Algorithm: (FATSEGMENT, EntryFile)
    ;               NewEntryCluster = EntryFile
    ;               rep:[size FAT < 64k [it`s not necessary to switch between segment]]
    ;               NewEntrySegment = FATSEGMENT:NewEntryCluster*3/2
    ;               
    ;               if NewEntryCluster % 2 == 0
    ;                   then NewEntryCluster = [NewEntrySegment]&0x0FFF
    ;                   else
    ;                        NewEntryCluster = [NewEntrySegment] >> 4
    ;---------------------------------------------------------------------------------------------------------------------
    push ds
    ;# Get next entry File in FAT
    mov dx, word [bp+6]  ;# second parameter FATSEGMENT
    mov ds, dx          ;# set segment FAT started
    ;# offset = cluster + cluster/2 = cluster*3/2
    mov si, cx          ;# set entry cluster file in FAT
    mov dx, cx          ;# set entry cluster file in FAT
    shr dx, 1         
    add si, dx
    mov dx, word [ds:si]    ;# store next cluster in DX
    ;# Test if entry cluster is even
    test cx, 0x0001
    jnz .read_cluster_odd
    and dx, 0x0FFF
    jmp .done
    .read_cluster_odd:
        shr dx, 4
    .done:
        pop ds
        mov cx, dx      ;# store next cluster in CX
        cmp cx, 0x0FF0
        jb .read_file_next_cluster
    mov sp, bp
    pop bp
ret

loader:

;----------------------------------------------------
; code reallocated at 9000:0000, adjust segment registers
;----------------------------------------------------

    
    mov ax, 0x07C0
    mov ds, ax
    mov ax, INITSEG
    mov es, ax
    mov ss, ax
    mov sp, 0xffff
    mov byte [BootDriver], dl
    
    
    call resetDiskSystem
    ;# Find filename into ROOT DIRECTORY TABLE

    push word filename
    push ROOTSEG
    call findFile

    ;# Copy FAT into memory

    push FATSEG
    call readFAT

    ;# Read file into memory
    push FATSEG
    push SETUPSEG
    call readFile
    
    jmp dword SETUPSEG:0

bootFailure:
    call reboot

    filename     db "INIBTOS BIN", 13, 10, 0
    no_found_file     db "No found file: ", 0
    rebootMsg   db "Error, reboot! ", 13, 10, 0
    rootDirectoryTableSize dw 0
    rootDirectoryTable  dw 0
    fileStart  dw 0
    rootEntryfile  dw 0
    
    times 510-($-$$) db 0
    dw 0xaa55
