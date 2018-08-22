/*    Author : Kiana McCullough
 *    Lab Section: B21
 *    Assignment: Lab # 9 Exercise # 1
 *    
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */

#include <avr/io.h>

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64                    // 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	//TCCR0A = (1 << COM0A0 );
	TCCR0A = (1 << COM0A0 | 1 << WGM00);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}


enum State{/*init,*/ wait, c4, d4, e4} state;
	//enum State{init, wait, on, off} state;
	unsigned char button_0 = 0;
	unsigned char button_1 = 0;
	unsigned char button_2 = 0;
	
void tones() {
	
	button_0 = ~PINA & 0x01; //PA0
	button_1 = ~PINA & 0x02; //PA1
	button_2 = ~PINA & 0x04; //PA2
	
	switch(state) {//transitions
		/*case init:
			state = wait;
			break;*/

			case wait:
			if(button_0 && !button_1 && !button_2) {
				state = c4;
			}
		
			else if (!button_0 && button_1 && !button_2) {
				state = d4;
			}
		
			else if(!button_0 && !button_1 && button_2) {
				state = e4;
			}
			
			/*else if(!button_0 && !button_1 && !button_2) {
				state = wait;
			}*/
		
			else { 
				state = wait;
			}
			break;
			
		case c4:
			if(button_0 && !button_1 && !button_2) {
				state = c4;
			}
			else {
				state = wait;
			}
			break;
		
		case d4:
			if(!button_0 && button_1 && !button_2) {
				state = d4;
			}
			else {
				state = wait;
			}
			break;
		
		case e4:
			if(!button_0 && !button_1 && button_2) {
				state = e4;
			}
			else {
				state = wait;
			}
			break;
			
			default:
			state = wait; //was init
			break;
			
			
	} //end transitions
	
	switch(state) { //actions
		
		/*case init:
		set_PWM(0); //stop speaker from generating sound
		break;*/
		
		case wait:
		set_PWM(0);
		break;
		
		case c4:
		set_PWM(261.63);
		break;
		
		case d4:
		set_PWM(293.66);
		break;
		
		case e4:
		set_PWM(329.63);
		break;
		
		default:
		break;
		
		
	} //end actions
	
} //end tones fct




int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF; //input buttons
	DDRB = 0x08; PORTB = 0x00; //output speaker
	
	state = wait; //was init
	
	PWM_on();
	
    while (1) 
    {
		tones();
    }
}

/*Use the tones C4, D4, and E4 from the table in the introduction section.
When a button is pressed and held, the tone mapped to it is generated on the speaker.
When more than one button is pressed simultaneously, the speaker remains silent.
When no buttons are pressed, the speaker remains silent.*/

