#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
// #include "sound.h"
// #include <ugpio/ugpio.h>

// #include "pwm-exp.h"


enum NotesName {C, D, E, F, G, A, B, Null};
// struct Key{
// 	NotesName name;
// };

// struct Notes{
// 	Key* key;
// 	Notes* next;
// 	int size;
// };

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


void Song::write_to_file(){

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

int main(){
	return 0;
}