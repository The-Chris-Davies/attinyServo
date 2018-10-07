AttinyServo
###########

A servo controller for the attiny. Currently uses the 16-bit timer on the attiny44.

This project is currently a big WIP: probably nothing will work 
correctly, and it will be finnicky. This is mainly a foray into avr w/out arduino.

to compile using avrdude, use
avr-gcc gimbal.c -Wall -Os -o main.hex -mmcu=attiny44
avrdude -c usbasp -B10 -p attiny44 -U flash:w:main.hex