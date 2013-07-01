# To change the part number: change MCU, change which BOOTSTART is
# called in LFLAGS, and change the fuse setting in fuse:

MCU	= atmega64
M328P_BOOTSTART = 0x7000
M64_BOOTSTART = 0xF800

# Fuse settings
M328P_FUSE = -U hfuse:w:0xD8:m -U lfuse:w:0xEF:m
M64_FUSE = -U hfuse:w:9C:m -U lfuse:w:0xC1:m
ARDUINO_FUSE = -U hfuse:w:DE:m -U lfuse:w:0xFF:m -U efuse:w:05:m

LFLAGS = -Wl,--section-start=.text=$(M64_BOOTSTART)
PROJECT	= boot
DUDEPORT = -c avrisp2 -P usb

AVRDUDE	= sudo avrdude $(DUDEPORT) -p $(MCU) -y -u
OBJS	= main.o uart.o

all: $(PROJECT).hex

%.o: %.c
	avr-gcc -g -Os -mmcu=$(MCU) -c $<

$(PROJECT).elf: $(OBJS)
	avr-gcc -g $(LFLAGS) -mmcu=$(MCU) -o $(PROJECT).elf $(OBJS)

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

flash: $(PROJECT).hex
	$(AVRDUDE) -U flash:w:$<:i

fuse: 
	$(AVRDUDE) $(M64_FUSE)

%.lst: %.elf
	avr-objdump -h -S $< > $@
	avr-size --common -d $<

wipe:
	$(AVRDUDE) -e

read:
	$(AVRDUDE) -U flash:r:read.hex:i
	less read.hex

clean:
	rm -f *.lst *.map *.elf *.o *.hex
