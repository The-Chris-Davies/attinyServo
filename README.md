AttinyServo
###########

A servo controller for the attiny85. 

Uses timer1 to schedule events every 20ms, and uses timer0 to generate a one-shot pulse of the desired length. (described [here](https://wp.josh.com/2015/03/12/avr-timer-based-one-shot-explained/)). Position is currently set from an analog voltage on PB3.

This project was just a quick one to gain some experience with avr and to mess around with lower-level timers and such.

The makefile is configured to compile using avr-gcc and to flash the code to the attiny using a USBasp.
