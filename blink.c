#include "defines.h"
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	DDRG |= (1<<PG0)|(1<<PG1);
	PORTG = (1<<PG0);
	while(1) {
		PORTG ^= (1<<PG0)|(1<<PG1);
		_delay_ms(500);
	}
}
