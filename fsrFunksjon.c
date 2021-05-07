/*
 * FSR_callerInterrupt.c
 *
 * Created: 04.05.2021 22:30:50
 *  Author: tove_
 */

#include <avr/io.h>		// this is always included in AVR programs
#include <util/delay.h>
#ifndef F_CPU					// if F_CPU was not defined in Project -> Properties
#define F_CPU 1600000UL			// define it now as 1 MHz unsigned long
#endif
#include <avr/interrupt.h>

void fsr(){

	DDRD |= (1 << DDD7);	// pin satt til utgang
	PORTD |= (1 << PORTD7);	// Skru på LED til

	ADMUX |= (1<<REFS0)|(1<<ADLAR);		//LEFT-adjust og AVCC with external capacitor at AREF pin
	ADCSRA |= (1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);  //ADC enable og rpescale


	PCICR |= (1<<PCIE1);
	PCMSK1 |= (1<<PCINT9);

	int fsrVerdi;
	do{
		ADCSRA |= (1 << ADSC);    // Start the ADC conversion
		loop_until_bit_is_clear(ADCSRA, ADSC);          /* wait until done */
		fsrVerdi = ADCH;                                     /* read ADC in */
	} while(fsrVerdi > 28);

	PORTD &= ~(1<<PORTD7);
// 	printString("Venter 4 sekunder\r");		//Plan var å la programmet vente i 4 sekunder før den går videre.
// 	_delay_ms(4000);

	PORTC |= (1<<PC1);

}
