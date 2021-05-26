#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>


void forward(uint8_t speed){
    go(1, 0,2);
}

void forwardshort(){
    go(0,0,5);
}

void backshort(){
    go(0,0,3);
}

// When it sees a wall it moves backwards and then spins around 

void wall(){
    //LCD_execute_command(CLEAR_DISPLAY);
    //LCD_print_String("WALL");
    go(1,1,30);
    right180();

}

// When it sees a bunker it does a little spinny dance and waits 

void bunker(){
    //LCD_execute_command(CLEAR_DISPLAY);
    //LCD_print_String("BUNKER");
    go(1,1,10);
    right180();
    _delay_ms(100);
    right180();
    _delay_ms(5000);
}

// Some of these are used some of them aren't and I am too scared to remove any unsused ones because it might break everything

// right turns
void rightshort(){
    go(1,2,0x7);
}

void leftshort(){
    go(1,3,0x7);
}
void right30(){
    go(1,2,0xC);
}

void right180(){
    go(2,2,0x1B);
}

void right45(){
    go(1,2,0xF);
}

void right90(){
    go(2,2,0x11);
}

// left turns
void left30(){
    go(1,3,0xC);
}

void left180(){
    go(2,3,0x1B);
}

void left45(){
    go(1,3,0xF);
}

void left90(){
    go(2,3,0x11);
}

// This "go" function that actually controls everything is completely rewritten from milestone 1 because my partner did it and I did not approve.
// Also it goes straight enough for the 40x32 inch field but any attempt for me to correct curving is futile because my floor has a slope to it 
// which means the robot will curve to the left when it goes north-south on my floor, and to the right when it goes south-north. I tried to level
// the whiteboard which the robot rides on but its just impossible especially with the inconsistency of the motor speed as the battery drains. oh well


void go(uint8_t speed, uint8_t direction, uint8_t duration)
{

    DDRB = 0x00;
    DDRD = 0xFF;

    // Loop
    for (int k = 0; k < (duration * 50); k++)
    {

        // Motor 1 Control Switch
        if ((k % (8 - (2 * speed))) == 0)
        {
            motor_1(direction);
            motor_2(direction);
        }
        else
        {
            // M1 - Coast
            PORTD &= ~(3 << 5);

            // M2 - Coast
            PORTD &= ~(1 << 3);
            PORTB &= ~(1 << 3);
        }
        // Motor Off Time

        _delay_us(250);
    }

        // Stop
        if ((direction==2) | (direction==3)){
        PORTD |= 104;
        PORTB |= 8;
        }

}

void motor_1(uint8_t direction)
{
    switch (direction)
    {
    case 0: // Forwards

        // M1 - Forward
        PORTD &= ~(1 << 5);
        PORTD |= (1 << 6);

        break;
    case 1: // Backwards

        // M1 - Backwards
        PORTD |= (1 << 5);
        PORTD &= ~(1 << 6);

        break;
    case 2: // Clockwise

        // M1 - Forward
        PORTD &= ~(1 << 5);
        PORTD |= (1 << 6);

        break;
    case 3: // Counter-clockwise

        // M1 - Backwards
        PORTD |= (1 << 5);
        PORTD &= ~(1 << 6);

        break;
    }
}

void motor_2(uint8_t direction)
{
    switch (direction)
    {
    case 0:
        // M2 - Forward
        PORTD &= ~(1 << 3);
        PORTB |= (1 << 3);
        break;
    case 1:
        // M2 - Backwards
        PORTD |= (1 << 3);
        PORTB &= ~(1 << 3);
        break;
    case 2:
        // M2 - Backwards
        PORTD |= (1 << 3);
        PORTB &= ~(1 << 3);
        break;

    case 3:
        // M2 - Forward
        PORTD &= ~(1 << 3);
        PORTB |= (1 << 3);
        break;
    }
}