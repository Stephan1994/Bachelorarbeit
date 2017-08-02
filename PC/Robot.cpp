//
// Created by Stephan on 13.06.17.
//

#include "Robot.h"

#include "ProtocolLibrary.h"
//MessageLength
//#define ML 8192
#include <iostream>
#include <cstring>
#include "Netzwerk/ProzessPiClient.h"

using std::cout;
using std::endl;
using std::to_string;

//baseclass for robots. Should be the parent of future robots

Robot::Robot()
{
    strncpy(RobotMessage, "Robot", sizeof(RobotMessage));
	cout << "RobotMessage: " << RobotMessage << endl;
}

Robot::~Robot() = default;

bool Robot::connect(string ip)
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
	if (!ProtocolLibrary::createMessage(message, "connect", "IP"))
        return false;

    SendeKommando(RobotMessage, message);

    listener.initListening();
    future<string> answerFuture = listener.addListener("connect");

    string answer = answerFuture.get();

    return answer == "connected";
}

bool Robot::closeConnection()
{
    char message[ML];
    if (ProtocolLibrary::createMessage(message, "disconnect", "IP"))
        SendeKommando(RobotMessage, message);

    future<string> answerFuture = listener.addListener("disconnect");
    string answer = answerFuture.get();
    if (answer == "disconnected"){
        listener.stopListening();
        done();
        return true;
    }
    return false;
}
/*
bool Robot::createMessage(char* out, string command, string value, bool request)
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

string Robot::createHeader(bool request, string command, bool parted, int parts, int partnr)
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

int Robot::createSplittedMessage(char* out, string command, string value, bool request, int part, int parts)
{
    string header;
    int overhead;
    if (parts == 0)
    {
        auto estimatedParts = (int) ceil(value.length() / (ML - (18 + 76)));
        header = Robot::createHeader(request, command, true, estimatedParts, part);
        cout << header.length();
        overhead = (int) header.length() + 18;
        parts = (int) ceil(value.length() / (ML-overhead));
    }
    header = Robot::createHeader(request, command, true, parts, part);
    overhead = (int) header.length() + 18;
    string valuePart = value.substr(0, (unsigned long) (ML - overhead - 1));
    string message = header + "message::" + valuePart + "::message";

    strncpy(out, message.c_str(), message.length());
    out[message.length()] = 0;
    return (int) valuePart.length();
}
*/
void Robot::forward(int speed)
{
	char message[ML];
	if (ProtocolLibrary::createMessage(message, "forward", to_string(speed)))
    	SendeKommando(RobotMessage, message);


}