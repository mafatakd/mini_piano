#include "pwm-exp.h"

// class Sound {
// public:
// 	void playFreq(float freq, float timeToPlay);
// }
void playFreq(float freq, float timeToPlay){

	int status = pwmDriverInit();
	status = pwmSetFrequency(freq);
	status = pwmSetupDriver(-1, 45, 0)
	sleep(timeToPlay);
	status = pwmSetDriver(-1, 0, 0);
	status = pwmSetFrequency(1526);

}
// int main(int argc, char **argv, char **envp){
	//Initializasion sequence on the servo expansion
	// int status = pwmDriverInit();
	// float freq = ;

	
	
	// while(freq <= 1500){
	// 	status = pwmSetFrequency(freq);

	// 	status = pwmSetupDriver(-1, 45, 0);
	// 	sleep(0.1);
	// 	freq = freq + 1; 
	// }	
	// status = pwmSetupDriver(-1, 0, 0);

