/*    Author : Kiana McCullough
 *    Lab Section: B21
 *    Assignment: Lab # 10 Exercise # 1
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <ucr/bit.h>
#include "keypad.h"
#include "scheduler.h"
#include "timer.h"
#include "bit.h"
#include "io.h"
#include "io.c"

//shared variables
///////////////////////////
unsigned char x = 0x1F;
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
			state = init;
			x = 0x1F;
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
			val = x;
			//x = 0x1F; //reset
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

	// Period for the tasks
	//unsigned long int keypad_calc = 10;
	
	//Calculating GCD
	//unsigned long int tmpGCD = 1;
	//tmpGCD = findGCD(tmpGCD, keypad_calc);
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = 10;

	//Recalculate GCD periods for scheduler
	unsigned long int keypad_period = 10;
	
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
	
	unsigned short i; // Scheduler for-loop iterator
	//unsigned char x;
	
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
            case '\0': PORTB = 0x1F; break; // All 5 LEDs on
            case '1': PORTB = 0x01; break; // hex equivalent
            case '2': PORTB = 0x02; break;
			case '3': PORTB = 0x03; break;
			case '4': PORTB = 0x04; break;
			case '5': PORTB = 0x05; break;
			case '6': PORTB = 0x06; break;
			case '7': PORTB = 0x07; break;
			case '8': PORTB = 0x08; break;
			case '9': PORTB = 0x09; break;
			case '0': PORTB = 0x00; break;

            // . . . ***** FINISH *****
			case 'A': PORTB = 0x0A; break;
			case 'B': PORTB = 0x0B; break;
			case 'C': PORTB = 0x0C; break;
            case 'D': PORTB = 0x0D; break;
            case '*': PORTB = 0x0E; break;
            case '#': PORTB = 0x0F; break;
            default: PORTB = 0x1B; break; // Should never occur. Middle LED off.
			
			while(!TimerFlag);
			TimerFlag = 0;
        } //end switch
    } //end while
	// Error: Program should not exit!
	return 0;
} //end main

/*Modify the keypad code to be in an SM task. Then, modify the keypad SM to utilize the simple task scheduler format (refer to PES Chp 7).
All code from here on out should use the task scheduler.*/