#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
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
void playFreq(float freq, int timeToPlay, int pinToPlay);
void menu();
int recording(char songName[]);
int logging(string s);

//////////////////////////////////////////////////////////////////////
//	structs and definitions
//////////////////////////////////////////////////////////////////////

#define freqC 523
#define freqD 587
#define freqE 659
#define freqF 698
#define freqG 784
#define freqA 880
#define freqB 988

enum LETTERS { C,
	D,
	E,
	F,
	G,
	A,
	B };

//////////////////////////////////////////////////////////////////////
//	Song class declaration
//////////////////////////////////////////////////////////////////////

class Song {
public:
	int write_to_file();
	bool record();
	bool play();
	Song(char songName[], int songNameLen);
	int readFile();
	~Song();

	//////////////////////////////////////////////////////////////////////
	// helper functions

	bool addKeyToSong(LETTERS newName, int timePressed);
	int typetoFreq(LETTERS letter);

private:
	struct Notes {
		LETTERS letterEnum;
		int timeHeld;
		Notes* next;
	};

	Notes* mySong;
	char* mySongName;
	int mySongNameLen;
};

int Song::typetoFreq(LETTERS letter){

	int freqArr[7] = { freqC, freqD, freqE, freqF, freqG, freqA, freqB };
	return freqArr[letter];
}

bool Song::play(){

	if (mySong->timeHeld == -1) {
		logging("OMG NO KEYS IN SONG, CAN'T PLAY. ABORT");
		return false;
	}

	Notes* curr = mySong;

	while (curr != NULL) {
		LETTERS currentNote = (LETTERS)curr->letterEnum;
		// logging("Playing note: " + (string) currentNote + " for " + (string) curr->timeHeld);
		playFreq(typetoFreq(currentNote), curr->timeHeld, -1);
		curr = curr->next;
	}

	logging("Done playing song");

	return true;
}

Song::Song(char songName[], int songNameLen){

	logging("Constructing song class");

	mySong = new Notes;
	mySong->letterEnum = C;
	mySong->timeHeld = -1;
	mySong->next = NULL;

	mySongNameLen = songNameLen;

	mySongName = new char[mySongNameLen];
	int i = 0;
	while (songName[i] != 0 && i < 300) {
		mySongName[i] = songName[i];
		i++;
	}
	mySongName[i] = 0;

	// logging("Printing songName recieved: ")
	// for (int i = 0; mySongName[i] != 0; i++) {
	// 	cout << mySongName[i];
	// }
	// cout << endl;

	if (i == 300) {
		logging("Songname not 0 appended!");
	}
	logging("Done constructing song class");
}

int Song::write_to_file(){

	cerr << "Entered Song::write_to_file" << endl;

	// check if no key in song :(
	if (mySong->timeHeld == -1) {
		cerr << "Omg NOT EVEN A SINGLE NOTE" << endl;
		return -1;
	}


	// initializing file for output.
	ofstream outfile;
	cout << "Instantiated outfile." << endl;

	char filename[mySongNameLen + 3];

	cout << "Created filename array" << endl;

	char piStr[4] = { '.', 'p', 'i', 0 };

	cout << "Created .pi array" << endl;

	
	int filenameInd = 0;

	while (*(mySongName + filenameInd) != 0) {
		filename[filenameInd] = *(mySongName + filenameInd);
		cout << filename[filenameInd];
		filenameInd += 1;
	}
	cout << endl;
	
	logging("Got filename");

	for (int i = 0; i < 4; i++) {
		filename[filenameInd + i] = piStr[i];
	}
	int i = 0;
	while (filename[i] != 0){
		cout << filename[i];
		i++;
	}
	cout << endl;
	logging("Got filename to write to using mySongName");
 
	outfile.open(filename);

	if (!outfile.is_open()) {
		logging("Could not open file");
		return -2;
	}
	logging("File opened successfully");
	// going through the song to print the notes.
	Notes* curr = mySong;
	char lettersArr[7] = { 'C', 'D', 'E', 'F', 'G', 'A', 'B' };

	while (curr != NULL) {
		outfile << lettersArr[curr->letterEnum] << " " << curr->timeHeld;
		if (curr->next != NULL) {
			outfile << " ";
		}
		else {
			outfile << 0 << endl;
		}
		curr = curr->next;
	}
}

