#include "GPIOLibrary.h"

#include <iostream>
using std::cout;
using std::endl;
using std::to_string;

#include <wiringPi.h>

//initializes the needed GPIO pins
//gpios must contain the pin number with an additional number: 1 for input, 2 for output and everything else for not used
//please use BCM-Pin numbers
bool GPIOLibrary::initGPIO(int gpios[GPIOCOUNT][2]){
	
	wiringPiSetup();
	for(int i = 0; i < GPIOCOUNT; i++){
		if (gpios[i][1] == 1)
			pinMode(gpios[i][0], INPUT);
		else if (gpios[i][i] == 2)
			pinMode(gpios[i][0], OUTPUT);
	}
	return true;
}


//give gpio pin number (BCM) and 0 for low-level or 1 for high
bool GPIOLibrary::setGPIO(int gpio, int level){
	if (level == 0)
		digitalWrite(gpio, LOW);
	else
		digitalWrite(gpio, HIGH);
	return true;
}

//give gpio pin number (BCM) and 0 for low-level or 1 for high
int GPIOLibrary::getGPIO(int gpio){
	return digitalRead(gpio);
}
