#ifndef _boot_h_
#define _boot_h_

#include "defines.h"
#include <inttypes.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>

void boot_program_page(uint32_t page, uint8_t *buf);

#endif