bool Song::addKeyToSong(LETTERS newName, int timePressed){

	if (timePressed < 1) {
		logging("addKeyToSong: Time pressed (passed to this function) is less than 1. ABORT");
		return false;
	}

	if (mySong->timeHeld == -1) {
		mySong->letterEnum = newName;
		mySong->timeHeld = timePressed;
		mySong->next = NULL;
		// logging("Successfully added first note to song! Note: " + (string) mySong->letterEnum (string) ", timeHeld: " + (string) mySong->timeHeld);
		return true;
	}

	Notes* curr = mySong;
	while (curr->next != NULL) {
		curr = curr->next;
	}

	Notes* newNote = new Notes;
	newNote->letterEnum = newName;
	newNote->timeHeld = timePressed;
	newNote->next = NULL;

	curr->next = newNote;
	// logging("Successfully added next note to song with note: " + (string) curr->next->letterEnum + " and timeHeld: " + (string) curr->next->timeHeld);
	return true;
}

int Song::readFile(){

	const int maxLineLength = 1000;
	char line[maxLineLength];

	char filename[mySongNameLen + 4];

	int i = 0;
	while (mySongName[i] != 0 && i < 300) {
		filename[i] = mySongName[i];
		i++;
	}

	filename[i] = '.';
	filename[i + 1] = 'p';
	filename[i + 2] = 'i';
	filename[i + 3] = 0;

	for (int i = 0; filename[i] != 0; i++) {
		cerr << filename[i];
	}
	cerr << endl;
	logging("readFile: Got filename.");

	ifstream infile;
	infile.open(filename);

	if (!infile.is_open())
		return -1;

	bool done = false;
	int fileLineNumber = 0;
	Notes* curr = mySong;

	while (!done) {
		++fileLineNumber;

		if (!infile.getline(line, maxLineLength)) {
			if (infile.eof()) {
				done = true;
			}
			else {
				return -1;
			}
		}

		else {
			int counter = 0;
			char in = line[0];
			bool gotLetter = false;
			char lettersArr[7] = { 'C', 'D', 'E', 'F', 'G', 'A', 'B' };
			int index = 0;
			int num = 0;

			while (in != 0) {
				in = line[counter];
				cout << counter << endl;

				if (!gotLetter) {
					for (int i = 0; i < 7; i++) {
						if (in == lettersArr[i]) {
							index = i;
							cout << "readFile: Letter recognized by array loop: " << lettersArr[index] << endl;
							break;
						}
					}
					gotLetter = true;
					cout << "readFile: Got letter: " << (LETTERS)index << endl;
				}

				else if (gotLetter) {
					num = in - 48;
					gotLetter = false;
					cout << num << endl;
					addKeyToSong((LETTERS)index, num);
				}
				counter = counter + 2;
			}
		}
	}
}

Song::~Song(){
	Notes* curr = mySong;
	while (curr != NULL) {
		Notes* next = curr->next;
		delete curr;
		curr = next;
	}
	mySong = 0;
	mySongNameLen = 0;
	delete mySongName;
}

//////////////////////////////////////////////////////////////////////
//	implementation
//////////////////////////////////////////////////////////////////////

