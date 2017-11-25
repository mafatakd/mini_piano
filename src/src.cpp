#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <ugpio/ugpio.h>

int main(){
	
	unsigned int gpioC = 9;

	gpio_request(gpioC, "doraFinds");


	gpio_direction_input(gpioC);
	gpio_set_value(gpioC, 0);

	if (gpio_get_value(gpioC)){
		std::cout << "I PLAYED SOME NOiSE ayy lmao" << std::endl;
	}

	


}






