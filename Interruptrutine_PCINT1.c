#include <avr/io.h>		// this is always included in AVR programs
#include <util/delay.h>
#include <avr/interrupt.h>

int Numba();
int termistorVerdi();

ISR (PCINT1_vect){
	int termistor = 0;	// måtte være med her selv om satt til extern i main... = 0 for å ikke slå av alarmen tilfeldig
	int matte = 0;		// måtte være med her selv om satt til extern i main... = 0 for å ikke slå av alarmen tilfeldig

	matte = Numba();

	termistor = termistorVerdi();

	//hvis verdiene er riktige og 1 returneres fra begge funksjonene
	//sett LOW på buzzerpin
	if ((matte ==1) & (termistor == 1)){

		int buzzer = 0;
		buzzerLyd(buzzer);
	}
}
