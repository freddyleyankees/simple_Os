#ifndef NOS_STRING_H
#define NOS_STRING_H 
#include "../include/size_t.h"

void empty_str(unsigned char *s);

void memset(unsigned char* p,int value,size_t size);

void *memmove(void *d, const void *s, size_t size);

unsigned int memcmp(const void *d, const void *s, size_t size);

void *memcpy(void *d, const void *s, size_t size);

unsigned int strlen(const char* );

unsigned int strcmp(const char* , const char* );

unsigned int strcopy(char* , const char* );

unsigned char* itoa(int );

unsigned char* itoax(unsigned int ,unsigned int);

unsigned char* itoaHex(unsigned int);

int atoi(const char* );

int atoix(const char* , int );

#endif
