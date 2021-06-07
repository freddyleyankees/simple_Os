#include "../include/string.h"
#include "../include/stdarg.h"

int v_printf(const char* s,va_list arg, char* buf){

	char* str, *tmp;
	void* ptr;
	char c;
	int count = 0, i,b;
	memset(buf, 0, strlen(buf));
	goto make;
	do_conv_dec:
		tmp = itoa(get_arg(arg, int));
		goto do_next;
	do_conv_x:
		tmp = itoax((unsigned int) get_arg(arg, int), b);
		goto do_next;
	do_conv_hex:
		tmp = itoaHex((unsigned int) get_arg(arg, int));
		goto do_next;
	do_conv_ptr:
		ptr = get_arg(arg, void*);
		tmp = itoaHex((unsigned int) ptr);
		goto do_next;
	do_conv_str:
		tmp = get_arg(arg, char*);
	do_next:
		if(strlen(tmp)>1024){
			return 0;
		}
		i = strcopy(buf, tmp);
		buf+=i;
		count+=i;
		s++;
	make:
	if(*s){
		switch(*s){
			case '%':
				switch(*++s){
					case 'i':
						goto do_conv_dec;
						break;
					case 'd':
						goto do_conv_dec;
						break;
					case 'u':
						b = 10;
						goto do_conv_x;
						break;
					case 'o':
						b = 8;
						goto do_conv_x;
						break;
					case 'x':
						b = 16;
						goto do_conv_x;
						break;
					case 'X':// 64 bits
						goto do_conv_hex;
						break;
					case 'b':
						b=2;
						goto do_conv_x;
						break;
					case 'p':
						goto do_conv_ptr;
						break;
					case 'n':
						break;
					case 'c':
						c = (unsigned char) get_arg(arg, int);
						*buf++=c;
						s++;
						++count;
						goto make;
						break;
					case 's':
						goto do_conv_str;
						break;
					case '%':
						*buf++='%';
						s++;
						++count;
						goto make;
						break;
					default:
						return 0;
						break;
				}
				break;
			default:
				*buf++ = *s++;
				++count;
				goto make;
				break;
		}
	}

	buf = 0;
	return count;
}