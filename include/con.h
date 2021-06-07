#ifdef CON
	static char print_buffer[1024] = {0,};
#endif
void clearCon(void);
void print_con (unsigned char*);
void cursorEnable(unsigned char s, unsigned char e);
void setColorText(unsigned char _c);
