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
void playFreq(float freq, float timeToPlay, int pinToPlay);
void menu();
int recording(char songName[]);


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
	Song(char songName[]);
	int readFile(const char filename[]);
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

Song::Song(char songName[]){
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


int Song::readFile(const char filename[]){

	const int maxLineLength=1000;
	char line[maxLineLength];

	char newfilename[100];

	int num= 0;
	for(int i= 0; filename[i] != 0; i++){
		newfilename[i]=filename[i];
		num++;
	}

	newfilename[num]= '.';
	newfilename[num+1]= 'p';
	newfilename[num+2]= 'i';
	newfilename[num+3] = 0;
	for (int i = 0; newfilename[i] != 0; i++){
		cout << newfilename[i];
	}
	cout << endl;	
	
	ifstream infile;
	infile.open(newfilename);

  	if(!infile.is_open())
  		return -1;
	
  	bool done=false;
  	int fileLineNumber=0;	
	Notes* curr = mySong;

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

        	while(in != 0){
		in=line[counter];
		cout<< counter;
		cout<<endl;
        		if(!gotLetter){
        			for(int i=0; i<7; i++){
        				if(in==lettersArr[i]){
        					index=i;
						cout << "in for loop: " << lettersArr[index] << endl;
        					break;
        				}
        			}
      				gotLetter= true;
				cout << "after got letter: " << (LETTERS) index <<endl;
        		}

        		else if(gotLetter){
        			num=in-48;
        			gotLetter= false;
				cout << num << endl;
				addKeyToSong((LETTERS) index, num);
				cout << "song key" <<  curr->name << ", " << curr->timeHeld;
				curr = curr->next;
				cout<<endl;
        		}
        		counter= counter+2;
			


        		

        	}
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

	unsigned int gpioEND = 0;

	unsigned int gpioC = 2;
	unsigned int gpioD = 3;
	unsigned int gpioE = 1;
	unsigned int gpioF = 99;
	unsigned int gpioG = 100;
	unsigned int gpioA = 101;
	unsigned int gpioB = 102;




	gpio_setup(gpioC);
	gpio_setup(gpioD);
	gpio_setup(gpioEND);

	bool stillPlaying = true;;
	float timeToPlay;


	Song newSong(songName);
	cout << "Created new song instance with songName: " << songName << endl;


	while (stillPlaying){

		if(gpio_get_value(gpioEND)){
			cerr << "Done playing" << endl;
			stillPlaying = false;
			break;
		}

		if (gpio_get_value(gpioC)){
			// timeToPlay = timeToPlayPressed(gpioC);
			timeToPlay = 1;
			printf("D is pressed (GPIO # %d) \n", gpioC);		// switch this to logging.
			playFreq(freqC, timeToPlay, -1);
			newSong.addKeyToSong(C, timeToPlay);
		}

		else if (gpio_get_value(gpioD)){
			// timeToPlay = timeToPlayPressed(gpioD);
			timeToPlay = 1;
			printf("D is pressed (GPIO # %d) \n", gpioD);		// switch this to logging.
			playFreq(freqD, timeToPlay, -1);	
			newSong.addKeyToSong(D, timeToPlay);
		}

		else if (gpio_get_value(gpioE)){
			timeToPlay = 1;
			printf("D is pressed (GPIO # %d) \n", gpioE);		// switch this to logging.
			playFreq(freqE, timeToPlay, -1);	
			newSong.addKeyToSong(E, timeToPlay);
		}

		
	}

	if ( newSong.write_to_file(songName) < 0){
		cerr << "Some error while writing to file! Oh no..." << endl;
	}
	else{
		cerr << "File written successfully" << endl;
	}
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
		string songName;
		cout << "Please type in your song name: ";
		getline(cin, songName);
		cerr << "Converting input to char array..." << endl;
		char* songNameArr = new char[songName.length() + 1];
		cerr << "Calling recording function with song name: " << songName << endl;
		recording(songNameArr);

		delete songNameArr;
	}
	else if (choice == 2){
		char* SongName= new char[5];
		SongName[0]= 'S';
		SongName[1]= 'o';
		SongName[2]= 'n';
		SongName[3]= 'g';
		SongName[4]= 0;
		Song newSong(SongName);
		newSong.readFile(SongName);
		newSong.play();
		delete SongName;

	}
	else if(choice == 3){
		cout << "Well done!, good bye!" << endl;
	}

}

int main(const int argc, const char* const argv[]){
	menu();
	return 0;


}




