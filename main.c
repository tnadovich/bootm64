#include <avr/io.h>
#include <avr/interrupt.h>
#include "defines.h"
#include <util/delay.h>
#include "boot.h"

void TC_init(void);

ISR (TIMER1_OVF_vect) {
	PORTG ^= (1<<PG0)|(1<<PG1);
}

void TC_init(void) {
	cli();
	// Enable overflow interrupts
	TIMSK = (1 << TOIE1);
	// Set counter to 0
	TCNT1 = 0x00;
	// Set control register (defaults)
	TCCR1A = 0x00;
	// Set clock source (divide by 8)
	TCCR1B = (0 << CS02)|(0 << CS01)|(1 << CS00);
	// Move interrupt vectors to boot flash
	MCUCR = (1 << IVCE);
	MCUCR = (1 << IVSEL);
	sei();
}

void TC_cleanup(void) {
	cli();
	MCUCR = (1 << IVCE);
	MCUCR = (0 << IVSEL);
}

int main(void) {
	DDRG |= (1<<PG0)|(1<<PG1);
	PORTG = (1<<PG0);
	TC_init();
	usart_init();
	while(1) {
		usart_putchar('A');
	}
}
