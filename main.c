#include "defines.h"
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/boot.h>

int main(void) {
	usart_init();
	usart_putchar('\n');
	DDRG |= (1<<PG0)|(1<<PG1);
	uint8_t buffer[SPM_PAGESIZE];
	uint16_t i = 0;

	while(i<SPM_PAGESIZE) {
		buffer[i++] = 0xAB;
	}
/*
	// Programming...
	usart_putchar('P');
	usart_putchar('r');
	usart_putchar('o');
	usart_putchar('g');
	usart_putchar('r');
	usart_putchar('a');
	usart_putchar('m');
	usart_putchar('m');
	usart_putchar('i');
	usart_putchar('n');
	usart_putchar('g');
	usart_putchar('.');
	usart_putchar('.');
	usart_putchar('.');
	usart_putchar('\n');
*/
	boot_program_page(0x0000,buffer);
/*	
	// Done
	usart_putchar('D');
	usart_putchar('o');
	usart_putchar('n');
	usart_putchar('e');
	usart_putchar('\n');
*/
	PORTG |= (1<<PG0);
	while(1) {
		PORTG ^= (1<<PG0)|(1<<PG1);
		_delay_ms(500);
	}
}
