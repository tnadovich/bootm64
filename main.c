#include <avr/io.h>
#include <avr/interrupt.h>
#include "defines.h"
#include <util/delay.h>
#include "boot.h"

void TC_init(void);

ISR (TIMER1_OVF_vect) {
	PORTG ^= (1<<PG0)|(1<<PG1);
	usart_putchar('A');
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
	TCCR1B = (0 << CS02)|(1 << CS01)|(0 << CS00);
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
	while(1);
}
	/*
	uint8_t bootIsDone = 0;
	uint16_t writePointer = 0x0000;
	uint8_t buffer[SPM_PAGESIZE];

	// Setup jump to application
	void (*start)(void);
	start = 0x0000;

	// Setup LEDs
	DDRG |= (1<<PG0)|(1<<PG1);
	PORTG = (1<<PG0);

	// Setup USART and interrupts
	usart_init();
	TC_init();

	// Fill buffer with ones by default
	uint16_t i = 0;
	while(i<SPM_PAGESIZE) {
		buffer[i++] = (char) 0xFF;
	}
	i = 0;

	// Write pages until a page write would overflow into the
	// bootloader or the finished signal is received
	while(((writePointer+SPM_PAGESIZE) < M64_BOOTSTART) && !bootIsDone) {
		// Read in 16 hex lines (one page) into the buffer
		i = 0;
		usart_putchar(XON);
		while(i<SPM_PAGESIZE) {
			bootIsDone = usart_gethexline(buffer,i);
			if (bootIsDone)
				break;
			i += 16;
		}

		usart_putchar(XOFF);
		// Write page from buffer
		if (!bootIsDone)
			boot_program_page(writePointer,buffer);

		// Increment write pointer and clear buffer
		writePointer += SPM_PAGESIZE;
		i = 0;
		while(i<SPM_PAGESIZE)
			buffer[i++] = (char) 0xFF;
	}
	
	// cleanup and jump to the program
	TC_cleanup();

	(*start)();
	
}*/
