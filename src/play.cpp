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


void readFile(const char filename[]);


void readFile(const char filename[]){
	const int maxLineLength=100;
	char line[maxLineLength];

	char newfilename[100];

	int num= 0;
	for(int i= 0; filename[i] != '\0'; i++){
		newfilename[i]=filename[i];
		num++;
	}





ifstream infile;
infile.open(filename);

}
// call song.play();


// somehow go back to main.cpp


C t B t D t E t G t C t B t ... N t 