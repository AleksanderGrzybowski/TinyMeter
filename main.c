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

int main() {

	DDRB = 0xff;
	while(1) {
		PORTB ^= 0xff;
		delay_ms(1000);
	}

}
