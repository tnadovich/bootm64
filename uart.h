#ifndef _uart_h_
#define _uart_h_

#include "defines.h"
#include "convert.h"
#include <util/setbaud.h>
#include <stdlib.h>
#include <avr/io.h>

// Initialize  USART
void usart_init(void);

// Send one byte over USART
void usart_putchar(char c);

// Receive one byte over USART
char usart_getchar(void);

// Receive one byte over USART then echo it back
char usart_getchar_echo(void);

// Send a character array
void usart_putstr(char* c);

// Receive a character array
void usart_getstr(char* str,int size);

// Send an integer as a string
void usart_putint(uint16_t sn);

// Receive a line from an Intel hex file
uint8_t usart_gethexline(char* buffer, int16_t bufPtr);

#endif
