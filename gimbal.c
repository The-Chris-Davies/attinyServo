/*
controls a servo connected to pin 0 (physical pin 5)

Servos require a 1-2ms pulse with a 20ms interval between the start of each pulse
dead zone of servo = 5us -> ~200 steps in 1 ms 
timer settings:
	max. value of 5120 << 2
	comp val = (250 + target) << 2
*/

#define F_CPU 1000000UL		//1MHz system clock (prescaled /8)

#include <avr/io.h>
#include <util/delay.h>

#define MAXVAL (256+64)
#define MINVAL (64)
#define MIDVAL (MINVAL + (MAXVAL-MINVAL)/2)

static inline void long_delay(uint16_t delay){
	//using for loop so attiny 8bit timer doesn't overflow
	for(delay /= 10; delay>0; delay--) _delay_ms(10);
}

int main() {
	int16_t dCycle1 = 0, dCycle2 = 0;
	int16_t dir1 = (MAXVAL - MINVAL)/10;
	int16_t dir2 = -(MAXVAL - MINVAL)/10;
	
	//set the pin the LED is connected to as an output
	DDRA |= (1 << PA5) | (1 << PA6);
	
	//timer config
	//COM1A1 / COM1B1: set high on BOTTOM, clear on MATCH on COM1A/B
	//WGM13 / WGM12 / WGM11 / ~WGM10: use fast PWM mode, with ICR1 as TOP
	//CS11: x8 prescaler
	TCCR1A = 0b10100010;
	TCCR1B = 0b00011010;
	//TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	//TCCR1B = (1 << WGM12)| (1 << WGM13) | (1 << CS11);
	
	//set TOP
	ICR1 = (128*5);
	

	//main loop
	while(1){
		OCR1A = dCycle1;
		dCycle1 += dir1;
		if(dCycle1 >= MAXVAL || dCycle1 <= MINVAL){
			dCycle1 = MIDVAL;
			dir1 = -dir1;
		}

		OCR1B = dCycle2;
		dCycle2 += dir2;
		if(dCycle2 >= MAXVAL || dCycle2 <= MINVAL){
			dCycle2 = MIDVAL;
			dir2 = -dir2;
		}
		long_delay(500);
	}
	return 0;
}
