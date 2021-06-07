
#include "../include/math.h"
#include "../include/string.h"

static unsigned char buffer[1024]={0,};

void empty_str(unsigned char *s){
	while(*s)
		*s++ = 0;
}


void memset(unsigned char* p,int value,size_t size){
    while(size--!=0)
    	*p++ = value;
}

void *memmove(void *d, const void *s, size_t size)
{
    unsigned char *dest = (unsigned char *)d;
    unsigned char *src = (unsigned char *)s;

    if(dest < src)
    	while(size--)
        	*dest++ = *src++;
    
    return d;
}

unsigned int memcmp(const void *d, const void *s, size_t size)
{
	unsigned char *dest = (unsigned char *) d;
    unsigned char *src = (unsigned char *) s;
    while(size--)
		if(*dest++ != *src++)
			return 0;
	
    return 1;    
}

void *memcpy(void *d, const void *s, size_t size)
{
    unsigned char *r = (unsigned char *) d;
    unsigned char *src = (unsigned char *) s;
    
    while(size--)
    	*r++ = *src++;
    return d;
}


unsigned int strlen(const char* s){
	int i=0;
	while(*s++)
		++i;
	return i;
}

unsigned int strcmp(const char* d, const char* s){
	while(*s)
		if (*d++ != *s++)
			return 0;
	return 1;
}

unsigned int strcopy(char* d, const char* s){
	int i = 0;
	while(*s){
		*d++ = *s++;
		++i;
	}
	return i;
}

unsigned char* itoa(int n){
	empty_str(buffer);
	char s[1024] = {0,};
	char neg;
	int i=0, j=0;

	if (n<0)
	{
		/* code */
		neg = '-';
		abs(n);
	}

	if (n == 0)
	{
		/* code */
		s[i++] = '0';
		goto result;
	}

	while (n != 0)
	{
		/* code */
		s[i++] = (n % 10) + '0';
		n/=10;
	}
	
	if (neg == '-')
		s[i++] = neg;
	result:
	while(i>=0)
		buffer[j++] = s[--i];
	neg = 0;
	return buffer;
}

unsigned char* itoax(unsigned int n,unsigned int base){
	empty_str(buffer);
	char s[1024] = {0,};
	int i=0, j=0, r=0;
	if (base == 2)
	{
		/* code */
		s[i++] = 'b';
	}
	if (n == 0)
	{
		/* code */
		s[i++] = '0';
		goto result;
	}
	while (n != 0)
	{
		/* code */
		r = n % base;
		switch(base){
			case 16:
				s[i++] = r >= 10 ? (r % 10) + 'a' : r + '0';
				break;
			default:
				s[i++] =  (n % base) + '0';
				break;
		}
		
		n/=base;
	}
	result:
	switch(base){
		case 8:
			s[i++] = '0';
			break;
		case 16:
			s[i++] = 'x';
			s[i++] = '0';
			break;
	}
	while(i>=0)
		buffer[j++] = s[--i];
	return buffer;
}

unsigned char* itoaHex(unsigned int n){
	empty_str(buffer);
	char s[1024] = {0,};
	int i=0, j=0, r=0;
	if (n == 0)
	{
		/* code */
		s[i++] = '0';
		goto result;
	}
	while (n != 0)
	{
		/* code */
		r = n % 16;
		s[i++] = r >= 10 ? (r % 10) + 'A' : r + '0';
		n/=16;
	}
	result:
	s[i++] = 'x';
	s[i++] = '0';

	while(i>=0)
		buffer[j++] = s[--i];
	return buffer;
}

int atoi(const char* s){
	char c; int result = 0;
	if(!s)
		return -1;
	while(c = *s++){
		if(c<0x30 && c>0x39)
			result += (c - '0')*pow(strlen(s));
		
	}
}

int atoix(const char* s, int base){
	
}