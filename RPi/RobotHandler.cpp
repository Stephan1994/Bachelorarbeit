// Baseclass for Handlers
// Defines the functions, that are able to call from PC
// 

#include "RobotHandler.h"


using std::cout;
using std::endl;
using std::to_string;
using std::map;
using std::placeholders::_1;
       

void RobotHandler::initFunctions()
{
	RobotFunctions = 
		{
			{"connect", std::bind(&RobotHandler::connect, this, _1)},
			{"disconnect", std::bind(&RobotHandler::closeConnection, this, _1)},
			{"forward", std::bind(&RobotHandler::forward, this, _1)},
			{"backward", std::bind(&RobotHandler::backward, this, _1)},
			{"stop", std::bind(&RobotHandler::stop, this, _1)},
			{"setSingleMotor", std::bind(&RobotHandler::setSingleMotorSpeed, this, _1)},
			{"getSingleMotor", std::bind(&RobotHandler::getSingleMotorSpeed, this, _1)},
			{"setLeftNRight", std::bind(&RobotHandler::setLeftNRightMotor, this, _1)},
			{"distance", std::bind(&RobotHandler::getProximitySensor, this, _1)},
			{"picture", std::bind(&RobotHandler::getPicture, this, _1)},
			{"startVideo", std::bind(&RobotHandler::startVideo, this, _1)},
			{"stopVideo", std::bind(&RobotHandler::stopVideo, this, _1)}
		};
}

string RobotHandler::connect(string value)
{
    cout << "connect called!" << endl;
	return "connected";
}

string RobotHandler::closeConnection(string value)
{
	cout << "closeConnection called" << endl;
	if (camera.actStreaming)
		camera.stopStreaming(); 
	return "disconnected";
}
/*
string RobotHandler::forward(string speed)
{
	cout << "forward called!" << endl;
	return "forwarded";
}

string RobotHandler::getPicture(string cam)
{
	cout << "getPicture called!" << endl;

	return camera.takePicture();
}

string RobotHandler::startVideo(string resolution)
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

string RobotHandler::stopVideo(string cam)
{
	camera.stopStreaming();
	return "stoped";
}

*/
