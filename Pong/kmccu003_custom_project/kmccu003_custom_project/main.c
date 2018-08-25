/*
 * kmccu003_custom_project.c
 *
 * Created: 8/22/2018 12:40:56 PM
 */ 

#include <avr/io.h>

#include "timer.h"
#include "io.h"
#include "io.c"
#include "matrix.h"
#include "bit.h"
#include "math.h"

unsigned char button = 0x00;
//unsigned char count = 0;
enum Menu {init, play, matrix} menu;

void Menu() {
	
	button = ~PINC & 0x01; //C0
	
	switch(menu) { //transitions
		
		case init:
			if(button == 0x00) {
				menu = init;
			}
		   
		   else if(button == 0x01) {
			   menu = matrix;
		   }
		   break;
		  
		case play:
			menu = play;
			break;
			
		case matrix:
			menu = play;
		   break;
		   
		default:
			menu = init;
			break;
		   
	}
	   
	switch(menu) { //actions
		
		case init:
			LCD_DisplayString(1, "PRESS TO PLAY");
			break;  
		
		case matrix:
		 matrix_write(1, 28); //p1 paddle in middle
		 break;
			
		case play:
			LCD_ClearScreen();
			LCD_DisplayString(1, "     SCORE:     P1: 0      P2: 0");
			break;
		   
		  default:
			  break;
		   
	}
	
}

unsigned char button_right = 0x00;
unsigned char button_left = 0x00;
unsigned char reset_button = 0x00;
unsigned char count_right = 28;

enum player_1{wait_p1, press_p1, right_press, left_press, release_p1, reset} p1_state;

void P1() {
	
	//count_right = 28;
	reset_button = ~PINC & 0x01; //C0
	button_right = ~PINC & 0x02; //C1
	button_left = ~PINC & 0x04; //C2
	
	switch(p1_state) { //transitions
		
		case wait_p1:
			if(!button_right && !button_left) { //stay until a button is pressed
				p1_state = wait_p1;
			}
			
			else if(button_right && !button_left) {
				p1_state = right_press;
			}
			
			else if(!button_right && button_left) {
				p1_state = left_press;
			}
			
			else if(reset_button) {
				p1_state = reset;
			}
			
			break;
		
			
		case right_press: //execute command then go straight to release
		
			if(reset_button) {
				p1_state = reset;
			}
			
			else {
				p1_state = release_p1;
				break;
			}
			
		case left_press:
		
			if(reset_button) {
				p1_state = reset;
			}
			
			else {
				p1_state = release_p1;
				break;
			}
		
		case release_p1: //wait for next press

			if(!button_right && !button_left) { //released
				p1_state = press_p1;
			}
			
			else if(reset_button) {
				p1_state = reset;
			}
			
			else {
				p1_state = release_p1;
			}
			
		case press_p1:
		
			if(button_right && !button_left) { //right
				p1_state = right_press;
			}
		
			else if(!button_right && button_left) { //left
				p1_state = left_press;
			}
			else {
				p1_state = press_p1;
			}
			break;
		
		case reset:
			p1_state = wait_p1;
			break;
			
		default:
			break;
			
	}
	
	
	switch(p1_state) { //actions
		
		case wait_p1:
		break;
		
		case right_press:
			matrix_clear();

			if(count_right < 224) {
				count_right = count_right * 2;
				//count_right += count_right;
				matrix_write(1, count_right); //p1 paddle in middle
			}
			
			else if (count_right == 224) {
				matrix_write(1, 224);
			}
			break;
		
		case left_press:
			matrix_clear();

			if(count_right != 7) {
				count_right = count_right / 2;
				//count_right += count_right;
				matrix_write(1, count_right); //p1 paddle in middle
			}
			
			else if (count_right == 7) {
				matrix_write(1, 7);
			}
			break;
		
		case release_p1:
		break;
		
		case reset: //not working
			matrix_clear();
			matrix_write(1, 224);
			//LCD_ClearScreen();
			//LCD_DisplayString(1, "     SCORE:     P1: 1       P2:  0");
			break;	
	}	
} //end player 1

enum ball_movement{ball_start} ball_state;

void Ball() {
	
	
	
	
} //end ball


int main(void)
{
    /* Replace with your application code */
	DDRB = 0xFF; PORTB = 0x00; //matrix
	DDRC = 0x00; PORTC = 0xFF; //input buttons
	
	DDRA = 0xFF; PORTA = 0x00; //lcd
	DDRD = 0xFF; PORTD = 0x00; //lcd
	
	//LCD_init();
	menu = init;
	
	matrix_init();
	//matrix_write(1, 7);
	//matrix_write(2, 1);
	
	TimerSet(100);
	TimerOn();
	
   // Initializes the LCD display
   LCD_init();
   
   
   while(1) {
	   
	   Menu();
	   P1();
	   //P2
	   Ball();
	   
	   
	   
		while (!TimerFlag);
		TimerFlag = 0;
   }
}

