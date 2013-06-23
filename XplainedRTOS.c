/*
 * XplainedRTOS.c
 *
 * Created: 2013-05-25 1:14:28 PM
 *  Author: nrqm
 */ 

#include <util/delay.h>
#include <avr/io.h>
#include "os.h"


enum { A=1, B, C, D, E, F, G };
const unsigned char PPP[] = {A, 20, B, 20, IDLE, 40, C, 20, D, 20, IDLE, 40};
const unsigned int PT = sizeof(PPP)/2;

EVENT* e;

//const unsigned char PPP[] = {};
//const unsigned int PT = 0;
void periodic_task(void)
{
	//uint8_t i;
	uint8_t arg = 0;
	arg = Task_GetArg();
	uint8_t v = 0b10000000;
	
	if (arg == A) v = _BV(PB0);
	else if (arg == B) v = _BV(PB1);
	else if (arg == C) v = _BV(PB2);
	else if (arg == D) v = _BV(PB3);
	//Event_Wait(e);	// periodic events can't wait!
	for(;;)
	{
		PORTB &= ~v;
		if (arg == A) _delay_ms(25);
		else if (arg == B) _delay_ms(75);
		else if (arg == C) _delay_ms(25);
		else if (arg == D) _delay_ms(75);
		PORTB |= v;
		Task_Next();
	}
}

void rr_task()
{
	for (;;)
	{
		Task_Next();
	}
}

int r_main(void)
{
	DDRB = 0xFF;
	PORTB  = 0xFF;
	e = Event_Init();
	// Task_Create params: function, parameter, level, name
	Task_Create(periodic_task, A, PERIODIC, A);
	Task_Create(periodic_task, B, PERIODIC, B);
	Task_Create(periodic_task, C, PERIODIC, C);
	Task_Create(periodic_task, D, PERIODIC, D);
	Task_Create(rr_task, 0, RR, 50);
	return 0;
}