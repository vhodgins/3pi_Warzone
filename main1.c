#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include "lcd_driver.h"
#include "port_macros.h"
#include "go2.c"

int main()
{

	// Initializing the buttons
	unsigned int last_left_button_state = (PINB & (1 << 1));
	unsigned int left_button_pressed = 0;
	unsigned int last_right_button_state = (PINB & (1 << 5));
	unsigned int right_button_pressed = 0;
	unsigned int last_middle_button_state = (PINB & (1 << 4));

	int i;
	int p;		

	DDRD |= (1 << 1); //configure pin as output

	// Aware i can do all of these next few lines in 2 lines but this robot is so fucky and just was not working when i did it all like this
	//DDRB &= ~ (50);
	//PORTB |= 50
	// Left Button
	DDRB &= ~(1 << 1);
	PORTB |= (1 << 1);
	//Right Button
	DDRB &= ~(1 << 5);
	PORTB |= (1 << 5);
	//Middle Button
	DDRB &= ~(1 << 4);
	PORTB |= (1 << 4);

	char constants[2][4][8] = {{"slow    ", "med     ", "fast    "}, {"fw      ", "rv      ", "cw      ", "ccw     "}};
	// List of the options on the LCD

	int commands[4][3] = {};
	// Empty list that gets filled with instructions as you enter them

	initialize_LCD_driver();
	LCD_execute_command(TURN_ON_DISPLAY);

	uint8_t iter = 0;
	uint8_t done = 0;

	while (1)
	{
		LCD_execute_command(CLEAR_DISPLAY);

		uint8_t spd = 0;  // Holds speed
		uint8_t step = 0; // the current step of whats being chosen : 0 is speed, 1 is direction, 2, is duration, 3 is choosing to enter another command
		uint8_t dir = 0;  // Direction
		uint8_t val = 0;  // Temp variable, theoretically i could rewrite all of this code only using this one variable but it works right now and we dont really care about how
						  // much memory were using yet

		LCD_print_String(constants[0][0]); // Sets up the lcd to print the first option

		// Choosing the speed
		while (1)

		{

			if (done)
			{
				break;
			}

			// To show the current value the user is selecting.

			//Pulser for left button  -- Taken from lab 7
			if ((PINB & (1 << 1)) != last_left_button_state)
			{
				if ((PINB & (1 << 1)) == 0)
				{
					left_button_pressed = 1;
				}
				last_left_button_state = (PINB & (1 << 1));
			}
			else
			{
				left_button_pressed = 0;
			}

			//Pulser for right button
			if ((PINB & (1 << 5)) != last_right_button_state)
			{
				if ((PINB & (1 << 5)) == 0)
				{
					right_button_pressed = 1;
				}
				last_right_button_state = (PINB & (1 << 5));
			}
			else
			{
				right_button_pressed = 0;
			}

			// Middle button code, I put all of the functionality in here and not in the other two above for no reason. I was rushing
			if ((PINB & (1 << 4)) != last_middle_button_state)
			{
				if ((PINB & (1 << 4)) == 0)
				{
					LCD_execute_command(CLEAR_DISPLAY);
					step += 1;	  // Every time the middle button is pressed it will go to the next option to chose so it has to increment step
					switch (step) // Different functionality for each option
					{
					case 1:
						commands[iter][0]=val;
						val = 0;
						LCD_print_String(constants[1][val]);
						break;
					case 2:
						commands[iter][1]= val;
						val = 0;
						LCD_print_hex16(val);
						break;
					case 3:
						commands[iter][2] = val;

						spd = dir = val = 0;

						if (iter < 3)
						{
							LCD_print_String("addY/N"); // Asks to add another command if yes then itll go back to the top of this huge loop otherwise
						}
						else
						{
							LCD_print_hex16(commands[0][0]);
							int i;
							for (i = 0; i <= iter; i++) // It goes here, and does a for loop with each of the instructions here
							{
								
								go(commands[i][0], commands[i][1], commands[i][2]);
								// This is the function that will do the commands, you dont have to keep it as "go()"
							}
							done = 1;
						}
						break;

					default:
						break;
					}
				}
				last_middle_button_state = (PINB & (1 << 4));
			}

			// A pressed
			if (left_button_pressed == 1) // This just toggles left through the options
			{
				if (val > 0)
				{
					val -= 1;
				}
				LCD_execute_command(CLEAR_DISPLAY);
				if (step < 2)
				{
					LCD_print_String(constants[step][val]);
				}
				else if (step == 2)
				{
					LCD_print_hex16(val);
				}
				else
				{
					iter++;
					break;
				}
			}

			//C pressed -- Toggles right through the options
			if (right_button_pressed == 1)
			{
				if (step < 2)
				{
					if (val < (step ? 3 : 2))
					{
						val += 1;
					}
					LCD_execute_command(CLEAR_DISPLAY);
					LCD_print_String(constants[step][val]);
				}
				else if (step == 2)
				{
					val++;
					LCD_execute_command(CLEAR_DISPLAY);
					LCD_print_hex16(val);
				}
				else
				{
					LCD_print_hex16(commands[0][0]);
					int p;
					for (p = 0; p <= iter; p++)
					{
						LCD_execute_command(CLEAR_DISPLAY);
					LCD_print_hex16(p);
						go(commands[p][0], commands[p][1], commands[p][2]);
						
					}
					done = 1;
				}
			}

			_delay_us(10000); // really high delay so i dont have to worry about debouncing the buttons
		}
	}
	return 0;
}