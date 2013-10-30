# To change the part number: change MCU, change which BOOTSTART is
# called in LFLAGS, and change the fuse setting in fuse:

PROJECT		= boot

MCU		= atmega64
M328P_BOOTSTART	= 0x7000
M64_BOOTSTART	= 0xF800

# Fuse settings
M328P_FUSE	= -U lfuse:w:0xEF:m -U hfuse:w:0xD8:m
M64_FUSE	= -U lfuse:w:0xC1:m -U hfuse:w:0x98:m -U efuse:w:0xFF:m
ARDUINO_FUSE	= -U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0x05:m

LFLAGS		= -Wl,--section-start=.text=$(M64_BOOTSTART)
DUDEPORT	= -c avrisp2 -P usb

AVRDUDE		= sudo avrdude $(DUDEPORT) -p $(MCU) -y -u
OBJS		= main.o uart.o boot.o convert.o

all: $(PROJECT).hex

%.o: %.c
	avr-gcc -g -Os -mmcu=$(MCU) -c $<

$(PROJECT).elf: $(OBJS)
	avr-gcc -g $(LFLAGS) -mmcu=$(MCU) -o $(PROJECT).elf $(OBJS)

%.elf: %.o
	avr-gcc -g -mmcu=$(MCU) -o blink.elf blink.o

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@
	mv $@ hexfiles

flash: $(PROJECT).hex
	$(AVRDUDE) -U flash:w:$<:i

blink: new_blink.hex
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

rfuse:
	$(AVRDUDE) -U lfuse:r:lf.hex:h -U hfuse:r:hf.hex:h -U efuse:r:ef.hex:h

clean:
	rm -f *.lst *.map *.elf *.o boot.hex blink.hex
