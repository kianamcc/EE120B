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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char button = 0x00;
unsigned char play_flag = 0;
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
		 play_flag = 1;
		 //matrix_write(4, 8); //ball
		 break;
			
		case play:
			LCD_ClearScreen();
			LCD_DisplayString(1, "     SCORE:     P1: 0      P2: 0");
			break;
		   
		  default:
			  break;
		   
	}
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct p1_struct { //type of struct
	
	unsigned char y; //coordinates
	unsigned char x;
	
} p1;

void p1_init() {
	p1.y = 1;
	p1.x = 28;
}

unsigned char button_right = 0x00;
unsigned char button_left = 0x00;
unsigned char reset_button = 0x00;
unsigned char count_right = 28;
unsigned char p1_index = 3;

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
			
			else if(reset_button) {
				p1_state = reset;
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
		 p1_init();
		break;
		
		case press_p1:
		break;
		
		case right_press:

			if(p1.x < 224) {
				p1.x  = p1.x * 2;
			}
			
			//matrix_write(p1.y, p1.x);
			break;
		
		case left_press:

			if(p1.x != 7) {
				p1.x = p1.x / 2;
			}
			
			//matrix_write(p1.y, p1.x);
			break;
		
		case release_p1:
		break;
		
		case reset: 
			matrix_clear();
			count_right = 28;
			//matrix_write(1, 28); //p1 paddle in middle
			//matrix_write(8, 28); //p2 paddle in middle
			break;	
	}	
} //end player 1
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char button_right_2 = 0x00;
unsigned char button_left_2 = 0x00;
unsigned char reset_button_2 = 0x00;
unsigned char count_right_2 = 28;


enum player_2{wait_p2, press_p2, right_press_p2, left_press_p2, release_p2, reset_p2} p2_state;

void P2() {
	
	reset_button_2 = ~PINC & 0x01; //C0
	button_right_2 = ~PINC & 0x08; //C3
	button_left_2 = ~PINC & 0x10; //C4
	
	switch(p2_state) { //transitions
		
		case wait_p2:
		if(!button_right_2 && !button_left_2) { //stay until a button is pressed
			p2_state = wait_p2;
		}
		
		else if(button_right_2 && !button_left_2) {
			p2_state = right_press_p2;
		}
		
		else if(!button_right_2 && button_left_2) {
			p2_state = left_press_p2;
		}
		
		else if(reset_button_2) {
			p2_state = reset_p2;
		}
		
		break;
		
		case right_press_p2: //execute command then go straight to release
		
			if(reset_button_2) {
				p2_state = reset_p2;
			}
		
			else {
				p2_state = release_p2;
				break;
			}
		
		case left_press_p2:
		
			if(reset_button_2) {
				p2_state = reset_p2;
			}
		
			else {
				p2_state = release_p2;
				break;
			}
		
		case release_p2: //wait for next press

			if(!button_right_2 && !button_left_2) { //released
				p2_state = press_p2;
			}
		
			else if(reset_button_2) {
				p2_state = reset_p2;
			}
		
			else {
				p2_state = release_p2;
			}
		
		case press_p2:
		
			if(button_right_2 && !button_left_2) { //right
				p2_state = right_press_p2;
			}
		
			else if(!button_right_2 && button_left_2) { //left
				p2_state = left_press_p2;
			}
			
			else if(reset_button_2) {
				p2_state = reset_p2;
			}
			
			else {
				p2_state = press_p2;
			}
			break;
		
			case reset_p2:
			p2_state = wait_p2;
			break;
		
		default:
		break;
	}
	
	switch(p2_state) { //actions
		
		case wait_p2:
		break;
		
		case press_p2:
		break;
		
		case right_press_p2:

			if(count_right_2 < 224) {
				count_right_2 = count_right_2 * 2;
				//count_right += count_right;
				matrix_write(8, count_right_2); //p1 paddle in middle
			}
		
			else if (count_right_2 == 224) {
				matrix_write(8, 224);
			}
			break;
		
		case left_press_p2:
		
			if(count_right_2 != 7) {
				count_right_2 = count_right_2 / 2;
				//count_right += count_right;
				matrix_write(8, count_right_2); //p1 paddle in middle
			}
		
			else if (count_right_2 == 7) {
				matrix_write(8, 7);
			}
			
			
			break;
		
		case release_p2:
		break;
		
		case reset_p2:
			matrix_clear();
			count_right_2 = 28;
			matrix_write(1, 28); //p1 paddle in middle
			matrix_write(8, 28); //p2 paddle in middle
			//LCD_ClearScreen();
			//LCD_DisplayString(1, "     SCORE:     P1: 1       P2:  0");
			break;
	}
} //end player 2
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char ball_flag = 0;

