#include "uart.h"

void usart_init(void) {
	// Initialize Send and Receive
	UCSR0B = ((1<<TXEN0)|(1<<RXEN0));
	// Initialize the baud rate
	UBRR0L = UBRRL_VALUE;
	UBRR0H = UBRRH_VALUE;
	// Set frame format (8 bits\1 stop bit\asynchronous\no parity)
	UCSR0C = (3<<UCSZ00);
#if USE_2X
	UCSR0A |= (1 << U2X);
#else
	UCSR0A &= ~(1 << U2X);
#endif
}

void usart_putchar(char c) {
	// Simplify carriage return
	if (c == '\n')
		usart_putchar('\r');

	// Wait for transmit buffer to be ready
	while(!(UCSR0A & (1<<UDRE)));
	// Send character
	UDR0 = c;

	return;
}

char usart_getchar(void)
{
	// Wait for buffer to be full
	while(!(UCSR0A & (1<<RXC0)));
	// Return character
	return UDR0;
}

void usart_putstr(char* s) {
	while(*s != 0x00)
		usart_putchar(*(s++));
}
