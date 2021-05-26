#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include "lcd_driver.h"
#include "port_macros.h"
#include "movement.c"
#include "sensors.c"
#include "menu.c"

int main()
{
	initialize_LCD_driver();
	LCD_execute_command(TURN_ON_DISPLAY);
	LCD_execute_command(CLEAR_DISPLAY);

	DDRC |= 0xFF;  // 21 = (1<<0)|(1<<2)|(1<<4)  -- Sets the DDRC[0,2,4] to outputs
	PORTC |= 0xFF; // Sets these ports high
	_delay_ms(1);  // Delay to allow them to reach high voltage
	DDRC &= 0x00;

	// Control Logic
	uint8_t speed = speed_pick();
	int bunkers_required = bunker_pick();

	// Program that actually does everything
	hunt(speed, bunkers_required);

	LCD_execute_command(CLEAR_DISPLAY);
	LCD_print_String("CLEAR");
}
