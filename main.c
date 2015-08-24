#define F_CPU 9600000

#include <avr/io.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <avr/signal.h>

#include "74595.h"

void delay_ms(uint16_t count) {
	while (count--) {
		_delay_ms(1);
	}
}
void delay_us(uint16_t count) {
	while (count--) {
		_delay_us(1);
	}
}

#define DOT 128
#define G 64
#define F 32
#define E 16
#define D 8
#define C 4
#define B 2
#define A 1

char tab[11] = { (A + B + C + D + E + F), (B + C), (A + B + G + E + D), (A + B
		+ G + C + D), (F + G + B + C), (A + F + G + C + D), (A + F + G + E + D
		+ C), (F + A + B + C), (A + B + C + D + E + F + G), (A + B + C + D + F
		+ G), (0) };
#define EMPTY_DIGIT 10

volatile char display[4]; // dot handled below
volatile int cur_digit = 0;
volatile int dot_on = 0; // 1 = dot is on, 0 = dot is off

ISR(TIM0_OVF_vect) {
	cur_digit++;
	if (cur_digit == 4) {
		cur_digit = 0;
	}

	uint8_t tosend_segments = ~(1 << cur_digit);
	uint8_t tosend_leds = ~display[cur_digit];
	if (cur_digit == 1 && dot_on) {
		tosend_leds &= ~DOT;
	}

	transmit(tosend_leds);
	transmit(tosend_segments);
	commit();
}

void set_display_each_digit(int dig0, int dig1, int dig2, int dig3, int dot) {
	display[0] = tab[dig0];
	display[1] = tab[dig1];
	display[2] = tab[dig2];
	display[3] = tab[dig3];
	dot_on = dot;
}

void set_display_whole_number(int number) {
	int d3 = number % 10;
	number /= 10;
	int d2 = number % 10;
	number /= 10;
	int d1 = number % 10;
	number /= 10;
	int d0 = number % 10;

	// clear leading 0's
	if (d0 == 0) {
		d0 = EMPTY_DIGIT;
		if (d1 == 0) {
			d1 = EMPTY_DIGIT;
		}
	}

	set_display_each_digit(d0, d1, d2, d3, 1);
}

int main() {

	DDRB = 0xff;
	DDRB &= ~(1 << PB3);

	TIMSK0 |= 1 << TOIE0;
	TCCR0B |= (1 << CS00);
	TCCR0B |= (1 << CS01);
	TCCR0B &= ~(1 << CS02);
	sei();

	ADMUX |= (1 << REFS0); // internal 1.1V

	ADMUX |= (1 << MUX0); // PB3
	ADMUX |= (1 << MUX1); // PB3
	DIDR0 |= (1 << ADC3D); // PB3

	ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADPS2) | (1 << ADEN); // clk/8

	while (1) {
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1 << ADSC)) {
		}

		uint32_t adc_read = ADC;
		uint32_t volts_4digit = (adc_read * 110) / 1024;
		set_display_whole_number(volts_4digit);
		delay_ms(500);
	}
}
