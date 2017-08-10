// Baseclass for Handlers
// Defines the functions, that are able to call from PC
// 

#include "RobotHandler.h"

#include "GPIOLibrary.h"
#include <cmath>
//MessageLength
#define ML 8192

using std::cout;
using std::endl;
using std::to_string;
using std::map;
using std::placeholders::_1;


RobotHandler::RobotHandler()
{
    strncpy(RobotMessage, "Robot", sizeof(RobotMessage));
	initFunctions();
}        

void RobotHandler::initFunctions()
{
	RobotFunctions = 
		{
			{"connect", std::bind(&RobotHandler::connect, this, _1)},
			{"disconnect", std::bind(&RobotHandler::closeConnection, this, _1)},
			{"forward", std::bind(&RobotHandler::forward, this, _1)},
			{"picture", std::bind(&RobotHandler::getPicture, this, _1)},
			{"video", std::bind(&RobotHandler::getVideo, this, _1)}
		};
}

RobotHandler::~RobotHandler(){}

string RobotHandler::connect(string value)
{
    cout << "connect called!" << endl;
	return "connected";
}

string RobotHandler::closeConnection(string value)
{
	cout << "closeConnection called" << endl;
	return "disconnected";
}

string RobotHandler::forward(string speed)
{
	cout << "forward called!" << endl;
	return "forwarded";
}

string RobotHandler::getPicture(string camera)
{
	cout << "getPicture called!" << endl;

	return GPIOLibrary::takePictureUSB(stoi(camera));
}

string RobotHandler::getVideo(string camera)
{
	GPIOLibrary::takePictureUSB(stoi(camera));
	return 0;
}



