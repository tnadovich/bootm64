Bootloader for KFLARE NIR source
Boston University Electronic Design Facility
Thomas Nadovich

This is a bootloader for the AVR microcontrollers on the KFLARE NIR
source. When complete, it will be able to flash the program memory of
the system controller (ATMEGA64) as well as an arbitrary number of laser
controller chips (ATMEGA164A). The user will interface with the system
by copying the Intel hex files to be loaded into the USB serial
interface connected to the system controller.

 TO DO 

 ATMEGA64 Bootloader....................[done]
 PC to board USART communication........[    ]
 Interrupt driven timeout to boot.......[    ]
 ATMEGA164A Bootloader..................[    ]
 Modbus communication...................[    ]
