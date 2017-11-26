#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
// #include "sound.h"
#include <ugpio/ugpio.h>

#include "pwm-exp.h"

#define freqC 261
#define freqD 293
#define freqE 329
#define freqF 349
#define freqG 392
#define freqA 440
#define freqB 493

void playFreq(float freq, float timeToPlay){

	int status = pwmDriverInit();
	status = pwmSetFrequency(freq);
	status = pwmSetupDriver(-1, 45, 0);
	sleep(timeToPlay);
	status = pwmSetupDriver(-1, 0, 0);
	status = pwmSetFrequency(1526);

}
void gpio_setup(unsigned int gpio){
	gpio_request(gpio, "GPIO_LABEL");
	gpio_direction_input(gpio);
	gpio_set_value(gpio, gpio);	
}

float timePressed(unsigned int gpio){
	float time = 0;
	while (gpio_get_value(gpio)){
		sleep(0.1);
		time  = time + 0.1;
	}
	return time;

}

int main(){
	unsigned int gpioC = 9;
	unsigned int gpioD = 8;

	gpio_setup(gpioC);
	gpio_setup(gpioD);
	int stillPlaying = 1;
	int keyPressed = 0;
	float time;

	while (stillPlaying){

		if (gpio_get_value(gpioC)){
			time = timePressed(gpioC);
			printf("D is pressed (GPIO # %d \n", gpioC);
			playFreq(freqC, time);
		}

		if (gpio_get_value(gpioD)){
			time = timePressed(gpioD);
			printf("D is pressed (GPIO # %d \n", gpioD);
			playFreq(freqD, time);
		}

		// if (condition)
			// stillPlaying = 0;

	}
}

