#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

#define MAX 100

void go(uint8_t speed, uint8_t direction, uint8_t duration)
{

	speed = (int)speed;
	direction = (int)direction;
	duration = (int)duration;

	DDRB = 0x00;
	DDRD = 0xFF;

	unsigned int counter = 0;
	unsigned int duty = 100 - ((2 - speed) * 33);
	if (speed == 2)
		duty = 90;

	//_delay_ms(50000);
	int k;
	for (k = 0; k < (duration * 1000); k += 1)
	{
		int p;
		for (p = 0; p < 365; p++)
		{
			counter++;

			if (direction == 0)
			{
				if (counter >= MAX)
					counter = 0;
				if (1 < counter && counter < (duty))
				{
					PORTD = PORTD | (1 << 6); //turns on forward motion for the left motor
					if (counter % 2 == 0)
						PORTB = PORTB | (1 << 3); //turns on forward motion for the right motor
				}
				else if (counter < (duty + (speed * 7 + 3)))
					PORTD = PORTD | (1 << 6); //turns on forward motion for the left motor
				else
				{
					PORTB = PORTB & ~(1 << 3); //turns off forward motion for the right motor
					PORTD = PORTD & ~(1 << 6); //turns off forward motion for the left motor
				}
			}

			else if (direction == 1)
			{
				if (counter >= MAX)
					counter = 0;
				if (1 < counter && counter < duty)
				{
					if (counter % 2 == 0)
						PORTD = PORTD | (1 << 5); //this would turn on reverse motion on the left motor

					PORTD = PORTD | (1 << 3); //this would turn on reverse motion ont the right motor
				}
				else if (counter < (duty + (speed * 9 + 1)))
					PORTD = PORTD | (1 << 5); //this would turn on reverse motion on the left motor
				else
				{
					PORTD = PORTD & ~(1 << 5); //this would turn off reverse motion on the left motor
					PORTD = PORTD & ~(1 << 3); //this would turn off reverse motion ont the right motor
				}
			}
			else if (direction == 2)
			{
				if (counter >= MAX)
					counter = 0;
				if (counter < duty)
				{
					PORTD = PORTD | (1 << 6); //this would turn on reverse motion on the left motor
					PORTD = PORTD | (1 << 3); //this would turn on reverse motion ont the right motor
				}

				else
				{
					PORTD = PORTD & ~(1 << 6); //this would turn off reverse motion on the left motor
					PORTD = PORTD & ~(1 << 3); //this would turn off reverse motion ont the right motor
				}
			}

			else if (direction == 3)
			{
				if (counter >= MAX)
					counter = 0;
				if (counter < duty)
				{
					PORTD = PORTD | (1 << 5); //this would turn on reverse motion on the left motor
					PORTB = PORTB | (1 << 3); //this would turn on reverse motion ont the right motor
				}
				else
				{
					PORTD = PORTD & ~(1 << 5); //this would turn off reverse motion on the left motor
					PORTB = PORTB & ~(1 << 3); //this would turn off reverse motion ont the right motor
				}
			}
			//_delay_us(100);
			PORTD = PORTD & ~(1 << 6); //turns off forward motion for the left motor
			PORTB = PORTB & ~(1 << 3); //turns off forward motion for the right motor
			PORTD = PORTD & ~(1 << 5); //this would turn off reverse motion on the left motor
			PORTD = PORTD & ~(1 << 3); //this would turn off reverse motion ont the right motor
		}
	}
	PORTD = PORTD & ~(1 << 6); //turns off forward motion for the left motor
	PORTB = PORTB & ~(1 << 3); //turns off forward motion for the right motor
	PORTD = PORTD & ~(1 << 5); //this would turn off reverse motion on the left motor
	PORTD = PORTD & ~(1 << 3); //this would turn off reverse motion ont the right motor
	return;
}
