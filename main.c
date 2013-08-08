#include <avr/io.h>
#include <avr/interrupt.h>
#include "defines.h"
#include "boot.h"

void TC_init(void);

ISR (TIMER1_OVF_vect) {
	usart_putchar('A');
}

void TC_init(void) {
	// Disable global interrupts
	cli();
	// Enable overflow interrupts
	TIMSK = (1 << TOIE1);
	// Set counter to 0
	TCNT1 = 0x00;
	// Set control regster (defaults)
	TCCR1A = 0x00;
	// Set clock source (divide by 1024)
	TCCR1B = (1 << CS02)|(0 << CS01)|(1 << CS00);
	// Renable global interrupts
	sei();
}


int main(void) {
	DDRG |= (1<<PG0)|(1<<PG1);
	PORTG = (1<<PG0);
	int writePointer = 0x0000;
	usart_init();
	TC_init();
	DDRG |= (1<<PG0)|(1<<PG1);
	uint8_t buffer[SPM_PAGESIZE];

	// Fill buffer with ones by default
	uint16_t i = 0;
	while(i<SPM_PAGESIZE) {
		buffer[i++] = (char) 0xFF;
	}
	i = 0;

	// Write pages until a page write would overflow into the bootloader
	while((writePointer+SPM_PAGESIZE) < M64_BOOTSTART) {
		// Read in 16 hex lines, one page into the buffer
		i = 0;
		while(i<SPM_PAGESIZE) {
			usart_putchar(XON);
			usart_gethexline(buffer,i);
			usart_putchar(XOFF);
			i += 16;
		}

		// Write page from buffer
		boot_program_page(writePointer,buffer);

		// Incrememnt write pointer and clear buffer
		writePointer += SPM_PAGESIZE;
		i = 0;
		while(i<SPM_PAGESIZE)
			buffer[i++] = (char) 0xFF;
	}

	void (*start)(void);
	start = 0x0000;
	(*start)();
}
