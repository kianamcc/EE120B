/*
 * kmccu003_lab9_part2.c
 *
 * Created: 8/17/2018 5:39:06 PM
 * Author : ANARCHY
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


enum State{init, sound_press, sound_release, up_press, up_release, down_press, down_release, c4, d4, e4} state;

	unsigned char button_0 = 0;
	unsigned char button_1 = 0;
	unsigned char button_2 = 0;
	unsigned char flag = 0x00;
	unsigned char i = 0;
	double tone[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
	
void tones() {
	
	button_0 = ~PINA & 0x01; //PA0 on/off
	button_1 = ~PINA & 0x02; //PA1 scale up
	button_2 = ~PINA & 0x04; //PA2 scale down
	
	switch(state) {//transitions


			case init:
			if(button_0) {
				state = sound_press;
			}
		
			else if (button_1) {
				state = up_press;
			}
		
			else if(button_2) {
				state = down_press;
			}
		
		else {
			state = init;
		}
			break;
			
		case sound_press:
			if(button_0) {
				state = sound_press;
			}
			else if(!button_0) {
				state = sound_release;
			}
			break;
			
		case sound_release:
			state = init;
			break;
		
		case up_press:
		if(button_1) {
			state = up_press;
		}
		else if(!button_1) {
			state = up_release;
		}
		break;
		
		case up_release:
			state = init;
			break;
		
		case down_press:
		if(button_2) {
			state = down_press;
		}
		else if(!button_2) {
			state = down_release;
		}
		break;
		
		case down_release:
			state = init;
			break;
			
			default:
			state = init;
			break;
			
			
	} //end transitions
	
	switch(state) { //actions
		
		case init:
		break;
		
		case sound_release:
		if(!flag) {
			set_PWM(tone[i]);
			flag = 0x01; //on
		}
		else {
			set_PWM(0);
			flag = 0x00; //off
		}
		break;
		
		case up_release:
		if(i < 7 && flag) {
			i++;
		}
		if(flag) {
			set_PWM(tone[i]); //know where you left off
		}
		break;
		
		case down_release:
		if(i > 0 && flag) {
			i--;
		}
		if(flag) {
			set_PWM(tone[i]); //know where you left off
		}
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
	
	state = init; //was init
	
	PWM_on();
	
    while (1) 
    {
		tones();
    }
}

/*Using the ATmega1284’s PWM functionality, design a system where the notes:
C4, D, E, F, G, A, B, and C5,  from the table at the top of the lab, can be generated on the speaker by
scaling up or down the eight note scale. Three buttons are used to control the system. One button toggles sound on/off.
The other two buttons scale up, or down, the eight note scale.

Criteria:
The system should scale up/down one note per button press.
When scaling down, the system should not scale below a C.
When scaling up, the system should not scale above a C.
Hints:
Breaking the system into multiple synchSMs could make this part easier.*/
