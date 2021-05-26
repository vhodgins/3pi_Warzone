#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include "lcd_driver.h"

uint8_t speed_pick()
{
    LCD_print_String("Pick");
    LCD_move_cursor_to_col_row(0, 1);
    LCD_print_String("Speed");
    _delay_ms(1000);
    LCD_execute_command(CLEAR_DISPLAY);
    LCD_print_String("Slow");
    DDRB = 0x00;

    uint8_t speed_value = 126, last_PINB = PINB;

    while (1)
    {

        //LCD_execute_command(CLEAR_DISPLAY);
        //LCD_print_hex8(PINB & (1<<4));
        _delay_ms(50);
        // Left button check
        if (PINB & 2)
        {
            speed_value--;
            choose_speed(speed_value);
        }

        // Right button check
        if (PINB & 32)
        {
            speed_value++;
            choose_speed(speed_value);
        }

        // Middle button check
        if (!(PINB & 16))
        {
            LCD_execute_command(CLEAR_DISPLAY);
            return (speed_value%3)+1;
        }
        

        last_PINB = PINB;
    }
}

void choose_speed(uint8_t speed_value)
{
    LCD_execute_command(CLEAR_DISPLAY);
    switch (speed_value % 3)
    {
    case 0:
        LCD_print_String("Slow");
        break;
    case 1:
        LCD_print_String("Med");
        break;
    case 2:
        LCD_print_String("Fast");
        break;
    default:
        break;
    }
}




uint8_t bunker_pick()
{
    LCD_print_String("Pick");
    LCD_move_cursor_to_col_row(0, 1);
    LCD_print_String("Bunkers");
    _delay_ms(1000);
    LCD_execute_command(CLEAR_DISPLAY);
    LCD_print_hex4(1);
    DDRB = 0x00;

    uint8_t bunker_value = 0, last_PINB = PINB;

    while (1)
    {

        LCD_execute_command(CLEAR_DISPLAY);
        LCD_print_hex4((bunker_value%5)+1);
        _delay_ms(50);
        // Left button check
        if (PINB & 2)
        {
            bunker_value++;
            _delay_ms(50);
        }

        // Right button check
        if (PINB & 32)
        {
            bunker_value--;
            _delay_ms(50);
        }

        // Middle button check
        if (!(PINB & 16))
        {
            return (bunker_value%5)+1;
        }
        

        last_PINB = PINB;
    }
}
