#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

void go(uint8_t speed, uint8_t direction, uint8_t duration)
{

    duration = (int)duration;

    DDRB = 0x00;
    DDRD = 0xFF;

    uint8_t counter = 0;
    uint8_t duty_cycle = (100 - (speed * 33));

    for (int i = 0; i < (duration * 100000); i++)
    {
        counter++;

        //main way to varry speed on the motors is using PWM

        if (counter >= 100)
            counter = 0;
        if (counter < duty_cycle)
        {
            switch (direction)
            {
            case 0:
                PORTD |= (1 << 6); //turns on forward motion for the left motor
                PORTB |= (1 << 3);
                break;
            case 1:
                PORTD |= (1 << 5);
                PORTD |= (1 << 3);
            case 2:
                PORTD |= (1 << 6);
                PORTD |= (1 << 3);
            case 3:
                PORTD |= (1 << 5);
                PORTB |= (1 << 3);

            default:
                break;
            }
        }
        else
        {
            PORTD = PORTD & ~(1 << 6); //turns off forward motion for the left motor
            PORTB = PORTB & ~(1 << 3); //turns off forward motion for the right motor
            PORTD = PORTD & ~(1 << 5); //this would turn off reverse motion on the left motor
            PORTD = PORTD & ~(1 << 3); //this would turn off reverse motion ont the right motor
        }

        //Using pulsers for the buttons, sets the duty_cycle to the value and then allows the
        //motor to run at that speed. Higher number means wheels spin faster
    }
    PORTD = PORTD & ~(1 << 6); //turns off forward motion for the left motor
    PORTB = PORTB & ~(1 << 3);
    PORTD = PORTD & ~(1 << 5); //this would turn off reverse motion on the left motor
    PORTD = PORTD & ~(1 << 3);
}
