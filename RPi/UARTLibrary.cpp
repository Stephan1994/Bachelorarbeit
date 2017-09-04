#include "UARTLibrary.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

int UARTLibrary::initUART(string device, int baudrate){
	int serial_fd;
	struct termios options;
	
	char *charDevice = const_cast<char*>(device.c_str());
	serial_fd = open(charDevice, O_RDWR | O_NOCTTY | O_NDELAY);
	
	if (serial_fd >= 0){
		fcntl(serial_fd, F_SETFL, 0);
		if (tcgetattr(serial_fd, &options) != 0) 
			return -1;
		
		memset(&options, 0, sizeof(options));
		
		cfsetispeed(&options, getBaudConstant(baudrate));
		cfsetospeed(&options, getBaudConstant(baudrate));
		
		options.c_cflag &= ~PARENB;         /* kein Paritybit */
		options.c_cflag &= ~CSTOPB;         /* 1 Stoppbit */
		options.c_cflag &= ~CSIZE;          /* 8 Datenbits */
		options.c_cflag |= CS8;

		options.c_cflag |= (CLOCAL | CREAD);

		options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
		options.c_iflag = IGNPAR;           
		options.c_oflag &= ~OPOST;          
		options.c_cc[VMIN]  = 0;            
		options.c_cc[VTIME] = 10;           
		tcflush(serial_fd,TCIOFLUSH);              
		if (tcsetattr(serial_fd, TCSAFLUSH, &options) != 0) 
			return -1;
     }
  return serial_fd;
}

int UARTLibrary::getBaudConstant(int baudrate){
	switch (baudrate){
		case 1200: return B1200;
		case 2400: return B2400;
		case 4800: return B4800;
		case 9600: return B9600;
		case 19200: return B19200;
		case 38400: return B38400;
		case 57600: return B57600;
		case 115200: return B115200;
		default: return B9600;
	}
}

int UARTLibrary::sendBytes(int serial_fd, char *buffer, int count)
{
	int sent = write(serial_fd, buffer, count);
	if (sent < 0 || sent < count)
    {
		perror("sendbytes failed!");
		return -1;
    }
	return sent;
}

int UARTLibrary::sendString(int serial_fd, string buffer)
{
	int length = buffer.length();
	char *charBuffer = const_cast<char*>(buffer.c_str());
	
	return sendBytes(serial_fd, charBuffer, length);
}

bool UARTLibrary::readBytes(int serial_fd, char *output, int max)
{
	int failed = read(serial_fd, output, max);
	if (failed == -1)
		return false;
	return true;
}

bool UARTLibrary::readUntil(int serial_fd, char *output, char sign, int max)
{
	char c;
	int count, i = 0;
	do
	{
		count = read(serial_fd, (void*)&c, 1);
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

