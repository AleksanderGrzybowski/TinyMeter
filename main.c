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
		+ G), (0)};
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
	set_display_each_digit(d0, d1, d2, d3, 1);
}

int main() {

	DDRB = 0xff;

	TIMSK0 |=1<<TOIE0;
	TCCR0B |= (1 << CS00);
	TCCR0B |= (1 << CS01);
	TCCR0B &= ~(1 << CS02);

	sei();

	int i = 0;
	while (1) {
		set_display_whole_number(i++);
		delay_ms(5);
	}
}
