#include "ExampleHandler.h"

#include <cmath>
#include "UARTLibrary.h"

using std::cout;
using std::endl;
using std::to_string;
using std::map;
using std::placeholders::_1;
       

ExampleHandler::ExampleHandler(MessageWriter *extWriter)
{
	initFunctions();
	camera.initCamera(extWriter, 0);
	SerialFd = UARTLibrary::initUART("/dev/ttyACM0",9600);
}        

void ExampleHandler::initFunctions()
{
	RobotHandler::initFunctions();
}

string ExampleHandler::forward(string speed)
{
	cout << "forward called!" << endl;
	UARTLibrary::sendString(SerialFd, "forward:" + speed + ";");
	return "forwarded";
}

string ExampleHandler::stop(string parameter)
{
	cout << "stop called!" << endl;
	
	if (parameter != ""){
		int motor = std::stoi(parameter);
		if (motor == 1)
			parameter = "left";
		else
			parameter = "right";
	}
	
	UARTLibrary::sendString(SerialFd, "stop:" + parameter + ";");
	return "stopped";
}

string ExampleHandler::backward(string speed)
{
	cout << "backward called!" << endl;
	UARTLibrary::sendString(SerialFd, "backward:" + speed + ";");
	return "backwarded";
}

string ExampleHandler::setSingleMotorSpeed(string parameter)
{
	cout << "setSingleMotorSpeed called!" << endl;
	std::size_t posOfSplit = parameter.find_last_of(',');
	int motor = std::stoi(parameter.substr(0 , posOfSplit));
	string speed = parameter.substr(posOfSplit + 1);
	if (speed[0] != '-'){
		if (motor == 1)
			UARTLibrary::sendString(SerialFd, "left:" + speed + ";");
		else
			UARTLibrary::sendString(SerialFd, "right:" + speed + ";");
	}
	else{
		if (motor == 2)
			UARTLibrary::sendString(SerialFd, "leftback:" + speed.substr(1) + ";");
		else
			UARTLibrary::sendString(SerialFd, "rightback:" + speed.substr(1) + ";");
	}
	
	return "set";
}

string ExampleHandler::setLeftNRightMotor(string parameter)
{
	cout << "setLeftNRightMotor called!" << endl;
	std::size_t posOfSplit = parameter.find_last_of(',');
	string left = parameter.substr(0 , posOfSplit);
	string right = parameter.substr(posOfSplit + 1);
	if (left[0] != '-')
		UARTLibrary::sendString(SerialFd, "left:" + left + ";");
	else
		UARTLibrary::sendString(SerialFd, "leftback:" + left.substr(1) + ";");
	
	if (right[0] != '-')
		UARTLibrary::sendString(SerialFd, "right:" + right + ";");
	else
		UARTLibrary::sendString(SerialFd, "rightback:" + right.substr(1) + ";");
	
	return "set";
}
	
string ExampleHandler::getSingleMotorSpeed(string motor){return "not implemented";}

string ExampleHandler::getProximitySensor(string sensor)
{
	cout << "getProximitySensor called!" << endl;
	UARTLibrary::sendString(SerialFd, "distance:;");
	char buffer[1000];
	UARTLibrary::readUntil(SerialFd, buffer, ';', 1000);
	string output(buffer);
	cout << "Got from Arduino: " << output << endl;
	//string output;
	std::size_t posOfSplit = output.find_last_of(':');
	output = output.substr(posOfSplit + 1);
	return output;
}
	

string ExampleHandler::getPicture(string cam)
{
	cout << "getPicture called!" << endl;

	return camera.takePicture();
}

string ExampleHandler::startVideo(string resolution)
{
	int i = 0;
	string colStr;
	while (resolution[i] != 'x') {
		colStr += resolution[i];
		i++;
	}
	int cols = stoi(colStr);
	string rowStr;
	i++;
	while (i < (int)resolution.length()) {
		rowStr += resolution[i];
		i++;
	}
	int rows = stoi(rowStr);
	
	camera.startStreaming(cols, rows);
	return "started";
}

string ExampleHandler::stopVideo(string cam)
{
	camera.stopStreaming();
	return "stoped";
}

