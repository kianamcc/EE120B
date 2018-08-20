/*
 * kmccu003_lab10_part1.c
 *
 * Created: 8/20/2018 11:52:31 AM
 * Author : ucrcse
 */ 

#include <avr/io.h>
#include <ucr/bit.h>
#include "keypad.h"
#include "scheduler.h"
#include "lcd_8bit_task.h"
#include "queue.h"
#include "timer.h"
#include "stack.h"
#include "usart.h"
#include "seven_seg.h"
#include "scheduler.h"
#include "bit.h"
#include "io.h"

unsigned char x = 0x00;
unsigned char val = 0x00;

enum States{init, get}  state;

int keypad(int state) {
	
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
			break;
		
		default:
			state = init;
			x = 0x00;
			break;
	} //end transitions
	
	switch(state) { //actions
		
		case init:
			x = GetKeypadKey();
			break;
		
		case get:
			val = x;
			x = 0x00; //reset
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
	
    unsigned char x;
    unsigned char i = 0;
	
	task tasks[1];
	const unsigned char tasksNum;
	
const unsigned char tasksNum = 1;
//const unsigned long tasksPeriodGCD = 500;
const unsigned long keypad_period = 10;

	
  tasks[i].state = 0;
  tasks[i].period = keypad_period;
  tasks[i].elapsedTime = keypad_period;
  tasks[i].TickFct = &keypad;
	
	TimerOn();
	TimerSet(keypad_period);
	
	
    while(1) {
        x = GetKeypadKey();
		
// For each task, call task tick function if task's period is up
for (i=0; i < tasksNum; i++) {
	if (tasks[i].elapsedTime >= tasks[i].period){
		// Task is ready to tick, so call its tick function
		tasks[i].state = tasks[i].TickFct(tasks[i].state);
		tasks[i].elapsedTime = 0; // Reset the elapsed time
	}
	tasks[i].elapsedTime += 1;
}

        switch (x) {
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
        }
    }
}

/*Modify the keypad code to be in an SM task. Then, modify the keypad SM to utilize the simple task scheduler format (refer to PES Chp 7).
All code from here on out should use the task scheduler.*/