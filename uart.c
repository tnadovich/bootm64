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

void usart_getstr(char* str,int size) {
	// clear the buffer
	int j = 0;
	while (j<size)
		str[j++] = 0x00;

	// write into the buffer
	int i = 0;
	char c;
	while(i < size) { // read characters into buffer
		*(str + i++) = usart_getchar();
	}
}

uint8_t usart_gethexline(char* buffer, int16_t bufPtr) {
	// If the first charachter isn't a ':' then 
	if(usart_getchar() != ':') {
		usart_putchar('!');
		return -1;
	
	} else {
		// Set up the various chunks of data that we're getting from the line
		char hexSize[2];
		char byteAddr[4];
		char recordType[2];
		char checksum[2];
		char aaa[2];

		// Get the data size, byte address, and record type
		usart_getstr(hexSize,2);
		uint8_t size = hexstr2int(hexSize[0],hexSize[1]);
		usart_getstr(byteAddr,4);
		usart_getstr(recordType,2);
		// if the record type is 1 then this is the end of the file
		if (recordType[1] == '1') {
			usart_getstr(aaa,2);
			return 1;
		}
		// Set up the data buffer to the size specified by the earlier data
		char* data = malloc(2 * size * sizeof(char));
		// Get the data and the checksum
		usart_getstr(data,size*2);
		usart_getstr(checksum,2);

		usart_getstr(aaa,2);

		// write the data from the line into the page buffer
		uint8_t i = 0;
		while(i<size)
			buffer[bufPtr + i++] = (char) hexstr2int(data[2*i],data[(2*i)+1]);

		return 0;
	}
}
		
