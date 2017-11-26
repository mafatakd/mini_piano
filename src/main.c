#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "sound.h"
#include <ugpio/ugpio.h>

void gpio_setup(unsigned int gpio){
	gpio_request(gpio, "GPIO_LABEL");
	gpio_direction_input(gpio);
	gpio_set_value(gpio, gpio);	
}

int main(){
	unsigned int gpioC = 9;
	unsigned int gpioD = 8;

	gpio_setup(gpioC);
	gpio_setup(gpioD);

	int keyPressed = 0;

	while (!keyPressed){
		if (gpio_get_value(gpioC)){
			printf("D is pressed (GPIO # %d", gpioC);
			keyPressed = 1;
			int time = 3;
			// get time pressed
			playFreq(freqC, time);

		}
		if (gpio_get_value(gpioD)){
			printf("D is pressed (GPIO # %d", gpioD);
			keyPressed = 1;
			int time = 3;
			// get time pressed
			playFreq(freqD, time);
		}
	}

}
