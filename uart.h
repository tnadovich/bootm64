#include "defines.h"
#include <util/setbaud.h>
#include <avr/io.h>

//Initialize  USART
void usart_init(void);

//Send one byte over USART
void usart_putchar(char c);

//Receive one byte over USART
char usart_getchar(void);
