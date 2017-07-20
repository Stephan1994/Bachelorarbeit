#include "RobotHandler.h"

//MessageLength
#define ML 1056

using std::cout;
using std::endl;
using std::to_string;
using std::map;
using std::placeholders::_1;

//baseclass for robotHandler. Should be the parent of future handlers



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
			{"forward", std::bind(&RobotHandler::forward, this, _1)},
			{"connect", std::bind(&RobotHandler::connect, this, _1)},
			{"disconnect", std::bind(&RobotHandler::closeConnection, this, _1)}
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

bool RobotHandler::createMessage(char* out, string command, string value, bool request)
{
	string message;
	if (request)
		message = "start_request_" + command + ":" + value + ":" + "end_request_" + command;
	else
		message = "start_answer_" + command + ":" + value + ":" + "end_answer_" + command;
	
	if (message.length() >= ML)
	{
		return false;
	}	

	strncpy(out, message.c_str(), message.length());
	cout << out << endl;
    	out[message.length()] = 0;
	cout << out << endl;
	return true;
}

void RobotHandler::sendSplittedMessage(char* out, string command, string value)
{
	cout << "SplittedMessage not implemented yet!" << endl;
}

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