struct ball_struct { //type of struct
	
	unsigned int direction; //0 down, 1 up, 2 right-up, 3 left-up, 4 right-down, 5 left-down
	unsigned char y; //coordinates
	unsigned char x;
	
} ball;

void ball_init() {
	
	ball.direction = 0;
	ball.y = 5;
	ball.x = 8;
}

unsigned char p1_ball_check() {
	return ball.x & p1.x;
}
//unsigned char p2_ball_check() {
	//return ball.x & p2.x;
//}
//unsigned char ball_count = 5; //start at 4, 8
unsigned char ball_column_index = 5;
unsigned char ball_row_index = 8;

enum ball_movement{init_ball, ball_start, ball_vertical/*, ball_hit*/} ball_state;

void Ball() {
	
	switch(ball_state) { //transitions
		
		case init_ball:
			if (play_flag == 1)
			{
				ball_state = ball_start;
			}
			else
			{
				ball_state = init_ball;
			}
			break;
			
			
		case ball_start:
			if (ball.y <= 2) { //ball near p1
			ball_state = ball_vertical;
			}
			if (ball.y >= 7)
			{
				ball_state = ball_vertical;
			}
			//ball_state = ball_start;
			break;
			
		case ball_vertical:
			ball_state = ball_start;
			break;
			
	}
	
	switch(ball_state) { //actions
		
		case init_ball:
			ball_init();
			break;
			
		case ball_start: //ball go down towards p1
			matrix_write(ball.y, 0);
			if (ball.direction == 0) { //stops right in front of paddle
				ball.y -= 1;
			}
			if (ball.direction == 1)
			{
				ball.y += 1;
			}
			matrix_write(ball.y, ball.x);
			break;
			
			case ball_vertical:
				if(ball.y <= 2) { //check player coordinate,
					if (p1_ball_check())
					{
						ball.direction = 1;
					}
					else if(ball.y == 1)
					{
						matrix_write(ball.y, 0);
						ball_init();
					}
				}
				if (ball.y >= 7)
				{
					ball.direction = 0;
				}
				break;
		
	}	
	
} //end ball
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
enum led_matrix{init_led, update} led_state;
void display()
{
	switch(led_state) {
		case init_led:
			led_state = update;
			break;
			
		case update:
			led_state = update;
			break;
		
	}
	switch(led_state) {
		case init:
			break;
		
		case update:
				 matrix_write(p1.y, p1.x); //p1 paddle in middle
				 matrix_write(ball.y, ball.x); //p2 paddle in middle
				 break;
	}
}


int main(void)
{
    /* Replace with your application code */
	DDRB = 0xFF; PORTB = 0x00; //matrix
	DDRC = 0x00; PORTC = 0xFF; //input buttons
	
	DDRA = 0xFF; PORTA = 0x00; //lcd
	DDRD = 0xFF; PORTD = 0x00; //lcd
	
	unsigned long ball_timer = 100;
	
	menu = init;
	
	matrix_init();
	
	TimerSet(100);
	TimerOn();
	
   // Initializes the LCD display
   LCD_init();
   
   
   while(1) {
	   
	   Menu();
	   
	   P1();
	   
	   P2();
	   
	   if(ball_timer == 300) {
		   Ball();
		   ball_timer = 0;
	   }
	   ball_timer += 100;
	   
	   display();
	   
		while (!TimerFlag);
		TimerFlag = 0;
   }
}

