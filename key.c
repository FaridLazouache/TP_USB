/** Main file for USB storage key **/

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "io.h"
#include "AT45DB641E.h"
#include "spi.h"
#include "./lufa-LUFA-210130/PolytechLille/lib"

// Constants

#define MAX_LED	3

#define PORTB_OUTPUT	0xD0 // PB7
#define PORTC_OUTPUT	0x60 // PC5, PC6
#define PORTD_OUTPUT	0x00

unsigned char led1[]={0x80,0x00,0x00};// PB7
unsigned char led2[]={0x00,0x40,0x00};// PC6
unsigned char led3[]={0x00,0x20,0x00};// PC5

unsigned char *leds[]={led1, led2, led3};// on définit les leds

int main(void)
{
unsigned char omasks[]={PORTB_OUTPUT,PORTC_OUTPUT,PORTD_OUTPUT};
outputs_init(omasks);
int i;
/*
while(1)
{
  for(i=0;i<MAX_LED;i++){ output_set(leds,i); _delay_ms(100); }
  for(i=0;i<MAX_LED;i++){ output_unset(leds,i); _delay_ms(100); }
}
*/

spi_init(); // init la lecture sur le bus spi, pour la memoire
PORTB |= 0x50; //on desactive les CS de base
unsigned char result[3]; // On choisit les 3 premiers octets, ce dont on a besoin
AT45DB641E_cmd(&PORTB, 4, 0x9f, result, 3); // on se base sur struct definis dans AT45DB641E.h, sur le PB4

if(result[0] == 0x1F && result[1] == 0x28 && result[2] == 0x00) // si on a bien ce que l'on veut on allume
{
	output_set(leds, 0); // on allume la led 1.
	_delay_ms(100);
}

AT45DB641E_cmd(&PORTB, 6, 0x9f, result, 3); // sur le PB6

if(result[0] == 0x1F && result[1] == 0x28 && result[2] == 0x00) // on allume si on a acces a la memoire, on arrive a lire la memoire.
{
	output_set(leds, 1);
	_delay_ms(100);
}
output_set(leds, 2);
_delay_ms(100);
	

return 0;
}
