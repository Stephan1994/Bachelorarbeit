//
// Created by stephan on 13.06.17.
//

#include "Robot.h"
#include "Netzwerk/ProzessPi.h"

//MessageLength
#define ML 1056

using std::cout;
using std::endl;
using std::to_string;

//baseclass for robots. Should be the parent of future robots



Robot::Robot()
{
	char robArray[6];
    strncpy(RobotMessage, "Robot", sizeof(RobotMessage));
    robArray[sizeof(RobotMessage) - 1] = 0;
	cout << "RobotMessage: " << RobotMessage << endl;
}

Robot::~Robot(){}

void Robot::connect(string ip, int port)
{
    //convert ip string to char*
    char ipArray[16];
    strncpy(ipArray, ip.c_str(), sizeof(ipArray));
    ipArray[sizeof(ipArray) - 1] = 0;
	//convert port string to char*
	string portstring = to_string(5001);
    char portArray[5];
    strncpy(portArray, portstring.c_str(), sizeof(portArray));
    portArray[sizeof(portArray) - 1] = 0;
	cout << "RobotMessage: " << RobotMessage << endl;
	//connect to the robot with help of extern Netzwerk-Library
    init(ipArray, portArray);
	

    //ToDo maybe get ipaddress
	char message[ML]; 
	bool success = createMessage(message, "connect", "IP");
	if (success)
	{
    	SendeKommando(RobotMessage, message);
	}
	else 
	{
		sendSplittedMessage(message, "connect", "IP");
	}
}

bool Robot::createMessage(char* out, string command, string value)
{
	string message = "start_question_" + command + ":" + value + ":" + "end_question_" + command;
	if (message.length() >= ML)
	{
		return false;
	}	

	strncpy(out, message.c_str(), message.length());
    //out[message.length() - 1] = 0;
	return true;
}

void Robot::sendSplittedMessage(char* out, string command, string value)
{
	cout << "SplittedMessage not implemented yet!" << endl;
}

void Robot::forward(int speed)
{
	char message[ML]; 
	bool success = createMessage(message, "forward", to_string(speed));
	if (success)
	{
    	SendeKommando(RobotMessage, message);
	}
	else 
	{
		sendSplittedMessage(message, "connect", "IP");
	}
}


