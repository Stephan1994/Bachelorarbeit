// Baseclass for Handlers
// Defines the functions, that are able to call from PC
// 

#include "RobotHandler.h"

#include "GPIOLibrary.h"
#include <math.h>
//MessageLength
#define ML 8192

using std::cout;
using std::endl;
using std::to_string;
using std::map;
using std::placeholders::_1;


RobotHandler::RobotHandler()
{
	char robArray[6];
    strncpy(RobotMessage, "Robot", sizeof(RobotMessage));
    robArray[sizeof(RobotMessage) - 1] = 0;
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
/*
bool RobotHandler::createMessage(char* out, string command, string value, bool request)
{
	string header = createHeader(request, command, false, 0);
	if (header.length() + 18 + value.length() >= ML){
		return false;
	}
		
	string message = header + "message::" + value + "::message";

	strncpy(out, message.c_str(), message.length());
	out[message.length()] = 0;
	return true;
}

//creates a header string
//header looks like this:
//"header::type:(request/answer),command: commandValue[, parted: yes, partnr: 00, parts: 00]::header"
string RobotHandler::createHeader(bool request, string command, bool parted, int parts, int partnr)
{
	string header = "header::";
	if (request)
		header += "type: request,";
	else
		header += "type: answer,";
	header += "command: " + command;
	if (parted){
		header += ", parted: yes, ";
		header += "partnr: " + to_string(partnr);
		header += ", parts: " + to_string(parts);
	}
	header += "::header";
	
	return header;
}

int RobotHandler::createSplittedMessage(char* out, string command, string value, bool request, int part, int parts)
{
	string header;
	int overhead;
	if (parts == 0)
	{
		int estimatedParts = std::ceil((double)value.length() / (double)(ML - (18 + 76)));
		header = createHeader(request, command, true, estimatedParts, part);
		cout << header.length();
		overhead = header.length() + 18;
		parts = ceil((double)value.length() / (double)(ML-overhead));
	}
	header = createHeader(request, command, true, parts, part);
	overhead = header.length() + 18;
	string valuePart = value.substr(0, ML - overhead - 1);
	string message = header + "message::" + valuePart + "::message";
	
	strncpy(out, message.c_str(), message.length());
	out[message.length()] = 0;
	return valuePart.length();
}
*/
string RobotHandler::forward(string speed)
{
	cout << "forward called!" << endl;
	return "forwarded";
	/*char message[ML]; 
	bool success = createMessage(message, "forward", to_string(speed));
	if (success)
	{
    	SendeKommando(RobotMessage, message);
	}
	else 
	{
		sendSplittedMessage(message, "connect", "IP");
	}*/
}

string RobotHandler::getPicture(string camera)
{
	cout << "getPicture called!" << endl;
	//cv::Mat picMat = GPIOLibrary::takePictureUSB(stoi(camera));
	//cv::FileStorage fs(".xml", cv::FileStorage::WRITE + cv::FileStorage::MEMORY);
	//fs << "pic" << picMat;
	//string pic = fs.releaseAndGetString();
	
	cout << "after getting string" << endl;
	return GPIOLibrary::takePictureUSB(stoi(camera));
}

string RobotHandler::getVideo(string camera)
{
	GPIOLibrary::takePictureUSB(stoi(camera));
	return 0;
}



