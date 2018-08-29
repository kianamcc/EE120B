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
//#include "LiquidCrystal.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char button = 0x00;
unsigned char play_flag = 0;
unsigned char p1_score = 0;
unsigned char p2_score = 0;
unsigned char disp_flag = 0;
unsigned char win = 0;
unsigned char button_flag = 0;
unsigned char count_right = 28;

struct p1_struct { //type of struct
	
	unsigned char y; //coordinates
	unsigned char x;
	unsigned char xr; //right x coordinate
	unsigned char xl; //left x coordinate
	
} p1;

void p1_init() {
	p1.y = 1;
	p1.x = 28;
	p1.xr = 16; //multiply by 2 each time paddle moves right and divide by two when it moves left
	p1.xl = 4;
}

struct p2_struct { //type of struct
	
	unsigned char y2; //coordinates
	unsigned char x2;
	unsigned char xr2;
	unsigned char xl2;
	
} p2;

void p2_init() {
	p2.y2 = 8;
	p2.x2 = 28;
	p2.xr2 = 16; //multiply by 2 each time paddle moves right and divide by two when it moves left
	p2.xl2 = 4;
}

struct ball_struct { //type of struct
	
	unsigned int direction; //0 down, 1 up, 2 right-up, 3 left-up, 4 right-down, 5 left-down
	unsigned char y; //coordinates
	unsigned char x;
	
} ball;

void ball_init() {
	
	//matrix_write(ball.x, 0);
	ball.direction = 0;
	ball.y = 5;
	ball.x = 8;
	matrix_write(ball.y, ball.x); //p2 paddle in middle
}
enum Menu {init, play, matrix, gameover, reset, button_release, butt_release2, butt_release3} menu;

