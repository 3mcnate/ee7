/********************************************
 *
 *  Name:
 *  Email:
 *  Section:
 *  Assignment: Lab 7 - Rotary Encoder
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"

void play_note(uint16_t);
void variable_delay_us(int16_t);

// Frequencies for natural notes from middle C (C4)
// up one octave to C5.
uint16_t frequency[8] =
    { 262, 294, 330, 349, 392, 440, 494, 523 };

volatile uint8_t new_state, old_state;
volatile uint8_t changed = 0;  // Flag for state change
volatile int16_t count = 0;		// Count to display
volatile uint8_t a, b;

int main(void) {

    // Initialize DDR and PORT registers and LCD
	PORTC |= (1 << PC1) | (1 << PC5);
	DDRB |= (1 << PB4);

	// enable interrupts for PC1 and PC5
	PCICR |= (1 << PCIE1);
	PCMSK1 |= (1 << PCINT9) | (1 << PCINT13);

	lcd_init();

    // Write a spash screen to the LCD
	lcd_moveto(0, 0);
	lcd_stringout("EE 109 Lab 7");
	lcd_moveto(1,0);
	lcd_stringout("Nate Boxer");
	_delay_ms(1000);
	lcd_moveto(1,0);
	lcd_stringout("          ");	

    // Read the A and B inputs to determine the intial state.
    // In the state number, B is the MSB and A is the LSB.
    // Warning: Do NOT read A and B separately.  You should read BOTH inputs
    // at the same time, then determine the A and B values from that value.    
	uint8_t x = PINC;
	a = x & (1 << PC1);
	b = x & (1 << PC5);

    if (!b && !a)
		old_state = 0;
    else if (!b && a)
		old_state = 1;
    else if (b && !a)
		old_state = 2;
    else
		old_state = 3;

    new_state = old_state;

	// enable global interrupts
	sei();

    while (1) {                 // Loop forever
		
		if (changed) { // Did state change?
			changed = 0;        // Reset changed flag

			// Output count to LCD
			char buf[10];
			snprintf(buf, 10, "%d", count);
			lcd_moveto(1,0);
			lcd_stringout("         ");
			lcd_moveto(1,0);
			lcd_stringout(buf);

			// Do we play a note?
			if ((count % 8) == 0) {
				// Determine which note (0-7) to play
				char note = (count % 64) / 8;
				if (note < 0) 
					note *= -1;

				// Find the frequency of the note
				// Call play_note and pass it the frequency
				play_note(frequency[note]);
			}
        }
    }
}

/*
  Play a tone at the frequency specified for one second
*/
void play_note(uint16_t freq)
{
    uint32_t period;

    period = 1000000 / freq;    // Period of note in microseconds

    while (freq--) {
	PORTB |= (1 << PB4);    // Buzzer output high
	variable_delay_us(period / 2);  // Delay for half the period
	PORTB &= ~(1 << PB4);   // Buzzer output low
	variable_delay_us(period / 2);  // Delay for half the period
    }
}

/*
    variable_delay_us - Delay a variable number of microseconds
*/
void variable_delay_us(int16_t delay)
{
    int i = (delay + 5) / 10;

    while (i--)
        _delay_us(10);
}

ISR(PCINT1_vect)
{
    // In Task 6, add code to read the encoder inputs and determine the new
    // count value

	// Read the input bits and determine A and B.
	uint8_t x = PINC;
	a = x & (1 << PC1);
	b = x & (1 << PC5);

	// convert a and b to 1 or 0
	a = (a >> PC1);
	b = (b >> PC5);

	// The following code is for Tasks 4 and later.
	// For each state, examine the two input bits to see if state
	// has changed, and if so set "new_state" to the new state,
	// and adjust the count value.
	if (old_state == 0) {
		// Handle A and B inputs for state 0
		if (a == 1) {
			new_state = 1;
			--count;
		}
		if (b == 1) {
			new_state = 2;
			++count;
		}

	}
	else if (old_state == 1) {

		// Handle A and B inputs for state 1
		if (a == 0) {
			new_state = 0;
			++count;
		}
		if (b == 1) {
			new_state = 3;
			--count;
		}

	}
	else if (old_state == 2) {

		// Handle A and B inputs for state 2
		if (a == 1) {
			new_state = 3;
			++count;
		}
		if (b == 0) {
			new_state = 0;
			--count;
		}

	}
	else {   // old_state = 3

		// Handle A and B inputs for state 3
		if (a == 0) {
			new_state = 2;
			--count;
		}
		if (b == 0) {
			new_state = 1;
			++count;
		}
	}		

	// If state changed, update the value of old_state,
	// and set a flag that the state has changed.
	if (new_state != old_state) {
		changed = 1;
		old_state = new_state;
	}
}


void timer1_init()
{
    // In Task 7, add code to inititialize TIMER1, but don't start it counting

}
ISR(TIMER1_COMPA_vect)
{
    // In Task 7, add code to change the output bit to the buzzer, and to turn
    // off the timer after enough periods of the signal

}
