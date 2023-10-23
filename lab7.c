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

int main(void) {
    uint8_t new_state, old_state;
    uint8_t changed = 0;  // Flag for state change
    int16_t count = 0;		// Count to display
    uint8_t a, b;

    // Initialize DDR and PORT registers and LCD
	PORTC |= (1 << PC1) | (1 << PC5);

	lcd_init();

    // Write a spash screen to the LCD


    // Read the A and B inputs to determine the intial state.
    // In the state number, B is the MSB and A is the LSB.
    // Warning: Do NOT read A and B separately.  You should read BOTH inputs
    // at the same time, then determine the A and B values from that value.    

    if (!b && !a)
	old_state = 0;
    else if (!b && a)
	old_state = 1;
    else if (b && !a)
	old_state = 2;
    else
	old_state = 3;

    new_state = old_state;

    while (1) {                 // Loop forever
		// Read the input bits and determine A and B.
		uint8_t x = PINC;
		a = x & (1 << PC1);
		b = x & (1 << PC5);

		// For the Checkpoint, print the values of A and B on the LCD.
		a = (a >> PC1);
		b = (b >> PC5);

		char buf[10];
 		snprintf(buf, 10, "A=%d B=%d", a, b);
		//lcd_writecommand(1);
		lcd_moveto(0,0);
		lcd_stringout(buf);


		// The following code is for Tasks 4 and later.
		// For each state, examine the two input bits to see if state
		// has changed, and if so set "new_state" to the new state,
		// and adjust the count value.
		if (old_state == 0) {

			// Handle A and B inputs for state 0

		}
		else if (old_state == 1) {

			// Handle A and B inputs for state 1

		}
		else if (old_state == 2) {

			// Handle A and B inputs for state 2

		}
		else {   // old_state = 3

			// Handle A and B inputs for state 3

		}

		// If state changed, update the value of old_state,
		// and set a flag that the state has changed.
		if (new_state != old_state) {
			changed = 1;
			old_state = new_state;
		}

			if (changed) { // Did state change?
			changed = 0;        // Reset changed flag

			// Output count to LCD

			// Do we play a note?
			if ((count % 8) == 0) {
			// Determine which note (0-7) to play

			// Find the frequency of the note

			// Call play_note and pass it the frequency

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
	PORTB &= ~(1 << PB4);   // Buzzer output log
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