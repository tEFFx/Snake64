#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <6502.h>

unsigned char stackTemp[STACK_SIZE];
unsigned char speed = 3;
unsigned char colorSpeed = 4;
unsigned char delayCounter = 0;
unsigned char xPos[256];
unsigned char yPos[256];
unsigned char length = 20;
unsigned char stackPos = 0;
unsigned char dir = 254;
unsigned char* screenOffset;
unsigned char color = 2;

struct regs jmp;

extern void music();

/*too slow, need asm pls*/
void clearScreen(void){
	unsigned char i = 0;
	for(; i < 255; i = i + 1){
		POKE(SCREEN_RAM + i, 0x20);
		POKE(SCREEN_RAM + 0x0100 + i, 0x20);
		POKE(SCREEN_RAM + 0x0200 + i, 0x20);
		POKE(SCREEN_RAM + 0x02E8 + i, 0x20);
		
		POKE(COLOR_RAM + i, 0x00);
		POKE(COLOR_RAM + 0x0100 + i, 0x00);
		POKE(COLOR_RAM + 0x0200 + i, 0x00);
		POKE(COLOR_RAM + 0x02E8 + i, 0x00);
	}
}

void respawn(){
	xPos[stackPos] = rand() % 40;
	yPos[stackPos] = rand() % 25;
}

unsigned char gameLoop(void){
	__asm__("dec $D019");
	_sys(&jmp);
	
	if(PEEK(JOYSTICK) < 127){
		dir = PEEK(JOYSTICK);
	}

	if((delayCounter % speed) == 0){
		unsigned char pos = stackPos - length;
		screenOffset = yPos[pos] * 40 + xPos[pos];
		POKE(SCREEN_RAM + screenOffset, 0x20);
		
		pos = stackPos + 1;
		xPos[pos] = xPos[stackPos];
		yPos[pos] = yPos[stackPos];
		
		++stackPos;
		
		if((dir & 1) == 0){
			--yPos[stackPos];
		}else if((dir & 2) == 0){
			++yPos[stackPos];
		}else if((dir & 4) == 0){
			--xPos[stackPos];
		}else if((dir & 8) == 0){
			++xPos[stackPos];
		}
		
		if(yPos[stackPos] == 25){
			yPos[stackPos] = 0;
		}else if(yPos[stackPos] == 255){
			yPos[stackPos] = 24;
		}
		
		if(xPos[stackPos] == 40){
			xPos[stackPos] = 0;
		}else if(xPos[stackPos] == 255){
			xPos[stackPos] = 39;
		}
		
		screenOffset = yPos[stackPos] * 40 + xPos[stackPos];
		if(PEEK(SCREEN_RAM + screenOffset) == 0xE0){
			clearScreen();
			respawn();
		}else{
			POKE(SCREEN_RAM + screenOffset, 0xE0);
			POKE(COLOR_RAM + screenOffset, 0x05);
		}
	}
	
	/*if((delayCounter % colorSpeed) == 0){
		color++;
		if(color > 15){
			color = 1;
		}
		
		POKE(BG_COLOR, color);
	}*/
			
	++delayCounter;
	
	return (IRQ_HANDLED);
}

unsigned int addr;
char str[20];
int main (void){	
	unsigned char i = 0;
	for(; i < 255; i++){}	/*needed for music for some reason*/
	
	jmp.pc = 0x1103;
	_sys(&jmp);
	jmp.pc = (void*)music;
	
	srand(PEEK(0xD012));
	clearScreen();
	respawn();
	
	SEI();
	POKE(0xDC0D, 0x7F);
	POKE(0xDD0D, 0x7F);
	POKE(0xD01A, 0x01);
	set_irq(&gameLoop, stackTemp, STACK_SIZE);
	POKE(0xD012, 0);
	POKE(0xD011, PEEK(0xD011) & 0x7F);
	CLI();
	
	while(1);
	
	return EXIT_SUCCESS;
}
