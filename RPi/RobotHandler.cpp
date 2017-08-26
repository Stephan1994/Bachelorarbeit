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


RobotHandler::RobotHandler(MessageWriter *extWriter)
	:camera(extWriter)
{
	cout << "Test in Handler: " << extWriter->test << endl;
    strncpy(RobotMessage, "Robot", sizeof(RobotMessage));
	initFunctions();
	//camera = new Camera(extWriter);
}        

void RobotHandler::initFunctions()
{
	RobotFunctions = 
		{
			{"connect", std::bind(&RobotHandler::connect, this, _1)},
			{"disconnect", std::bind(&RobotHandler::closeConnection, this, _1)},
			{"forward", std::bind(&RobotHandler::forward, this, _1)},
			{"picture", std::bind(&RobotHandler::getPicture, this, _1)},
			{"startVideo", std::bind(&RobotHandler::startVideo, this, _1)},
			{"stopVideo", std::bind(&RobotHandler::stopVideo, this, _1)}
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
	if (camera.actStreaming)
		camera.stopStreaming(); 
	return "disconnected";
}

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


