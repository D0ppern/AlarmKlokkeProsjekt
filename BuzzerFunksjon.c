/*
 * buzzerAlarmLyd_v01.c
 *
 * Created: 06.05.2021 19:25:29
 *  Author: tove_
 */



#ifndef F_CPU					// if F_CPU was not defined in Project -> Properties
#define F_CPU 1600000UL			// define it now as 1 MHz unsigned long
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

enum t0_prescaler
{
	T0_PRESCALER_1 = _BV(CS00),
	T0_PRESCALER_8 = _BV(CS01),
	T0_PRESCALER_64 = _BV(CS00) | _BV(CS01),
	T0_PRESCALER_256 = _BV(CS02),
	T0_PRESCALER_1024 = _BV(CS02) | _BV(CS00),
};
static void t0_set_prescaler(enum t0_prescaler ps)
{
	TCCR0B = ps;
}
static unsigned short t0_get_prescaler_rate(enum t0_prescaler ps)
{
	unsigned short rate;
	switch(ps)
	{
		case T0_PRESCALER_1:
		rate = 1;
		break;
		case T0_PRESCALER_8:
		rate = 8;
		break;
		case T0_PRESCALER_64:
		rate = 64;
		break;
		case T0_PRESCALER_256:
		rate = 256;
		break;
		case T0_PRESCALER_1024:
		rate = 1024;
		break;
		default:
		rate = 0;
		break;
	}
	return rate;
}
static unsigned long div_round(unsigned long d, unsigned long q)
{
	return (d + (q/2)) / q;
}
static void t0_set_ctc_a(unsigned long hz, unsigned long timer_freq)
{
	OCR0A = div_round(timer_freq, hz*2) - 1;
	TCCR0A =
	_BV(COM0A0) // toggle
	| _BV(WGM01); // CTC
}

void buzzerLyd(int buzzer){



unsigned long timer_freq;
enum t0_prescaler ps = T0_PRESCALER_1024;	//ndret fra 256 for å sjekke om lyd veksler mer slow

DDRD |= _BV(DDD6);
t0_set_prescaler(ps);
timer_freq = div_round(F_CPU, t0_get_prescaler_rate(ps));



if(buzzer == 1)
{
	t0_set_ctc_a(200, timer_freq);


}

//Skrur av alarm
else{

	TCCR0A &= ~(1<<COM0A0);
	PORTD &= ~(1<<PIND6);
}
}
