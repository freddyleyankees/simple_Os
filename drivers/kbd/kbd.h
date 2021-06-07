#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "../../include/io.h"

#ifdef KBD
#define KBD_READ_STATUT_PORT			0x64 	/* read status register */
#define KBD_SEND_CMD_PORT				0x64	/* write | send command to controller */
#define KBD_READ_BUFFER_PORT			0x60	/* read input buffer */
#define KBD_WRITE_BUFFER_PORT			0x60	/* write input buffer */

/* keyboard controller commands */ 

#define KBD_READ_CTRL_BYTE				0x20	/* read input buffer */
#define KBD_WRITE_CTRL_BYTE				0x60	/* read input buffer */
#define KBD_SEFT_TEST					0xAA	/* read input buffer */
#define KBD_INTERFACE_TEST				0xAB	/* read input buffer */
#define KBD_DISABLE						0xAD	/* read input buffer */
#define KBD_ENABLE						0xAE	/* read input buffer */
#define KBD_READ_INPUT_PORT				0xC0	/* read input buffer */
#define KBD_READ_OUTPUT_PORT			0xD0	/* read input buffer */
#define KBD_WRITE_OUTPUT_PORT			0xD1	/* read input buffer */
#define KBD_ENABLE_A20					0xDD	/* read input buffer */
#define KBD_DISABLE_A20					0xDF	/* read input buffer */
#define KBD_READ_TEST_INPUT				0xE0	/* read input buffer */
#define KBD_RESET_SYSTEM				0xFE	/* read input buffer */

#define KEY_NOACTIVE					0x00
#define LSHIFT_KEY_ACTIVE				0x01		
#define RSHIFT_KEY_ACTIVE				0x02		
#define LCTRL_KEY_ACTIVE				0x03		
#define RCTRL_KEY_ACTIVE				0x04		
#define ALT_KEY_ACTIVE					0x05		
#define ALTGR_KEY_ACTIVE				0x06		
#define CAPLOCK_KEY_ACTIVE				0x07		
#define NUMLOCK_KEY_ACTIVE				0x08		

#define TYPE_KEYBOARD_EN				0x1		
#define TYPE_KEYBOARD_FR				0x2

#define KEYBOARD_NO_DEFINE				0x0
#define KEYBOARD_DEFINE					0x1
#define KEYBOARD_CLASSCODE				0x00090000

#define waitkbdbread()					({										 	\
											unsigned char _a, _b; 						\
											asm("1:"::); 						\
																	\
										})



#define wait_input_buf_empty()		({										 	\
											unsigned char _a, _b; 						\
											asm("1:"::); 						\
											do{ 									\
												_a = inb(KBD_READ_STATUT_PORT); \
												_b = _a & 2;						\
											}while(_b!=0); 							\
										})

#define wait_output_buf_empty()		({ 											\
											unsigned char _a, _b; 						\
											asm("1:"::); 						\
											do{ 									\
												_a = inb(KBD_READ_STATUT_PORT); \
												_b = _a & 1; 						\
											}while(_b==0);	 						\
										})

static void set_led_kbd(unsigned char, unsigned char, unsigned char);
static void set_self_test_kbd(void);
static void set_interface_test_kbd(void);
static void disable_kbd(void);
static void cmd_read_port_kbd(void);
static void cmd_write_port_kbd(void);
static unsigned char read_input_port_kbd(void);
static void write_output_port_kbd(unsigned char);
static void enable_kbd(void);
static void system_reset(void);
static void config_device_keyboard(void);


void toggleCapLockKey(void);
void toggleNumLockKey(void);
void setLanguageKeyboard(unsigned char);



struct ctrl_key_struct
{
	unsigned char lshift;
	unsigned char rshift;
	unsigned char lctrlkey;
	unsigned char rctrlkey;
	unsigned char altkey;
	unsigned char altGrkey;
	unsigned char caplock;
	unsigned char numlock;
};

struct ctrl_key_struct key_controller;

static unsigned char fn_key_controller(struct ctrl_key_struct*, unsigned char);
static unsigned char load_char(unsigned char*, unsigned char);
static void set_fn_key(struct ctrl_key_struct*, unsigned char, unsigned char);
static unsigned char load_kbd(unsigned char);
static unsigned char* get_kbd();
static void set_kbd(unsigned char);

#endif

void init_kbd(void);
unsigned char eventKeypress();
#endif
