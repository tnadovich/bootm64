#include <avr/io.h>
#include "defines.h"
#include "boot.h"

int main(void) {
	int page = 0x0000;
	usart_init();
	usart_putchar('\n');
	DDRG |= (1<<PG0)|(1<<PG1);
	uint8_t buffer[SPM_PAGESIZE];
	char c;
	uint8_t done = 0;

	uint16_t i;
	while(!done) {
		i = 0;
		while(i<SPM_PAGESIZE) {
			buffer[i++] = (char) 0xFF;
		}

		i = 0;
		while((i<(SPM_PAGESIZE)) && !done) {
			done = usart_gethexline(buffer,i);
			i += 16;
		}

		boot_program_page(page,buffer);
		page += SPM_PAGESIZE;
	}

	void (*start)(void);
	start = 0x0000;
	(*start)();
	
}
