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
CCARGS = -Wpedantic -Os

CLK = 1000000						# 1Mhz

OBJS = $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))				#object files are generated from src filenames

PROGRAMMER = -c usbasp -P usb		# For using usbasp
AVARGS = -B10
FUSES =  -U lfuse:w:0x62:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m	# settings as taken from http://www.engbedded.com/fusecalc/

COMPILE = $(CC) $(CCARGS) -mmcu=$(D)
AVRDUDE = avrdude $(PROGRAMMER) $(AVARGS) -p $(D)

# symbolic targets:
main.hex: $(OBJS)
	$(COMPILE) -o $@ $^

obj/%.o: src/%.c
	$(COMPILE) -c $< -o $@

flash: main.hex
	$(AVRDUDE) -U flash:w:main.hex

fuse:
	$(AVRDUDE) $(FUSES)

.PHONY: install
install: flash fuse

clean:
	rm -f main.hex $(OBJECTS)

