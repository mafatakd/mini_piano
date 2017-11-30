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

void playFreq(float freq, float timeToPlay, int pinToPlay){

	int status = pwmDriverInit();
	status = pwmSetFrequency(freq);
	status = pwmSetupDriver(pinToPlay, 45, 0);
	sleep(timeToPlay);
	status = pwmSetupDriver(pinToPlay, 0, 0);
	status = pwmSetFrequency(1526);

}
int main(){
	playFreq(440, 1, -1);

}