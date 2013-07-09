#include "defines.h"
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/boot.h>
#define SPM_PAGESIZE 256

void boot_program_page(uint32_t page, uint8_t *buf) {
	uint16_t i;
	uint8_t sreg;

	// Disable interrupts.
	
	sreg = SREG;
	cli();

	eeprom_busy_wait();

	boot_page_erase_safe(page);
	boot_spm_busy_wait(); //Wait until the memory is erased.

	for(i=0;i<SPM_PAGESIZE;i+=2) {
		// Set up little-endian word.
		
		uint16_t w = *(buf++);
		w+= *(buf++) << 8;

		boot_page_fill_safe(page+i,w);
	}

	boot_page_write_safe(page+2); // Store buffer in flash page.
	boot_spm_busy_wait(); // Wait until the memory is written.

	// Reenable RWW-section again. We need this if we want to jump back
	// to the application after bootloading.
	
	boot_rww_enable_safe();

	// Re-enable interrupts (if they were ever enabled).
	
	SREG = sreg;
}

int main(void) {
	usart_init();
	usart_putchar('\n');
	DDRG |= (1<<PG0)|(1<<PG1);
	uint8_t buffer[SPM_PAGESIZE];
	uint16_t i = 0;
	char c;

	while(i<SPM_PAGESIZE) {
		c = usart_getchar();
		usart_putchar(c);
		buffer[i++] = c;
	}

	usart_putstr("Writing...\n");
	boot_program_page(0x0000,buffer);
	usart_putstr("Done\n");

	PORTG |= (1<<PG0);
	while(1) {
		PORTG ^= (1<<PG0)|(1<<PG1);
		_delay_ms(500);
	}
}
