#define POKE(addr,val) 	(*(unsigned char*) (addr) = (val))
#define PEEK(addr)		(*(unsigned char*) (addr))

#define BG_COLOR		0xD021
#define BORDER_COLOR	0xD020

#define SCREEN_RAM		0x0400
#define	COLOR_RAM		0xD800

#define STACK_SIZE     2048
#define JOYSTICK        0xDC00