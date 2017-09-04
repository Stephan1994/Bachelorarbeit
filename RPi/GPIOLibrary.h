
#ifndef GPIOLIB_H
#define GPIOLIB_H
#define GPIOCOUNT 26

#include<string>
using std::string;


class GPIOLibrary
{
	public:
		static bool initGPIO(int gpios[GPIOCOUNT][2]);
		static bool setGPIO(int gpio, int level);
		static int getGPIO(int gpio);
};

#endif //GPIOLIB_H