#include "I2CLibrary.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int I2CLibrary::initI2C(string device, int slaveAddress){
	int i2c_fd;
	
	char *charDevice = const_cast<char*>(device.c_str());
	i2c_fd = open(charDevice, O_RDWR);
	
	if (i2c_fd >= 0){
		if (ioctl(i2c_fd, I2C_SLAVE, slaveAddress) < 0)
		{
			//no communication with slave possible
			return -1;
		}
     }
  return i2c_fd;
}

int I2CLibrary::sendBytes(int i2c_fd, char *buffer, int count)
{
	int sent = write(i2c_fd, buffer, count);
	if (sent < 0 || sent < count)
    {
		perror("sendbytes failed!");
		return -1;
    }
	return sent;
}

int I2CLibrary::sendString(int i2c_fd, string buffer)
{
	int length = buffer.length();
	char *charBuffer = const_cast<char*>(buffer.c_str());
	
	return sendBytes(i2c_fd, charBuffer, length);
}

bool I2CLibrary::readBytes(int i2c_fd, char *output, int max)
{
	int failed = read(i2c_fd, output, max);
	if (failed == -1)
		return false;
	return true;
}

bool I2CLibrary::readUntil(int i2c_fd, char *output, char sign, int max)
{
	char c;
	int count, i = 0;
	do
	{
		count = read(i2c_fd, (void*)&c, 1);
		if (count > 0)
		{
			if (c != sign)
				output[i++] = c;
		}
	}
	while (c != sign && i < max && count >= 0);

	if (count < 0)
		return false;
	else
		output[i] = '\0';
	
	return true;
}

