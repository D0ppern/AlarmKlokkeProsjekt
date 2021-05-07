/*
 * ProsjektForsøkTimer.c
 *
 * Created: 23.04.2021 00:08:06
 * Author : jojoh
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "USART.h"
#define USART_BAUDRATE 9600 // desired baud rate
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) // UBRR value
#include <stdlib.h>
#include <stdio.h>


//Setter knappene på pinnene PD2, PD4 og PB4 til a, b og c
#define a 2   //OK knapp   Her står det PIND2
#define b PINB4   //Øker antall
#define c 4   //Minker antall	Her står det PIND4


static volatile int SEC = 0;//allocating integer memory for storing seconds
static volatile int MIN = 0;// allocating integer memory for storing minutes
static volatile int HOU = 0;// allocating integer memory for storing hours

int ALSEC = 0;//alarm seconds storing memory
int ALMIN = 0;//alarm minutes storing memory
int ALHOU = 0;//alarm hours storing memory
int ALACTIVATE = 0;
int matte = 0;

//Deklarerer funksjoner
ISR(TIMER1_COMPA_vect);
void settingClock();
int settingAlarm();
int Numba();
void fsr();
void buzzerLyd();

int main(void)
{
	UBRR0 = BAUD_PRESCALE; // Set UBBR according to system clock and desired baud rate
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0); // Turn on the transmission and reception circuit

	PORTD |= (1<<PD3) | (1<<PD2) | (1<<PD4);  //Setter intern pull-up
	PORTB |= (1<<PB4);

	settingClock();	//Kaller funksjon som stiller inn tiden

	TCCR1B |=(1<<CS12)|(1<<CS10)|(1<<WGM12);	// setting prescale 1024 and CTC mode
	OCR1A = 15625;	//setting compare value equal to counter clock frequency to get an interrupt every second

	EIMSK |= (1<<INT1); // Enable INT1
	EICRA |= (1<<ISC01);	// EICRA sense control, negative flank


	PCICR |= (1<<PCIE0);
	PCMSK0 |= (1<<PCINT0);

	sei();	//enabling global interrupts

	TIMSK1 |=(1<<OCIE1A);	//compare match interrupt enable

    while (1)
    {
    }
}

ISR(INT1_vect){
	settingAlarm();
	_delay_ms(500);
}

ISR(PCINT0_vect){
	if ((ALHOU == HOU) && (ALMIN == MIN) && (ALACTIVATE == 1))
	{
		printString("Pinnen");		//Programmet går inn her, men må undersøke hvordan piezo lage lyd.
		int buzzer = 1;
		buzzerLyd(buzzer);
		fsr();
		ALACTIVATE = 0;

	}
	PORTB &= ~(1<<PINB0);
}

void settingClock(){
	printString("Still inn time:\r");	// \r for ny linje!!!
	printByte(HOU);
	printString("\r");

	while (bit_is_set(PIND,a))
	{
		if(bit_is_clear(PINB,b)){	//Øker time antallet med 1
			HOU += 1;

			if (HOU >= 24)
			{
				HOU = 0;
			}
			printByte(HOU);
			printString("\r");
			_delay_ms(500);
		}

		if(bit_is_clear(PIND,c)){	//Minker time antallet med 1
			HOU -= 1;

			if (HOU < 0)
			{
				HOU = 23;
			}
			printByte(HOU);
			printString("\r");
			_delay_ms(500);
		}
	}

	_delay_ms(500);
	printString("Still inn minutt:\r");
	printByte(MIN);
	printString("\r");

	while (bit_is_set(PIND,a))
	{
		if(bit_is_clear(PINB,b)){	//Øker minutt antallet med 1
			MIN += 1;

			if (MIN > 59)
			{
				MIN = 0;
			}
			printByte(MIN);
			printString("\r");
			_delay_ms(500);
		}

		if(bit_is_clear(PIND,c)){	//Minker minutt antallet med 1
			MIN -= 1;

			if (MIN < 0)
			{
				MIN = 59;
			}
			printByte(MIN);
			printString("\r");
			_delay_ms(250);
		}
	}
	printByte(HOU);
	printString(":");
	printByte(MIN);
	printString("\r");
}

ISR(TIMER1_COMPA_vect){
	SEC += 1;
	if (SEC >= 10)
	{
		SEC = 0;
		MIN += 1;


		if (MIN >= 60)
		{
			MIN = 0;
			HOU += 1;

			if (HOU >= 24)
			{
				HOU = 0;
			}
		}

		PORTB |= (1<<PORTB0); //Kaller interrupten som sjekker om alarmtid = klokketid

		printByte(HOU);
		printString(":");
		printByte(MIN);
		printString("\r");
	}
}

int settingAlarm(){
	printString("Still inn time ALARM:\r");	// \r for ny linje!!!
	printByte(ALHOU);
	printString("\r");

	while (bit_is_set(PIND,a))
	{
		if(bit_is_clear(PINB,b)){	//Øker time antallet med 1
			ALHOU += 1;

			if (ALHOU >= 24)
			{
				ALHOU = 0;
			}
			printByte(ALHOU);
			printString("\r");
			_delay_ms(250);
		}

		if(bit_is_clear(PIND,c)){	//Minker time antallet med 1
			ALHOU -= 1;

			if (ALHOU < 0)
			{
				ALHOU = 23;
			}
			printByte(ALHOU);
			printString("\r");
			_delay_ms(250);
		}
	}

	_delay_ms(250);
	printString("Still inn minutt:\r");
	printByte(ALMIN);
	printString("\r");

	while (bit_is_set(PIND,a))
	{
		if(bit_is_clear(PINB,b)){	//Øker minutt antallet med 1
			ALMIN += 1;

			if (ALMIN > 59)
			{
				ALMIN = 0;
			}
			printByte(ALMIN);
			printString("\r");
			_delay_ms(250);
		}

		if(bit_is_clear(PIND,c)){	//Minker minutt antallet med 1
			ALMIN -= 1;

			if (ALMIN < 0)
			{
				ALMIN = 59;
			}
			printByte(ALMIN);
			printString("\r");
			_delay_ms(250);
		}
	}
	printString("Alarm is set");
	printString("\r");
	ALACTIVATE = 1;
	return ALACTIVATE;	//Vet ikke om den er nødvendig
}

int Numba(){
	int i = 120;
	int d = 3;
	srand(SEC);		//Genererer et "random number seed" basert på hva tiden nå er

	int num1 = (rand() % (i - 0 + 1) + 0);		//Prøver å skape tilfeldige tall, to froskjellige måter å gjøre det på. (rand() % (øvregrense - nedregrense + 1) + nedregrense)
	int num2 = (rand() % (i - 0 + 1) + 0);
	int correctAns = num1 + num2;
	int wrongAns1 = (rand() % ((correctAns + 5) - (correctAns - 5) + 1) + (correctAns - 5));
	int wrongAns2 = (rand() % ((correctAns + 5) - (correctAns - 5) + 1) + (correctAns - 5));
	int multiplechoice = (rand() % (d - 1 + 1) + 1);

	printString("Hva er summen av disse tallene?\r");
	printByte(num1);
	printString(" + ");
	printByte(num2);
	printString("\r");

	if(wrongAns1 == correctAns)
	{
		wrongAns1 += 1;
	}
	if (wrongAns2 == correctAns)
	{
		wrongAns2 += 1;
	}

	if (multiplechoice == 1)
	{
		printString("	a: ");
		printByte(correctAns);
		printString("	b: ");
		printByte(wrongAns1);
		printString("	c: ");
		printByte(wrongAns2);
		printString("\r");
	}
	if (multiplechoice == 2)
	{
		printString("	a: ");
		printByte(wrongAns1);
		printString("	b: ");
		printByte(correctAns);
		printString("	c: ");
		printByte(wrongAns2);
		printString("\r");
	}
	if (multiplechoice == 3)
	{
		printString("	a: ");
		printByte(wrongAns2);
		printString("	b: ");
		printByte(wrongAns1);
		printString("	c: ");
		printByte(correctAns);
		printString("\r");
	}

	while (matte == 0)
	{
		if ((bit_is_clear(PIND,a)) && (multiplechoice == 1))
		{
			printString("Riktig!\r");
			matte = 1;
			return matte;
		}
		if (((bit_is_clear(PINB,b)) | (bit_is_clear(PIND,c))) && (multiplechoice == 1))
		{
			printString("Feil svar, prøv igjen\r");
		}

		if ((bit_is_clear(PINB,b)) && (multiplechoice == 2))
		{
			printString("Riktig!\r");
			matte = 1;
			return matte;
		}
		if (((bit_is_clear(PIND,a)) | (bit_is_clear(PIND,c))) && (multiplechoice == 2))
		{
			printString("Feil svar, prøv igjen\r");
		}

		if ((bit_is_clear(PIND,c)) && (multiplechoice == 3))
		{
			printString("Riktig!\r");
			matte = 1;
			return matte;
		}
		if(((bit_is_clear(PINB,b)) | (bit_is_clear(PIND,a))) && (multiplechoice == 3))
		{
			printString("Feil svar, prøv igjen\r");
		}
	}
}