void playFreq(float freq, int timeToPlay, int pinToPlay){

	int status = pwmDriverInit();

	status = pwmSetFrequency(freq);
	status = pwmSetupDriver(pinToPlay, 10, 0);

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

int logging(string s){
	time_t rawtime;
	struct tm * timeinfo;
	time (&rawtime);
	timeinfo = localtime (&rawtime);

	ofstream outfile; 
    	outfile.open("Logging.txt");

    if (!outfile.is_open())
        return -1;  

    outfile<< timeinfo->tm_hour << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec << s;
    outfile<<endl;
    outfile.close();
}

int whichPin(bool pinSeven){
	if (pinSeven){
		return 7;
	}
	else{
		return 0;
	}
}

int recording(char songName[], int songNameLength){
	unsigned int gpioEND = 1;

	unsigned int gpioC = 19;	// white
	unsigned int gpioD = 9;	// green
	unsigned int gpioE = 3;	// yellow		
	unsigned int gpioF = 8;	// blue
	unsigned int gpioA = 18;	// grey
	unsigned int gpioG = 0;

	gpio_setup(gpioA);
	gpio_setup(gpioC);
	gpio_setup(gpioD);
	gpio_setup(gpioE);
	gpio_setup(gpioF);
	gpio_setup(gpioG);
	gpio_setup(gpioEND);

	bool stillPlaying = true;
	bool pinSeven = true;
	int timeToPlay;

	Song newSong(songName, songNameLength);
	cout << "Created new song instance with songName" << endl;

	while (stillPlaying) {

		if (gpio_get_value(gpioEND)) {
			cerr << "Well played! Your masterpiece was saved." << endl;
			stillPlaying = false;
			break;
		}

		if (gpio_get_value(gpioC)) {
			// timeToPlay = timeToPlayPressed(gpioC);
			timeToPlay = 1;
			printf("C is pressed (GPIO # %d) \n", gpioC); // switch this to logging.
			playFreq(freqC, timeToPlay, whichPin(pinSeven));
			pinSeven = !pinSeven;
			newSong.addKeyToSong(C, timeToPlay);
		}

		else if (gpio_get_value(gpioD)) {
			// timeToPlay = timeToPlayPressed(gpioD);
			timeToPlay = 1;
			printf("D is pressed (GPIO # %d) \n", gpioD); // switch this to logging.
			playFreq(freqD, timeToPlay, whichPin(pinSeven));
			pinSeven = !pinSeven;
			newSong.addKeyToSong(D, timeToPlay);
		}

		else if (gpio_get_value(gpioE)) {
			timeToPlay = 1;
			printf("E is pressed (GPIO # %d) \n", gpioE); // switch this to logging.
			playFreq(freqE, timeToPlay, whichPin(pinSeven));
			pinSeven = !pinSeven;
			newSong.addKeyToSong(E, timeToPlay);
		}
		else if (gpio_get_value(gpioF)) {
			timeToPlay = 1;
			printf("F is pressed (GPIO # %d) \n", gpioF); // switch this to logging.
			playFreq(freqF, timeToPlay, whichPin(pinSeven));
			pinSeven = !pinSeven;
			newSong.addKeyToSong(F, timeToPlay);
		}
		else if (gpio_get_value(gpioG)) {
			timeToPlay = 1;
			printf("G is pressed (GPIO # %d) \n", gpioG); // switch this to logging.
			playFreq(freqG, timeToPlay, whichPin(pinSeven));
			pinSeven = !pinSeven;
			newSong.addKeyToSong(G, timeToPlay);
		}
		else if (gpio_get_value(gpioA)) {
			timeToPlay = 1;
			printf("A is pressed (GPIO # %d) \n", gpioA); // switch this to logging.
			playFreq(freqA, timeToPlay, whichPin(pinSeven));
			pinSeven = !pinSeven;
			newSong.addKeyToSong(A, timeToPlay);
		}
	}

	if (newSong.write_to_file() < 0) {
		cerr << "Some error while writing to file! Oh no..." << endl;
	}
	else {
		cerr << "File written successfully" << endl;
	}
	newSong.play();
	menu();

	// implement write to menu

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

	while (choice > 3 || choice < 1) {
		cout << "Error please input a valid choice [1-3]" << endl;
		choice = getChoice();
	}
	if (choice == 1) {
		string songName;
		cout << "Please type in your song name: ";
		getline(cin, songName);

		cerr << "Converting input to char array..." << endl;
		cout << songName << endl;
		char songNameArr[songName.length() + 1];
		
		int i = 0;

		for (i = 0; i < songName.length(); i++) {
			songNameArr[i] = songName[i];
			cout << songNameArr[i] << endl;
		}
		songNameArr[i] = 0;


		cerr << "Calling recording function with song name: " << songName << endl;

		recording(songNameArr, songName.length() + 1);
	}

	else if (choice == 2) {
		cout << "Which song would you like to play?" << endl;

		//First read file called Choices.txt
		int maxLineLength = 100;
		bool done = false;
		int fileLineNumber = 0;

		//string songNames[9] = {0};
		string txt;
		string line[9];

		ifstream in("Choices.txt");
		string s;
		int lineCounter = 0;
		while (getline(in, s)) {
			cout << s << '\n';
			line[lineCounter] = s;
			lineCounter++;
		}

		// After displaying the choices of the song, the user will enter their choice
		int choice;

		cout << "Enter your song choice: ";

		// User enters a choice
		cin >> choice;

		//error check
		while (choice < 1 || choice > 9) {
			cerr << "Error, you must select a song between the range of 1 to 9" << endl;
			cin >> choice;
		}

		string sentence = line[choice - 1];

		char chosenSong[sentence.length() - 2];
		int i = 0;
		for (i = 0; i < sentence.length() - 3; i++) {
			chosenSong[i] = sentence[i + 3];
			cout << chosenSong[i] << endl;
		}
		chosenSong[i] = 0;

		cout << "Your choice was: " << chosenSong << endl;

		Song newSong(chosenSong, sentence.length() - 2);
		newSong.readFile();
		newSong.play();
	}
	else if (choice == 3) {
		cout << "Well done!, good bye!" << endl;
	}
}

int main(const int argc, const char* const argv[]){
	menu();
	return 0;
}
