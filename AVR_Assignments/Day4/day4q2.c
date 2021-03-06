// ***********************************************************
// Project:
// Author:
// Module description:
// ***********************************************************
/*Write a code to toggle LED's on PORTB at the rate of 1 sec when 1 is pressed from
the keyboard and change the rate to 2 sec when 2 is pressed.*/
#include <avr\io.h>              // Most basic include files
#include <avr\interrupt.h>       // Add the necessary ones
#include <avr\signal.h>          // here

// Define here the global static variables
//
int My_global;
int flag = 0;
// Interrupt handler example for INT0
//
ISR(  INT0_vect )
{
   TCNT2 = 0;
	OCR2 = 122;
}
ISR(INT1_vect)
{
   TCNT2 = 0;
	OCR2 = 244;
}
ISR(   TIMER2_COMP_vect )
{
    	PORTB = !PORTB;

}

// It is recommended to use this coding style to
// follow better the mixed C-assembly code in the
// Program Memory window
//


// ***********************************************************
// Main program
//
int main(void) {
   DDRD = 0;
   PORTD |= (1<<PD2)|(1<<PD3);
	DDRB = 1;
    TCNT2 = 0;
    OCR2 = 0;
    TIMSK = (1<<OCIE2);
	TCCR2 = (1<<CS21)|(1<< WGM21)|(1<<CS20);
	GICR = (1<<INT0)|(1<<INT1);
	MCUCR  |= (1 << ISC11)|(1<<ISC01)|(1<<SE);
	sei();
   while(1)
   {
	     asm("SLEEP");

	}

}




