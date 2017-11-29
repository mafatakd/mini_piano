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

/* 
record or play? (or stat)
if record 
	cin songname

	click buttons  -- songBuilding

	write song 
	append song name, number to menu file.


else if play:
	
	print options from menu file (file reading)
	choose song (cin numbers [0-9])
	play song --> other persons job

else if stat:
	print options form file (file reading)
	choose song (cin numbers [0-9])

 somehow go back to menu.cpp
*/

/*
	for file setup:
		menufile.pi
			list of songs and song names
		songname.pi
			C t B t D t E t G t C t B t ... N t 0
		each file has it's own .pi file.
		stat.pi:
			fk this.
*/


/* while recording:
	t = 1;



