#include "pwm-exp.h"

int main(int argc, char **argv, char **envp){

	//Initializasion sequence on the servo expansion
	int status = pwmDriverInit();
	float freq = 24;

	
	
	while(freq <= 1500){
		status = pwmSetFrequency(freq);

		status = pwmSetupDriver(-1, 45, 0);
		sleep(0.1);
		freq = freq + 1; 
	}	
	status = pwmSetupDriver(-1, 0, 0);
	

}
