#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
// #include "sound.h"
#include <ugpio/ugpio.h>

#include "pwm-exp.h"

//////////////////////////////////////////////////////////////////////
//	structs and definitions
//////////////////////////////////////////////////////////////////////
#define freqC 261
#define freqD 293
#define freqE 329
#define freqF 349
#define freqG 392
#define freqA 440
#define freqB 493



enum noteName {C, D, E, F, G, A, B};

struct Key{
	noteName name;
};

struct Note{

	Key* key;
	Note* next;
	int size;

};
//////////////////////////////////////////////////////////////////////
//	function declarations
//////////////////////////////////////////////////////////////////////
void playFreq(float freq, float timeToPlay, int pinToPlay);
void gpio_setup(unsigned int gpio);
float timePressed(unsigned int gpio);
bool addKeytoSong(noteName key, Note*& song);


//////////////////////////////////////////////////////////////////////
//	implementation
//////////////////////////////////////////////////////////////////////


void playFreq(float freq, float timeToPlay, int pinToPlay){

	int status = pwmDriverInit();
	status = pwmSetFrequency(freq);
	status = pwmSetupDriver(pinToPlay, 45, 0);
	sleep(timeToPlay);
	status = pwmSetupDriver(pinToPlay, 0, 0);
	status = pwmSetFrequency(1526);

}
void gpio_setup(unsigned int gpio){
	gpio_request(gpio, "GPIO_LABEL");
	gpio_direction_input(gpio);
	gpio_set_value(gpio, gpio);	
}

float timePressed(unsigned int gpio){
	
	float time = 0;
	while (gpio_get_value(gpio)){ // while clicked
		sleep(0.1);
		time  = time + 0.1;
	}
	printf("I'm done, I got the time, it's %f.", time);
	return time;

}
bool addKeytoSong(noteName noteName, Note*& song){
	
	Key* newKey = new Key;
	newKey->name = noteName;
	song->size += 1;

	// if (song->key == NULL){
	// 	song->key = newKey;
	// 	return true;
	// }

	Note* curr = song;
	while (curr->next != NULL){
		curr = curr->next;
	}

	Note* newNote = new Note;
	newNote->key = newKey;
	newNote->next = NULL;
	newNote->size = -1;
	curr->next = newNote;

	return true;
}


int main(){
	unsigned int gpioC = 9;
	unsigned int gpioD = 8;
	unsigned int startGpio = 0;


	gpio_setup(gpioC);
	gpio_setup(gpioD);
	bool stillPlaying = true;;
	float time;

	Note* newSong = new Note;


	while (stillPlaying){

		if (gpio_get_value(gpioC)){
			// time = timePressed(gpioC);
			time = 1;
			printf("D is pressed (GPIO # %d) \n", gpioC);	
			playFreq(freqC, time, 7);
			addKeytoSong(C, newSong);
		}

		if (gpio_get_value(gpioD)){
			// time = timePressed(gpioD);
			time = 1;
			printf("D is pressed (GPIO # %d) \n", gpioD);
			playFreq(freqD, time, -1);	
			addKeytoSong(D, newSong);
		}
	}



}


