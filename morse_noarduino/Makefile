#
# Makefile template for ATtiny45
#
# Taken from: http://www.instructables.com/id/Honey-I-Shrunk-the-Arduino-Moving-from-Arduino-t/
#
# Derived from AVR Crosspack template
#
#

DEVICE     = attiny85           # See avr-help for all possible devices
CLOCK      = 16000000           # 16Mhz
PROGRAMMER = -c usbtiny -P usb  # For using Adafruit USBtiny
OBJECTS    = morse.o             # Add more objects for each .c file here
# FUSES      = -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m  # settings as taken from http://www.engbedded.com/fusecalc/

# AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -C ~/Downloads/avrdude.conf
AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all: morse.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:morse.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID morse.hex

clean:
	rm -f morse.hex morse.elf $(OBJECTS)

# file targets:
morse.elf: $(OBJECTS)
	$(COMPILE) -o morse.elf $(OBJECTS)

morse.hex: morse.elf
	rm -f morse.hex
	avr-objcopy -j .text -j .data -O ihex morse.elf morse.hex
	avr-size --format=Berkeley morse.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm: morse.elf
	avr-objdump -d morse.elf

cpp:
	$(COMPILE) -E morse.c
