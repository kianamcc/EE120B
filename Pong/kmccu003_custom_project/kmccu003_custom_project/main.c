/*
 * kmccu003_custom_project.c
 *
 * Created: 8/22/2018 12:40:56 PM
 * Author : ucrcse
 */ 

#include <avr/io.h>

#include "timer.h"
#include "io.h"
#include "io.c"
#include "matrix.h"
#include "bit.h"

unsigned char button = 0x00;
unsigned char count = 0;
enum Menu {init, play} menu;

void Menu() {
	
	button = ~PINC & 0x01; //C0
	
	switch(menu) { //transitions
		
		case init:
			if(button == 0x00) {
				menu = init;
			}
		   
		   else if(button == 0x01) {
			   menu = play;
		   }
		   break;
		  
		case play:
			menu = play;
			break;
		   
		default:
			menu = init;
			break;
		   
	}
	   
	switch(menu) { //actions
		
		case init:
		//if(count >= 10) {
			LCD_DisplayString(1, "Score P1");
			//count = 0;
			//}
			//++count;
			break;  
			
		case play:
			LCD_ClearScreen();
			LCD_DisplayString(1, "PLAY");
			break;
		   
		  default:
			  break;
		   
	}
	
}


int main(void)
{
    /* Replace with your application code */
	DDRB = 0x00; PORTB = 0xFF; //
	DDRC = 0x00; PORTC = 0xFF; //
	
	DDRA = 0xFF; PORTA = 0x00; //lcd
	DDRD = 0xFF; PORTD = 0x00; //lcd
	
	//LCD_init();
	//menu = init;
	//
	//matrix_init();
	//matrix_write(1, 7);
	//matrix_write(2, 1);
	
	TimerSet(100);
	TimerOn();
	
   // Initializes the LCD display
   LCD_init();
   
   
   while(1) {
	   /*button = ~PINC & 0x01;
	   if (button)
	   {
		   LCD_DisplayString(1, "Hello");
	   }*/
	   Menu();
	   
	   
	   
		while (!TimerFlag);
		TimerFlag = 0;
   }
}

