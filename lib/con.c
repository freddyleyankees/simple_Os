#include "../include/io.h"
#include "../include/con.h"
#include "../include/string.h"
#define COLUMN  80
#define LINE  25
#define ATTR  0x07
#define start_Screen  0xB8000

unsigned char x = 0;
unsigned char y = 0;
unsigned char color = (unsigned char) ATTR;

void cursorDisable(void){
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void cursorEnable(unsigned char s, unsigned char e){

    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | s);
 
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3E0) & 0xE0) | e);
}

void moveCursor(unsigned char _x, unsigned char _y){
    unsigned short pos_cur;
    pos_cur = (_y * COLUMN + _x);
    outb(0x3d4, 0x0f);
    outb(0x3d5, (unsigned char) pos_cur);
    outb(0x3d4, 0x0e);
    outb(0x3d5, (unsigned char) (pos_cur >> 8));
}


void putCh(unsigned char c, unsigned char cl){
    unsigned char* v = (unsigned char*)((COLUMN*y+x)*2 + start_Screen);
    *v = c;
    *(v+1) = cl;
    return;
}

void static scroll(){
    if(y>LINE-1){
        unsigned char* tmp, *addr;
        tmp = addr = (unsigned char*) start_Screen ;
        
        for (int i= 0;i<4000-160;i++){
            tmp[i] = addr[i+160];
        }
        for (int i= 4000-160;i<4000;i+=2){
            tmp[i] = 0xFF;
            tmp[i+1] = color;
        }
        y=LINE-1;
        x = 0;
    }
}

void print_con(unsigned char* str){
    unsigned char c;
    while ( c = *str++)
    {
        scroll();
        if (c == 0xa)
        {   
            x = 0;
            y++;
            c = 0xff;
        }
        if (c == 0x9)
        {   
            if(x)
                x--;
            c = 0xFF;
        }
        if (c == 0xd)
        {    
            y++;
            c = 0;
        }
        if (x >= 80)
        {    
            y++;
            x = 0;
        }
        if(c){
            putCh(c, color);
            if(c!=0xff)
                x++;
        }
        moveCursor(x,y);
    }
}

void gotoxy(unsigned char _x, unsigned char _y){
    x = _x;
    y = _y;
}

void gotoX(unsigned char _x){
    x = _x;
}

void gotoY(unsigned char _y){
    y = _y;
}

void setColorText(unsigned char _c){
    if (_c>0xf)
        return;
    color = (color&0xF0)|_c;
}

void setBg(unsigned char _c){
    if (_c>0xf)
        return;
    color = (color&0x0F)|(_c<<4);
}
void clearLine(void){
    unsigned char *addr = (unsigned char*)((COLUMN*y)*2 + start_Screen);
    
    asm volatile (" \n \
                    mov $0x0, %%cl \n \
                    mov %0, %%edi \n \
                    clear: \n \
                    movb $0x00, %%al \n \
                    movb %%al, (%%edi) \n \
                    incb %%cl \n \
                    addl $2, %%edi \n \
                    cmp $0x50, %%cl \n \
                    jnz clear"::"m"(addr));
    x = 0;
}

void selectTextLine(unsigned char bg, unsigned char start, unsigned char length){
    if (bg>0xf)
        return;
    unsigned char a = (color&0x0F)|(bg<<4);
    unsigned char *addr = (unsigned char*)((COLUMN*y+start)*2 + start_Screen);
    
    asm volatile (" \n \
                    mov $0x0, %%cl \n \
                    mov %0, %%edi \n \
                    repeat: \n \
                    movb %2, %%al \n \
                    movb %%al, 1(%%edi) \n \
                    incb %%cl \n \
                    addl $2, %%edi \n \
                    cmp %1, %%cl \n \
                    jnz repeat"::"m"(addr),"m"(length),
                     "m"(a));
}

char strsch(unsigned char* src, unsigned char m){
    unsigned int i = 0;
    while (*src){
        if (m == *src++){
            return i;
        }
        i++;
    }
    return -1;
}

unsigned int replacech(unsigned char* src, unsigned char m, int n){
    if(src[n]){
        src[n] = m;
        return n;
    }
    return 0;
}

void selectLine(unsigned char bg){
    if (bg>0xf)
        return;
    selectTextLine(bg,0,0x50);
}

void nextLine(void){
    if (y>=25)
        return;
    gotoxy(0,++y);
}

void clearCon(void){
    unsigned char a = color;
    asm volatile (" \n \
                    movl $0x7D0, %%ecx \n \
                    cld \n \
                    movl %0, %%edi \n \
                    mov $0xFF, %%al \n \
                    mov %1, %%ah \n \
                    rep stosw"::"g"(start_Screen),
                     "m"(a));
}
