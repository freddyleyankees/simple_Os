#define CON
#include "../include/string.h"
#include "../include/con.h"
#include "../include/stdarg.h"

extern int v_printf(const char* s,va_list arg, char* buf);

int kprintf(const char* fmt, ...){
	va_list arg;
	int i;
	start_arg(arg, fmt);
	i = v_printf(fmt, arg, print_buffer);
	if(!i)
		return 0;
	end_arg(arg);
	print_con(print_buffer);
	return i;
}