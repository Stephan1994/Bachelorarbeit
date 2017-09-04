#ifndef I2CLIB_H
#define I2CTLIB_H

#include <string>
using std::string;

class I2CLibrary
{
	public:
		static int initI2C(string device, int slaveAddress);
		static int sendBytes(int i2c_fd, char *buffer, int count);
		static int sendString(int i2c_fd, string buffer);
		static bool readBytes(int i2c_fd, char *output, int max);
		static bool readUntil(int i2c_fd, char *output, char sign, int max);
};

#endif //I2CLIB_H