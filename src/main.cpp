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
//	function declarations
//////////////////////////////////////////////////////////////////////

void playFreq(float freq, float timeToPlay, int pinToPlay);
void gpio_setup(unsigned int gpio);
float timePressed(unsigned int gpio);
void playFreq(float freq, float timeToPlay, int pinToPlay);
void readFile(const char filename[])
void menu();

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
enum LETTERS {C, D, E, F, G, A, B};

//////////////////////////////////////////////////////////////////////
//	Song class declaration
//////////////////////////////////////////////////////////////////////

class Song{
public:

	int write_to_file(char filename[]);
	bool record();
	bool play();
	Song(char* songName);
	// Song(&Song);

	~Song();
	
	//////////////////////////////////////////////////////////////////////
	// helper functions

	bool addKeyToSong(LETTERS newName, int timePressed);
	int typetoFreq(LETTERS letter);


private:
	struct Notes{
		LETTERS name;
		int timeHeld;
		Notes* next;
	};

	Notes* mySong;
};

int Song::typetoFreq(LETTERS letter){

	int freqArr[7] = {261, 293, 329, 349, 392, 440, 493};
	return freqArr[letter];

}

bool Song::play(){
	
	if (mySong->timeHeld == -1){
		cerr << "OMG NO KEYS IN SONG CAN'T PLAY. ABORT" << endl;
		return false;
	}

	Notes* curr = mySong;

	while (curr != NULL){
		playFreq(typetoFreq(curr->name), curr->timeHeld, -1);
		curr = curr->next;
	}

	cout << "Done playing song" << endl;

	return true;
}

Song::Song(char* songName){
	mySong = new Notes;
	mySong->name = C;
	mySong->timeHeld = -1;
	mySong->next = NULL;
}


int Song::write_to_file(char filename[]){
	// check if no key in song :(
	if (mySong->timeHeld == -1){
		cerr << "Omg NOT EVEN A SINGLE NOTE" << endl;
		return -1;
	}

	// initializing file for output.
	ofstream outfile;
	char* filenameCopy;
	char piStr[5] = {'.', 'p', 'i', 0};
	int filenameInd = 0;

	while (filename[filenameInd] != 0){
		filenameInd += 1;
	}

	for (int i = 0; i < 5; i++){
		filenameCopy[filenameInd + i] = piStr[i];
	}
	outfile.open(filenameCopy);

	if (!outfile.is_open()){
		cerr << "Could not open file" << endl;
		return -2;
	}

	// going through the song to print the notes.
	Notes* curr = mySong;
	char lettersArr[7] = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};

	while (curr != NULL){
		outfile <<  lettersArr[curr->name] << " " << curr->timeHeld;
		if (curr->next != NULL){
			outfile << " ";
		}
		else{
			outfile << 0 << endl;
		}
	}
}

bool Song::addKeyToSong(LETTERS newName, int timePressed){

	if (mySong->timeHeld == -1){
		mySong->name = newName;
		mySong->timeHeld = timePressed;
		mySong->next = 0x0;
		return true;
	}

	Notes* curr = mySong;
	while (curr->next != NULL){
		curr = curr->next;
	}

	Notes* newNote = new Notes;
	newNote->name = newName;
	mySong->timeHeld = timePressed;
	newNote->next = NULL;
	curr->next = newNote;

	return true;
}


void Song::readFile(const char filename[]){
	const int maxLineLength=1000;
	char line[maxLineLength];

	char newfilename[100];

	int num= 0;
	for(int i= 0; filename[i] != '\0'; i++){
		newfilename[i]=filename[i];
		num++;
	}

	newfilename[num]= '.';
	newfilename[num+1]= 'p';
	newfilename[num+2]= 'i';

	ifstream infile;
	infile.open(newfilename);

  	if(!infile.is_open())
  		return -1;

  	bool done=false;
  	int fileLineNumber=0;	

  	while(!done){
        ++fileLineNumber;
        
        if (!infile.getline(line, maxLineLength)) {
            if (infile.eof()) {
                done = true;
            }

            else{
                return -1;
            }
        }

        else{
        	int counter=0;
        	char in=line[0];
        	bool gotLetter= false;
        	char lettersArr[7] = {'C', 'D', 'E', 'F', 'G', 'A', 'B'};
        	int index=0;
        	int num=0;

        	while(in!=0){
        		if(!gotLetter){
        			in=line[counter];

        			for(int i=0; i<7; i++){
        				if(in==lettersArr[i]){
        					index=i;
        					break;
        				}
        			}

      				gotLetter= true;
        		}

        		else if(gotLetter){
        			num=in-48;
        			gotLetter= false;
        		}

        		counter= counter+2;
        		addKeyToSong(index, num);
        	}
        }
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

// float timeToPlayPressed(unsigned int gpio){
	
// 	float timeToPlay = 0;
// 	while (gpio_get_value(gpio)){ // while clicked
// 		sleep(0.1);
// 		timeToPlay  = timeToPlay + 0.1;
// 	}
// 	printf("I'm done, I got the timeToPlay, it's %f.", timeToPlay);
// 	return timeToPlay;

// }

int recording(char songName[]){

	unsigned int gpioC = 9;
	unsigned int gpioD = 8;
	unsigned int gpioEND = 0;

	gpio_setup(gpioC);
	gpio_setup(gpioD);
	gpio_setup(gpioEND);

	bool stillPlaying = true;;
	float timeToPlay;
	
	// char* songName = new char[100];
	// songName = (char*) argv[1];
	cout << "Got argument 1 into songName: " << songName << endl;

	Song newSong(songName);
	cout << "Created new song instance with songName: " << songName << endl;


	while (stillPlaying){

		if (gpio_get_value(gpioC)){
			// timeToPlay = timeToPlayPressed(gpioC);
			timeToPlay = 1;
			printf("D is pressed (GPIO # %d) \n", gpioC);		// switch this to logging.
			playFreq(freqC, timeToPlay, 7);
			newSong.addKeytoSong(C, timeToPlay);
		}

		if (gpio_get_value(gpioD)){
			// timeToPlay = timeToPlayPressed(gpioD);
			timeToPlay = 1;
			printf("D is pressed (GPIO # %d) \n", gpioD);		// switch this to logging.
			playFreq(freqD, timeToPlay, -1);	
			newSong.addKeytoSong(D, timeToPlay);
		}

		if(gpio_get_value(gpioEND)){
			stillPlaying = false;
			break;
		}
	}

	if ( newSong.write_to_file(songName) < 0){
		cerr << "Some error while writing to file! Oh no..." << endl;
	}
	else{
		cerr << "File written successfully" << endl;
	}

	delete songName;
}


int getChoice(){
	int choice;
	cin >> choice;
	cin.ignore();
	return choice;
}

void menu(){

	cout << "Welcome to the MINI PIANO!" << endl;
	cout << "Please choose an option from below [1 or 2]" << endl;
	cout << "1. Play a song, and record it." << endl;
	cout << "2. Play a song from memory." << endl;
	cout << "3. Exit." << endl;
	int choice = getChoice();

	while (choice > 3 || choice < 1){
		cout << "Error please input a valid choice [1-3]" << endl;
		choice = getChoice();
	}
	if (choice == 1){
		char* songName;
		cin >> songName;
		recording(songName);
	}
	else if (choice == 2){

	}
	else if(choice == 3){
		cout << "Well done!, good bye!" << endl;
	}

}

int main(const int argc, const char* const argv[]){
	menu();
	return 0;


}




