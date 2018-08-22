/*    Author : Kiana McCullough
 *    Lab Section: B21
 *    Assignment: Lab # 10 Exercise # 3
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "keypad.h"
#include "timer.h"
#include "scheduler.h"
#include "bit.h"
#include "io.h"
#include "io.c"


//shared variables
///////////////////////////
unsigned char x = 0x1F; //if nothing is pressed
unsigned char val = 0x00;
//////////////////////////

enum States{init, get}  state;

int keypad(int state) { //keypad SM
	
	switch(state) {
		case init:
			if(x == 0x1F) {
				state = init;
			}
			else {
				state = get;
			}
			break;
		
		case get:
			if(x == 0x1F) {
				state = init;
			}
			else {
				state = get; //stay
			}
			break;
		
		default:
			state = init;
			x = 0x1F;
			break;
	} //end transitions
	
	switch(state) { //actions
		
		case init:
			x = GetKeypadKey();
			break;
		
		case get:
			x = GetKeypadKey();
			val = GetKeypadKey();
			break;
			
		default:
			break;
		
	} //end actions
	
return state;
}  //end keypad

int main(void)
{
    DDRB = 0xFF; PORTB = 0x00; // PORTB set to output, outputs init 0s
    DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s
	DDRA = 0xFF; PORTA = 0x00; //outputs
	DDRD = 0xFF; PORTD = 0x00;

	// Period for the tasks
	unsigned long int keypad_calc = 100;
	
	//Calculating GCD
	//unsigned long int tmpGCD = 1;
	//tmpGCD = findGCD(tmpGCD, keypad_calc);
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = 100;

	//Recalculate GCD periods for scheduler
	unsigned long int keypad_period = 100;
	
	//Declare an array of tasks 
	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);
	
	//task1
	task1.state = -1;//Task initial state.
	task1.period = keypad_period;//Task Period.
	task1.elapsedTime = keypad_period;//Task current elapsed time.
	task1.TickFct = &keypad;//Function pointer for the tick.
	
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	LCD_init();
	
	unsigned short i; // Scheduler for-loop iterator
	unsigned char tmp = 0;
	
    while(1) {

	// Scheduler code
	for ( i = 0; i < numTasks; i++ ) {
		// Task is ready to tick
		if ( tasks[i]->elapsedTime == tasks[i]->period ) {
			// Setting next state for task
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			// Reset the elapsed time for next tick.
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 1;
	}

	//x = GetKeypadKey();
        switch (val) {
			
			LCD_ClearScreen();
		
            case '\0': PORTB = 0x1F; break; // All 5 LEDs on
			case '0': PORTB = 0x00; tmp = 0; LCD_Cursor(1); LCD_WriteData(tmp + '0'); break;
            case '1': PORTB = 0x01; tmp = 1; LCD_Cursor(1); LCD_WriteData(tmp + '0'); break; // hex equivalent
            case '2': PORTB = 0x02; tmp = 2; LCD_Cursor(1); LCD_WriteData(tmp + '0'); break;
			case '3': PORTB = 0x03; tmp = 3; LCD_Cursor(1); LCD_WriteData(tmp + '0'); break;
			case '4': PORTB = 0x04; tmp = 4; LCD_Cursor(1); LCD_WriteData(tmp + '0'); break;
			case '5': PORTB = 0x05; tmp = 5; LCD_Cursor(1); LCD_WriteData(tmp + '0'); break;
			case '6': PORTB = 0x06; tmp = 6; LCD_Cursor(1); LCD_WriteData(tmp + '0'); break;
			case '7': PORTB = 0x07; tmp = 7; LCD_Cursor(1); LCD_WriteData(tmp + '0'); break;
			case '8': PORTB = 0x08; tmp = 8; LCD_Cursor(1); LCD_WriteData(tmp + '0'); break;
			case '9': PORTB = 0x09; tmp = 9; LCD_Cursor(1); LCD_WriteData(tmp + '0'); break;

            // . . . ***** FINISH *****
			case 'A': PORTB = 0x0A; tmp = 65; LCD_Cursor(1); LCD_WriteData(tmp); break;
			case 'B': PORTB = 0x0B; tmp = 66; LCD_Cursor(1); LCD_WriteData(tmp); break;
			case 'C': PORTB = 0x0C; tmp = 67; LCD_Cursor(1); LCD_WriteData(tmp); break;
            case 'D': PORTB = 0x0D; tmp = 68; LCD_Cursor(1); LCD_WriteData(tmp); break;
            case '*': PORTB = 0x0E; tmp = 42; LCD_Cursor(1); LCD_WriteData(tmp); break;
            case '#': PORTB = 0x0F; tmp = 35; LCD_Cursor(1); LCD_WriteData(tmp); break;
            default: PORTB = 0x1B; break; // Should never occur. Middle LED off.
			
			while(!TimerFlag);
			TimerFlag = 0;
        } //end switch
    } //end while
	// Error: Program should not exit!
	return 0;
} //end main

/*Combine the functionality of the keypad and LCD so when keypad is pressed and released, the character of the button pressed is displayed on the LCD,
and stays displayed until a different button press occurs (May be accomplished with two tasks: LCD interface & modified test harness).*/
