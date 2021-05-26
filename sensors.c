#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include "lcd_driver.h"



// Sensor values
int s0(){
    return (PINC & 1);
}

int s1(){
    return (PINC & 2);
}

int s2(){
    return (PINC & 4);
}

int s3(){
    return (PINC & 8);
}

int s4(){
    return (PINC & 16);
}



// The real "main" program
void hunt(int speed, int bunkers_required)
	{
		uint8_t bunker_detected = 0, bunkers_found = 0, sensor_sum;

		while (1)
		{
			sensor_sum = s0() + s1() + s2() + s3() + s4(); 
			// Sum of the sensor readings lets you know how many sensors are detecting an obstacle.
			// This is helpful because generally 4 of the sensors will be active when it hits a "wall"
			// and only 2 or 3 when it encounters a "bunker."

			LCD_execute_command(CLEAR_DISPLAY);
			LCD_print_String("Scan: ");
			LCD_print_hex8(PINC);
			LCD_move_cursor_to_col_row(0,1);
			LCD_print_String("Bunk:");
			LCD_print_hex4(bunkers_found);

			_delay_ms(5 * speed);

			if (sensor_sum == 0) // If nothing is detected we continue moving forward
			{
				if (bunker_detected) // If no obstacle is detected the cycle after we detected something, its likely we ran over a bunker.
				{
					bunker(); 
					bunkers_found++;
					bunker_detected = 0;
				}
				forward(speed);
			}

			if (sensor_sum > 3) // If 4+ sensors detect somethin its probably hitting a wall
			{
				bunker_detected = 0;
				wall();
			}
			else if (s3() | s4()) // If the sensors on the right detect something we should turn to check
			{
				bunker_detected = 1;
				rightshort();
			}
			else if (s0() | s1()) // If the sensors on the left detect something we also should turn 
			{
				bunker_detected = 1;
				leftshort();
			}

			if (s2() & (s1() | s3())) // If the front sensor and the side sensors detect something and the sum is less than 4 its also probably a bunker
			{
				bunker();
				bunkers_found++;
				bunker_detected = 0;
			}

			if (bunkers_found == bunkers_required) // Ends the loop and brings us back to the main function
			{
				break;
			}
		}
	}