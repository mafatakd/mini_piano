#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
// #include "sound.h"
#include <ugpio/ugpio.h>

#include "pwm-exp.h"
using namespace std;
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
enum NotesName {C, D, E, F, G, A, B};

class Song{
public:
	int write_to_file(char filename[]);
	void play(char filename[]);
	bool record();
	Song(char* songName);
	// Song(&Song);

	~Song();
	
	//////////////////////////////////////////////////////////////////////
	//	helper functions
	//////////////////////////////////////////////////////////////////////
	bool addKeytoSong(NotesName newName);

private:
	struct Key{
	NotesName name;
	};

	struct Notes{
		Key* key;
		Notes* next;
		int size;
	};
	Notes* mySong;
};



void Song::play(char filename[]){
	// take a given file and play the song.
}

Song::Song(char* songName){
	mySong = new Notes;
	mySong->key = NULL;
	mySong->next = NULL;
	mySong->size = 0;
	

}


int Song::write_to_file(char filename[]){
	if (mySong->key == NULL){
		cerr << "Omg NOT EVEN A SINGLE NOTE" << endl;
		return -1;
	}
	ofstream outfile;
	char* filenameCopy;
	char musStr[5] = {'.', 'm', 'u', 's', 0};
	int filenameInd = 0;

	while (filename[filenameInd] != 0){
		filenameInd += 1;
	}

	for (int i = 0; i < 5; i++){
		filenameCopy[filenameInd + i] = musStr[i];
	}

	outfile.open(filenameCopy);
	if (!outfile.is_open()){
		cerr << "Could not open file" << endl;
		return -1;
	}

	// while (mySong != NULL){

	// }


}



bool Song::addKeytoSong(NotesName newName){
	
	Key* newKey = new Key;
	newKey->name = newName;
	mySong->size += 1;

	if (mySong->key == NULL){
		mySong->key = newKey;
		mySong->next = 0x0;
		return true;
	}

	Notes* curr = mySong;
	while (curr->next != NULL){
		curr = curr->next;
	}

	Notes* newNote = new Notes;
	newNote->key = newKey;
	newNote->next = NULL;
	newNote->size = -1;
	curr->next = newNote;

	return true;
}

Song::~Song(){
	Notes* curr = mySong;
	while (curr != NULL){
		Notes* next = curr->next;
		delete curr;
		curr = next;
	}
	mySong = 0;
}



//////////////////////////////////////////////////////////////////////
//	function declarations
//////////////////////////////////////////////////////////////////////
void playFreq(float freq, float timeToPlay, int pinToPlay);
void gpio_setup(unsigned int gpio);
float timePressed(unsigned int gpio);

//////////////////////////////////////////////////////////////////////
//	implementation
//////////////////////////////////////////////////////////////////////


void playFreq(float freq, float timeToPlayToPlay, int pinToPlay){

	int status = pwmDriverInit();
	status = pwmSetFrequency(freq);
	status = pwmSetupDriver(pinToPlay, 45, 0);
	sleep(timeToPlayToPlay);
	status = pwmSetupDriver(pinToPlay, 0, 0);
	status = pwmSetFrequency(1526);

}
void gpio_setup(unsigned int gpio){
	gpio_request(gpio, "GPIO_LABEL");
	gpio_direction_input(gpio);
	gpio_set_value(gpio, gpio);	
}




float timeToPlayPressed(unsigned int gpio){
	
	float timeToPlay = 0;
	while (gpio_get_value(gpio)){ // while clicked
		sleep(0.1);
		timeToPlay  = timeToPlay + 0.1;
	}
	printf("I'm done, I got the timeToPlay, it's %f.", timeToPlay);
	return timeToPlay;

}
// bool addKeytoSong(noteName noteName, Note*& song){
	
// 	Key* newKey = new Key;
// 	newKey->name = noteName;
// 	song->size += 1;

// 	// if (song->key == NULL){
// 	// 	song->key = newKey;
// 	// 	return true;
// 	// }

// 	Note* curr = song;
// 	while (curr->next != NULL){
// 		curr = curr->next;
// 	}

// 	Note* newNote = new Note;
// 	newNote->key = newKey;
// 	newNote->next = NULL;
// 	newNote->size = -1;
// 	curr->next = newNote;

// 	return true;
// }


int main(const int argc, const char* const argv[]){
	unsigned int gpioC = 9;
	unsigned int gpioD = 8;
	unsigned int gpioEND = 0;


	gpio_setup(gpioC);
	gpio_setup(gpioD);
	gpio_setup(gpioEND);

	bool stillPlaying = true;;
	float timeToPlay;
	
	char* songName = new char[100];
	songName = (char*) argv[1];
	cout << "Got argument 1 into songName: " << songName << endl;

	Song newSong(songName);
	cout << "Created new song instance with songName: " << songName << endl;


	while (stillPlaying){

		if (gpio_get_value(gpioC)){
			// timeToPlay = timeToPlayPressed(gpioC);
			timeToPlay = 1;
			printf("D is pressed (GPIO # %d) \n", gpioC);		// switch this to logging.
			playFreq(freqC, timeToPlay, 7);
			newSong.addKeytoSong(C);
		}

		if (gpio_get_value(gpioD)){
			// timeToPlay = timeToPlayPressed(gpioD);
			timeToPlay = 1;
			printf("D is pressed (GPIO # %d) \n", gpioD);		// switch this to logging.
			playFreq(freqD, timeToPlay, -1);	
			newSong.addKeytoSong(D);
		}

		if(gpio_get_value(gpioEND)){
			stillPlaying = false;
			break;
		}
	~newSong();

	}
}


