#
# Makefile for AVR attiny
#Compiled with avr-gcc
#programmed with AVRdude over usbasp
#
#example commands:
#avr-gcc fadey.c -Wall -Os -o main.hex -mmcu=attiny85
#avrdude -c usbasp -B10 -p attiny85 -U flash:w:main.hex

D = attiny85					# See avr-help for all possible Ds
CC = avr-gcc
CCARGS = -Wall -Os

CLK = 1000000						# 1Mhz

OBJS = $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))				#object files are generated from src filenames

PROGRAMMER = -c usbasp -P usb		# For using usbasp
AVARGS = -B10
FUSES = -U lfuse:w:0xe2:m -U hfuse:w:0xde:m -U efuse:w:0xff:m			# settings as taken from http://www.engbedded.com/fusecalc/

COMPILE = $(CC) $(CCARGS) -mmcu=$(D)
AVRDUDE = avrdude $(AVARGS) -p $(D)

# symbolic targets:
all: main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash: all
	$(AVRDUDE) -U flash:w:main.hex

fuse:
	$(AVRDUDE) $(FUSES)

install: flash fuse

clean:
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	avr-size --format=avr --mcu=$(D) main.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm: main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c
