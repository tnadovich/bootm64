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
	usart_putstr("\n\r");
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
	uint32_t time = 0;
	// Wait for buffer to be full
	while(!(UCSR0A & (1<<RXC0)));
	// Return character
	return UDR0;
}

char usart_getchar_echo(void)
{
	// Wait for buffer to be full
	while(!(UCSR0A & (1<<RXC0)));
	// Get character
	char c = UDR0;
	// Echo character
	usart_putchar(XOFF);
	usart_putchar(c);
	usart_putchar(XON);
	// Return character
	return c;
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

//Sends a number over USART
void usart_putint(uint16_t sn)
{
	char cnum[16];
	itoa(sn,cnum,10);
	usart_putstr(cnum);
}

void usart_gethexline(char* buffer, int16_t bufPtr) {
	// If the first character isn't a ':' then we're done
	if(usart_getchar() != ':') {
		return;
	
	} else {
		// Set up the various chunks of data that we're getting from the line
		char hexSize[2];
		char byteAddr[4];
		char recordType[2];
		char data[32];
		char checksum[2];
		char nlcr[2];

		// Get the data size, byte address, and record type
		usart_getstr(hexSize,2);
		usart_getstr(byteAddr,4);
		usart_getstr(recordType,2);

		// if the record type is 1 then this is the end of the file
		if ((recordType[0] == '0') && (recordType[1] == '1')) {
			void (*start)(void);
			start = 0x0000;
			(*start)();
		}

		// Get the data and the checksum
		usart_getstr(data,32);
		usart_getstr(checksum,2);

		// Get the newline and carriage return
		usart_getstr(nlcr,2);

		// write the data from the line into the page buffer
		uint8_t i = 0;
		while(i<16)
			buffer[bufPtr + i++] = (char) hexstr2int(data[2*i],data[(2*i)+1]);
	}
}
		
