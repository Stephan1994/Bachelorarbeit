#ifndef UARTLIB_H
#define UARTLIB_H

#include <string>
using std::string;

class UARTLibrary
{
	public:
		static int initUART(string device, int baudrate);
		static int sendBytes(int serial_fd, char *buffer, int count);
		static int sendString(int serial_fd, string buffer);
		static bool readBytes(int serial_fd, char *output, int max);
		static bool readUntil(int serial_fd, char *output, char sign, int max);
		
private:
		static int getBaudConstant(int baudrate);
};

#endif //UARTLIB_H