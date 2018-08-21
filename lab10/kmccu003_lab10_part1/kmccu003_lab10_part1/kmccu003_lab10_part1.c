/*
 * kmccu003_lab10_part1.c
 *
 * Created: 8/20/2018 11:52:31 AM
 * Author : ucrcse
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <ucr/bit.h>
#include "keypad.h"
#include "scheduler.h"
//#include "lcd_8bit_task.h"
//#include "queue.h"
#include "timer.h"
//#include "stack.h"
//#include "usart.h"
//#include "seven_seg.h"
#include "scheduler.h"
#include "bit.h"
#include "io.h"
#include "io.c"

// Returns '\0' if no key pressed, else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port C
/* Keypad arrangement
        PC4 PC5 PC6 PC7
   col  1   2   3   4
row
PC0 1   1 | 2 | 3 | A
PC1 2   4 | 5 | 6 | B
PC2 3   7 | 8 | 9 | C
PC3 4   * | 0 | # | D
*/
/*unsigned char GetKeypadKey() {

    PORTC = 0xEF; // Enable col 4 with 0, disable others with 1’s
    asm("nop"); // add a delay to allow PORTC to stabilize before checking
    if (GetBit(PINC,0)==0) { return('1'); }
    if (GetBit(PINC,1)==0) { return('4'); }
    if (GetBit(PINC,2)==0) { return('7'); }
    if (GetBit(PINC,3)==0) { return('*'); }

    // Check keys in col 2
    PORTC = 0xDF; // Enable col 5 with 0, disable others with 1’s
    asm("nop"); // add a delay to allow PORTC to stabilize before checking
    if (GetBit(PINC,0)==0) { return('2'); }
	if (GetBit(PINC, 1)==0) { return('5'); }
	if (GetBit(PINC, 2)==0) { return('8'); }
	if (GetBit(PINC, 3)==0) { return('0'); }
    // ... *****FINISH*****

    // Check keys in col 3
    PORTC = 0xBF; // Enable col 6 with 0, disable others with 1’s
    asm("nop"); // add a delay to allow PORTC to stabilize before checking
	if (GetBit(PINC, 0)==0) { return('3'); }
	if (GetBit(PINC, 1)==0) { return('6'); }
	if (GetBit(PINC, 2)==0) { return('9'); }
	if (GetBit(PINC, 3)==0) { return('#'); }
    // ... *****FINISH*****

    // Check keys in col 4
	PORTC = 0x7F;
	asm("nop");
	if (GetBit(PINC, 0)==0) { return('A'); }
	if (GetBit(PINC, 1)==0) { return('B'); }
	if (GetBit(PINC, 2)==0) { return('C'); }
	if (GetBit(PINC, 3)==0) { return('D'); }
    // ... *****FINISH*****

    return('\0'); // default value
}*/

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
			val = x;
			x = 0x1F; //reset
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
	unsigned long int keypad_calc = 10;
	
	//Calculating GCD
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(tmpGCD, keypad_calc);
	
	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int keypad_period = keypad_calc/GCD;
	
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
	//while(!TimerFlag);
	//TimerFlag = 0;
//} //end scheduler

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

            // . . . ***** FINISH *****
			case 'A': PORTB = 0x0A; break;
			case 'B': PORTB = 0x0B; break;
			case 'C': PORTB = 0x0C; break;
            case 'D': PORTB = 0x0D; break;
            case '*': PORTB = 0x0E; break;
            case '0': PORTB = 0x00; break;
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