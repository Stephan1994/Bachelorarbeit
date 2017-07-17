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

bool Robot::connect(string ip, int port)
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

    char recvdValue[10000];
    EmpfangeRobotKommando(recvdValue);

    string recvdString(recvdValue);
    Message mes = extractHeader(recvdString);
    if (mes.request == false && mes.transferFailure == false){
        if (mes.command.compare("connect") == 0){
            if (mes.value.compare("connected") == 0){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            //Todo: behandeln von commands, die nicht zu dieser Funktion gehören
            return false;
        }
    }
    else{
        //send repeat request
        return false;
    }
}

void Robot::closeConnection()
{
    char message[ML];
    if (createMessage(message, "disconnect", "IP"))
    {
        SendeKommando(RobotMessage, message);
    }
    else
    {
        sendSplittedMessage(message, "disconnect", "IP");
    }
    done();
}

bool Robot::createMessage(char* out, string command, string value)
{
	string message = "start_request_" + command + ":" + value + ":" + "end_request_" + command;
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

//extracts information from package to struct Message and tests for transfer failures
Message Robot::extractHeader(string value)
{
    Message mes;
    size_t pos = value.find("_");
    if (value.substr(0,pos).compare("start") == 0)
    {
        value = value.substr(pos+1);
        pos = value.find("_");
        if (value.substr(0,pos).compare("request") == 0)
        {
            mes.request = true;
        }
        else if (value.substr(0,pos).compare("answer") == 0)
        {
            mes.request = false;
        }
        else
        {
            mes.transferFailure = true;
            //sendrepeatrequest()
            cout << "send repeat request not implemented yet" << endl;
        }

        value = value.substr(pos+1);
        pos = value.find(":");
        mes.command = value.substr(0, pos);
        value = value.substr(pos+1);
        pos = value.find(":");
        mes.value = value.substr(0, pos);
        value = value.substr(pos+1);

        //test if end is without failure
        string testStr = "end_";
        testStr += ((mes.request) ? "request_" : "answer_") + mes.command + ";";
        if (value.compare(testStr) != 0)
        {
            mes.transferFailure = true;
            //sendrepeatrequest()
            cout << "send repeat request not implemented yet" << endl;
        }
    }
    else
    {
        mes.transferFailure = true;
        //sendrepeatrequest()
        cout << "send repeat request not implemented yet" << endl;
    }

    return mes;
}


