#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
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

class Song{
public:
	enum NotesName {C, D, E, F, G, A, B};
	void write_to_file();
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
	char statStr[5] = {'.', 'm', 'u', 's', 0};

	for (int i = 0; i < 5; i++){
		filenameCopy[filenameInd + i] = statStr[i];
	}

	outfile.open(filenameCopy);
	if (!outfile.is_open()){
		cerr << "Could not open file" << endl;
		return -1;
	}

	while (mySong != NULL){

	}


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
	unsigned int startGpio = 0;


	gpio_setup(gpioC);
	gpio_setup(gpioD);
	bool stillPlaying = true;;
	// float time;
	
	char* songName = new char[100];
	songName = argv[1];
	cout << "Got argument 1 into songName: " << songName << endl;

	Song newSong(songName);
	cout << "Created new song instance with songName: " << songName << endl;


	while (stillPlaying){

		if (gpio_get_value(gpioC)){
			// time = timePressed(gpioC);
			time = 1;
			printf("D is pressed (GPIO # %d) \n", gpioC);		// switch this to logging.
			playFreq(freqC, time, 7);
			addKeytoSong(C, newSong);
		}

		if (gpio_get_value(gpioD)){
			// time = timePressed(gpioD);
			time = 1;
			printf("D is pressed (GPIO # %d) \n", gpioD);		// switch this to logging.
			playFreq(freqD, time, -1);	
			addKeytoSong(D, newSong);
		}

		if(gpio_get_value(gpioEND)){
			stillPlaying = false;
			break;
		}


	}
}


