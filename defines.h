#ifndef _defines_h_
#define _defines_h_

// CPU Frequency
#define F_CPU 1000000UL

// UART baud rate
#define BAUD  9600

// Byte size of flash page
#define SPM_PAGESIZE 256

// Byte adress of first byte of bootloader
#define M64_BOOTSTART 0xF800

// Length of time to wait before jumping to the main program
#define TIMEOUT_SECONDS 2

// Flow Control characters
#define XON 17
#define XOFF 19

#endif
