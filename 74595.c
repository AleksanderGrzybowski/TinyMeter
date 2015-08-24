#include <avr/io.h>

#define DS_PORT PORTB
#define DS_DDR DDRB
#define DS_NUM (1 << PB0)

#define SHCP_PORT PORTB
#define SHCP_DDR DDRB
#define SHCP_NUM (1 << PB1)

#define STCP_PORT PORTB
#define STCP_DDR DDRB
#define STCP_NUM (1 << PB2)

void DS_1() {
	DS_PORT |= DS_NUM;
}
void DS_0() {
	DS_PORT &= ~DS_NUM;
}
void SHCP_1() {
	SHCP_PORT |= SHCP_NUM;
}
void SHCP_0() {
	SHCP_PORT &= ~SHCP_NUM;
}
void STCP_1() {
	STCP_PORT |= STCP_NUM;
}
void STCP_0() {
	STCP_PORT &= ~STCP_NUM;
}

void transmit(char b) {
	int cur;
	for (cur = 128; cur != 0; cur /= 2) {
		if (b & cur) {
			DS_1();
		} else {
			DS_0();
		}

//		delay_ms(1);
		SHCP_1();
//		delay_ms(1);
		SHCP_0();
//		delay_ms(1);
	}
}

void commit() {
//	delay_ms(1);
	STCP_1();
//	delay_ms(1);
	STCP_0();
//	delay_ms(1);
}
