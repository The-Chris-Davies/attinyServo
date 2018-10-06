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

#define LED_PIN PB0

inline void long_delay(uint16_t delay){
	//using for loop so attiny 8bit timer doesn't overflow
	for(delay /= 10; delay>0; delay--) _delay_ms(10);
}

int main() {
	int8_t dCycle = 0;
	int8_t dir = 1;
	
	//set the pin the LED is connected to as an output
	DDRA |= (1 << PA5) | (1 << PA6);
	
	//timer config
	//COM1A1 / COM1B1: set high on BOTTOM, clear on MATCH on COM1A/B
	//WGM13 / WGM12 / WGM11 / ~WGM10: use fast PWM mode, with ICR1 as TOP
	//CS10: no prescaler used
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM12)| (1 << WGM13) | (1 << CS10);
	//set TOP
	ICR1 = (256*20) << 2;
	
	
	//main loop
	while(1){
		OCR1A = (250 + (dCycle/100.0) * 255.0) << 2;
		dCycle += dir;
		if(dCycle >= 100 || dCycle <= 0) dir = !dir;
		
		_delay_ms(100);
	}
	return 0;
}