void Menu() {
	
	button = ~PINC & 0x01; //C0
	
	switch(menu) { //transitions
		
		case init:
			if(button == 0x00) {
				menu = init;
			}
		   
		   else if(button == 0x01) {
			   menu = button_release;
		   }
		   break;
		   
		case button_release:
			if(button == 0x00) {
				menu = matrix;
			}
			else {
				menu = button_release;
			}
			break;
			
		case butt_release2:
			if(button == 0x00) {
				menu = reset;
			}
			else {
				menu = butt_release2;
			}
			break;
			
		case play:
			if (p1_score == 7 || p2_score == 7 )
			{
				menu = gameover; //gameover;
			}
			else if (button)
			{
				menu = butt_release2;
			}
			else
			{
				menu = play;
			}
			/*else if(win == 1) {
				win = 0; //reset
				menu = init;
			}*/
			
			break;
			
		case matrix:
			menu = play;
		   break;
		  
		 case gameover:
		 	if (p1_score == 7) {
			 	LCD_ClearScreen();
			 	LCD_DisplayString(1, "PLAYER 1 WINS!");
			 	win = 1;
			 	//matrix_clear();
			 	//disp_flag = 0;
		 	}
		 					
		 	if (p2_score == 7) {
			 	LCD_ClearScreen();
			 	LCD_DisplayString(1, "PLAYER 2 WINS!");
			 	win = 1;
			 	//matrix_clear();
			 	//disp_flag = 0;
		 	}
			if (button)
			{
				menu = butt_release2;
			}
			else
			{
				menu = gameover;
			}
			break;
		 
		 case reset:
			//matrix_clear();
			menu = init;
			break;
		   
		default:
			menu = init;
			break;
		   
	}
	   
	switch(menu) { //actions
		
		case init:
			LCD_DisplayString(1, "PRESS TO PLAY");
			break;
			
		case button_release:
			break;  
		
		case butt_release2:
			break;
			
		case butt_release3:
			break;
		
		case matrix:
		 play_flag = 1;
		 break;
			
		case play:
		
			if(disp_flag == 0) {
			LCD_ClearScreen();
			LCD_DisplayString(1, "     SCORE:     P1: 0      P2: 0");
			disp_flag = 1;
			}
			
			LCD_Cursor(21);
			LCD_WriteData(p1_score +'0');
			LCD_Cursor(32);
			LCD_WriteData(p2_score +'0');
			
			break;
		   
		   //case gameover:
					//if (p1_score == 7) {
						//LCD_ClearScreen();
						//LCD_DisplayString(1, "PLAYER 1 WINS!");
						//win = 1;
						////matrix_clear();
						////disp_flag = 0;
					//}
					//
					//if (p2_score == 7) {
						//LCD_ClearScreen();
						//LCD_DisplayString(1, "PLAYER 2 WINS!");
						//win = 1;
						////matrix_clear();
						////disp_flag = 0;
					//}
					//break;
		   
			case reset:
				
				LCD_ClearScreen();
				disp_flag = 0;
				p1_score = 0;
				p2_score = 0;
				//ball_init();
				count_right = 28;
				play_flag = 0;
				p1_init();
				p2_init();
				ball_init();
				break;
		   
		  default:
			  break;
		   
	}
	
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//struct p1_struct { //type of struct
	//
	//unsigned char y; //coordinates
	//unsigned char x;
	//unsigned char xr; //right x coordinate
	//unsigned char xl; //left x coordinate
	//
//} p1;
//
//void p1_init() {
	//p1.y = 1;
	//p1.x = 28;
	//p1.xr = 16; //multiply by 2 each time paddle moves right and divide by two when it moves left
	//p1.xl = 4;
//}

unsigned char button_right = 0x00;
unsigned char button_left = 0x00;
//unsigned char reset_button = 0x00;
unsigned char p1_index = 3;

enum player_1{init_p1, wait_p1, press_p1, right_press, left_press, release_p1} p1_state;

void P1() {
	
	//count_right = 28;
	//reset_button = ~PINC & 0x01; //C0
	button_right = ~PINC & 0x02; //C1
	button_left = ~PINC & 0x04; //C2
	
	switch(p1_state) { //transitions
		case init_p1:
			if (play_flag)
			{
				p1_state = wait_p1;
			}
			else
			{
				p1_state = init_p1;
			}
			break;
		
		case wait_p1:
			if (play_flag) {
				if(!button_right && !button_left) { //stay until a button is pressed
					p1_state = wait_p1;
				}
			
				else if(button_right && !button_left) {
					p1_state = right_press;
				}
			
				else if(!button_right && button_left) {
					p1_state = left_press;
				}
			}
			else
			{
				p1_state = init_p1;
			}
			//else if(reset_button) {
				//p1_state = reset;
			//}
			
			break;
			
		case right_press: //execute command then go straight to release
		
			//if(reset_button) {
				//p1_state = reset;
			//}
			
			//else {
				p1_state = release_p1;
				break;
			//}
			
		case left_press:
		
			//if(reset_button) {
				//p1_state = reset;
			//}
			
			//else {
				p1_state = release_p1;
				break;
			//}
		
		case release_p1: //wait for next press

			if(!button_right && !button_left) { //released
				p1_state = press_p1;
			}
			
			//else if(reset_button) {
				//p1_state = reset;
			//}
			
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
			
			//else if(reset_button) {
				//p1_state = reset;
			//}
			else {
				p1_state = press_p1;
			}
			break;
		
		//case reset:
			//p1_state = wait_p1;
			//break;
			//
		default:
			p1_state = init_p1;
			break;
			
	}
	
	switch(p1_state) { //actions
		case init_p1:
			p1_init();
			break;
		
		case wait_p1:
			break;
		
		case press_p1:
			break;
		
		case right_press:

			if(p1.x < 224 && p1.xr < 128) {
				p1.x  = p1.x * 2;
				p1.xr = p1.xr * 2; //individual bit, NEW
				p1.xl = p1.xl * 2;
			}
			
			//matrix_write(p1.y, p1.x);
			break;
		
		case left_press:

			if(p1.x != 7) {
				p1.x = p1.x / 2;
				p1.xr = p1.xr / 2; //individual bit, NEW
				p1.xl = p1.xl / 2;
			}
			
			//matrix_write(p1.y, p1.x);
			break;
		
		case release_p1:
			break;
		
		//case reset: 
			//matrix_clear();
			//LCD_ClearScreen();
			//disp_flag = 0;
			//p1_score = 0;
			//p2_score = 0;
			////ball_init();
			//count_right = 28;
			//play_flag = 0;
			//break;	
		default:
			break;
	}	
} //end player 1
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//struct p2_struct { //type of struct
	//
	//unsigned char y2; //coordinates
	//unsigned char x2;
	//unsigned char xr2;
	//unsigned char xl2;
	//
//} p2;
//
//void p2_init() {
	//p2.y2 = 8;
	//p2.x2 = 28;
	//p2.xr2 = 16; //multiply by 2 each time paddle moves right and divide by two when it moves left
	//p2.xl2 = 4;
//}

unsigned char button_right_2 = 0x00;
unsigned char button_left_2 = 0x00;
//unsigned char reset_button_2 = 0x00;
unsigned char count_right_2 = 28;


enum player_2{init_p2, wait_p2, press_p2, right_press_p2, left_press_p2, release_p2} p2_state;

void P2() {
	
	//reset_button_2 = ~PINC & 0x01; //C0
	button_right_2 = ~PINC & 0x08; //C3
	button_left_2 = ~PINC & 0x10; //C4
	
	switch(p2_state) { //transitions
		case init_p2:
			p2_init();
			if (play_flag)
			{
				p2_state = wait_p2;
			}
			else
			{
				p2_state = init_p2;
			}
			break;
		
		case wait_p2:
			if(play_flag) {
				if(!button_right_2 && !button_left_2) { //stay until a button is pressed
					p2_state = wait_p2;
				}
		
				else if(button_right_2 && !button_left_2) {
					p2_state = right_press_p2;
				}
		
				else if(!button_right_2 && button_left_2) {
					p2_state = left_press_p2;
				}
		
				//else if(reset_button_2) {
					//p2_state = reset_p2;
				//}
				//
			}
			else
			{
				p2_state = init_p2;
			}
			break;
		
		case right_press_p2: //execute command then go straight to release
		//
			//if(reset_button_2) {
				//p2_state = reset_p2;
			//}
		
			//else {
				p2_state = release_p2;
				break;
			//}
		
		case left_press_p2:
		
			//if(reset_button_2) {
				//p2_state = reset_p2;
			//}
		
			//else {
				p2_state = release_p2;
				break;
			//}
		
		case release_p2: //wait for next press

			if(!button_right_2 && !button_left_2) { //released
				p2_state = press_p2;
			}
		
			//else if(reset_button_2) {
				//p2_state = reset_p2;
			//}
		
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
			
			//else if(reset_button_2) {
				//p2_state = reset_p2;
			//}
			
			else {
				p2_state = press_p2;
			}
			break;
		
			//case reset_p2:
			//p2_state = wait_p2;
			//break;
		
		default:
		p2_state = init_p2;
		break;
	}
	
	switch(p2_state) { //actions
		
		case wait_p2:
		break;
		
		case press_p2:
		break;
		
		case right_press_p2:

			if(p2.x2 < 224) {
				p2.x2  = p2.x2 * 2;
				p2.xr2 = p2.xr2 * 2; //individual bit, NEW
				p2.xl2 = p2.xl2 * 2;
			}
	
			//matrix_write(p1.y, p1.x);
			break;
		
		case left_press_p2:
		
			if(p2.x2 != 7) {
				p2.x2 = p2.x2 / 2;
				p2.xr2 = p2.xr2 / 2; //individual bit, NEW
				p2.xl2 = p2.xl2 / 2;
			}
			
			//matrix_write(p1.y, p1.x);
			break;
		
		case release_p2:
			break;
		
		//case reset_p2:
			//matrix_clear();
			//LCD_ClearScreen();
			//disp_flag = 0;
			//p1_score = 0;
			//p2_score = 0;
			//count_right = 28;
			//play_flag = 0;
		default:
			break;
	}
} //end player 2
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char ball_flag = 0;

//struct ball_struct { //type of struct
	//
	//unsigned int direction; //0 down, 1 up, 2 right-up, 3 left-up, 4 right-down, 5 left-down
	//unsigned char y; //coordinates
	//unsigned char x;
	//
//} ball;
//
//void ball_init() {
	//
	////matrix_write(ball.x, 0);
	//ball.direction = 0;
	//ball.y = 5;
	//ball.x = 8;
	//matrix_write(ball.y, ball.x); //p2 paddle in middle
//}

unsigned char p1_ball_check() {
	return ball.x & p1.x;
}

unsigned char p2_ball_check() {
	return ball.x & p2.x2;
}

unsigned char ball_column_index = 5;
unsigned char ball_row_index = 8;

enum ball_movement{init_ball, ball_start, ball_vertical/*, ball_p1, ball_2*/} ball_state; //top right, bottom right, etc...

void Ball() {
	
	switch(ball_state) { //transitions
		
		case init_ball:
			if (play_flag == 1)
			{
				ball_init();
				ball_state = ball_start;
			}
			else
			{
				ball_state = init_ball;
			}
			break;
			
			
		case ball_start:
			if (play_flag) {
				if (ball.y <= 2) { //ball near p1
					ball_state = ball_vertical;
				}
			
				if (ball.y >= 7)
				{
					ball_state = ball_vertical;
				}
				if(p1_score == 7 || p2_score == 7) {
					ball_state = init_ball;
				}
			}
			else
			{
				ball_state = init_ball;
			}
			break;
			
		case ball_vertical:
			ball_state = ball_start;
			break;
			
		default:
			ball_state = init_ball;
			break;
	}
	
	switch(ball_state) { //actions
		
		case init_ball:
			break;
			
		case ball_start: //ball go down towards p1
		
			matrix_write(ball.y, 0);
			matrix_write(ball.x, 0);
			
			if (ball.direction == 0) { //move down from p2
				ball.y -= 1;
			}
			if (ball.direction == 1) //move up from p1
			{
				ball.y += 1;
			}
			if (ball.direction == 2) //move right from p1
			{
				if(ball.x < 128) {
					ball.y += 1;
					ball.x *= 2;
				}
				else { 
					ball.direction = 3;
					break;
				}
			}
			if (ball.direction == 3) //move left from p1
			{
				if(ball.x > 1) {
					ball.y += 1;
					ball.x /= 2;
				}
				else {
					ball.direction = 2;
					break;
				}
			}
			
			if (ball.direction == 4) { //right from p2
				if(ball.x < 128) {
					ball.y -= 1;
					ball.x *= 2;
				}
				else { 
					ball.direction = 5;
					break;
				}
			}
			
			if (ball.direction == 5) //move left from p2
			{
				if(ball.x > 1) {
					ball.y -= 1;
					ball.x /= 2;
				}
				
				else {
					ball.direction = 4;
					break;
				}
			}
			matrix_write(ball.y, ball.x);
			break;
			
			case ball_vertical:
				if(ball.y <= 2) { //check player coordinate,
					if (p1_ball_check())
					{
						if(p1.xr != ball.x && p1.xl != ball.x) { //if not rightmost bit and leftmost bit
							ball.direction = 1;
						}
						
						else if(p1.xr == ball.x) {
							if(p1.xr != 128) { //move vertical when paddle not moving
								ball.direction = 2; //right up
							}
							else if (p1.xr == 128) {
								ball.direction = 3;
							}
						}
							
						else if(p1.xl == ball.x) //left up
						{
							ball.direction = 3;
						}
					}
					else if(ball.y == 1) //if it reaches ground
					{
						matrix_write(ball.y, 0);
						p2_score++;
						//score(p1_score, p2_score);
						ball_init();
					}
				} //end p1 check
				
				if (ball.y >= 7) //if near p2
				{
					if(p2_ball_check()) //if p2 hits it
					{
						if (p2.xr2 != ball.x && p2.xl2 != ball.x) { //middle
							ball.direction = 0; //down
						}
					
						else if(p2.xr2 == ball.x) { //right side of paddle, not left
							if(p2.xr2 != 128) {
								ball.direction = 4; //right from p2
							}
							else if(p2.xr2 == 128) {
								ball.direction = 5;
							}
						}
						
						else if(p2.xl2 == ball.x) //left from p2
						{
							if(p2.xl2 != 1) {
							ball.direction = 5;
							}
							else if(p2.xl2 == 1) {
								ball.direction = 4;
							}
						}
					}
					
					else if(ball.y == 8) //reaches p2 ground
					{
						matrix_write(ball.y, 0);
						p1_score++;
						//score(p1_score, p2_score);
						ball_init();
					}
				}
				break;
				
		default:
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
		case init_led:
			break;
		
		case update:
			if (play_flag) {
				matrix_write(p1.y, p1.x); //p1 paddle in middle
				matrix_write(p2.y2, p2.x2);
				matrix_write(ball.y, ball.x); //p2 paddle in middle
			}
			else
			{
				matrix_clear();
			}
			break;
	}
} //end display

//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    /* Replace with your application code */
	DDRB = 0xFF; PORTB = 0x00; //matrix
	DDRC = 0x00; PORTC = 0xFF; //input buttons
	
	DDRA = 0xFF; PORTA = 0x00; //lcd
	DDRD = 0xFF; PORTD = 0x00; //lcd
	
	unsigned long ball_timer = 50;
	
	menu = init;
	
	matrix_init();
	
	TimerSet(50);
	TimerOn();
	
   // Initializes the LCD display
   LCD_init();
   
   
   while(1) {
	   
	   Menu();
	   
	   P1();
	   
	   P2();
	   
	   if(ball_timer == 100) {
		  Ball();
		  ball_timer = 0;
	   }
	   ball_timer += 50;
	   
	   display();
	   
		while (!TimerFlag);
		TimerFlag = 0;
   }
}

