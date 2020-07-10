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
#include <avr/interrupt.h>
#include <util/delay.h>

#define MAXVAL (256+64)
#define MINVAL (64)
#define MIDVAL (MINVAL + (MAXVAL-MINVAL)/2)

uint8_t pos;

int main() {
	pos = 0;
	//set the pin the servo is connected to as an output
	DDRB |= (1 << PB1);
	//configure timer 1 to trigger at 50Hz ~= 1MHz / (512*39)
	//configure timer prescaler to /512 and reset on match
	TCCR1 = (1 << CTC1) | (1 << CS13) | (1 << CS11);
	//set timer compare A to 39
	OCR1A = 39;
	//enable timer compare A interrupt
	TIMSK = (1 << OCIE1A);

	//setup ADC
	//set 8-bit mode with input on ADC3
	ADMUX = (1 << ADLAR) | (1 << MUX1) | (1 << MUX0);
	//enable adc and set prescaler to /32
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0);
	//disable digital input on PB3
	DIDR0 = (1 << ADC3D);


	//configure timer 0 to trigger one-shot at 2ms (500Hz) ~= 1MHz / (8*250)
	//configure output to OC0B in inverting mode and set to fast pwm mode
	TCCR0A = (1 << COM0B1) | (1 << COM0B0) | (1 << WGM01) | (1 << WGM00);
	//configure fast pwm mode and set prescaler to /64
	TCCR0B = (1 << WGM02) | (1 << CS01);
	//set TOP to 0
	OCR0A = 0;
	//set MATCH to 2
	OCR0B = 2;


	//enable interrupts
	sei();
	//main loop
	while(1){
		//start adc read
		ADCSRA |= (1 << ADSC);
		//wait until done
		while(ADCSRA & (1 << ADSC));
		pos = ADCH;
		OCR0B = pos;
	}	
	
	return 0;
}

//timer 1 compare A interrupt
ISR (TIMER1_COMPA_vect) {
	//trigger one-shot in timer0 (set to full)
	TCNT0 = OCR0B-1;
}
