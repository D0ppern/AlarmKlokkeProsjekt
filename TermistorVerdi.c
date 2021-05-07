/*
 * termistorverdi.c
 *
 * Created: 05.05.2021 09:42:10
 *  Author: tove_
 */

#include <avr/io.h>
#include <math.h>
#define BIT_IS_SET(byte, bit) (byte & (1 << bit))
#define BIT_IS_CLEAR(byte, bit) (!(byte & (1 << bit)))

//-----builder i simulering pr 050521, sjekk med fysisk oppkobling-----
int termistorVerdi(){
//------------termistoravlesning-----------
//deklarere variabel for å lagre verdien til fsravlesning, satt til 0 for å unngå feil avlått alarm

int termistor = 0;


ADMUX |= (1<<REFS0)|(1<<ADLAR)|(1<<MUX1)|(1<<MUX0);	//Skrur på AREF og venstre lesing. ADC inngang 3.

uint16_t beta = 3950;

printString("Øk temperaturen på termistoren.\r")
do{
	ADCSRA |= (1<<ADSC);
	loop_until_bit_is_clear(ADCSRA,ADSC);

	uint8_t spenning = ADCH;
	uint16_t res = 10000 * (255/spenning - 1);

	uint16_t tKelvin = (beta*298.15)/(beta + (298.15*log(res/10000)));
	uint16_t tCelcius = tKelvin - 273.15;

	if (tCelcius > 30){
		termistor = 1;
	}
} while(termistor == 0); //sjekk hva som er fornuftig verdi ift 30 grader... (en plass mlm 0-255)
return termistor;
}
