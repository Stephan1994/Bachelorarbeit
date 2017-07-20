#include <iostream>
#include <iomanip>
#include "RobotHandler.h"
#include "Netzwerk/ProzessPi.h"

//MessageLength
#define ML 1056

using std::cout;
using std::endl;

using namespace std;

struct Message{
	Message() : transferFailure(false), request(true) {}
	bool transferFailure;
	bool request;
	string command;
	string value;
};

Message extractHeader(string value);
bool handleRequest(string command, string value);
bool handleAnswer(string command, string value);

int main() 
{
	char recvdValue[10000];
	init("192.168.1.1", "5001");
	while(true)
	{
		EmpfangeRobotKommando(recvdValue);
		cout << "Value received: " << recvdValue << endl;
		string recvdString(recvdValue);
		Message message;
		message = extractHeader(recvdValue);

		cout << "Actual Message:" << endl;
		cout << "\t" << "failure? " << (message.transferFailure ? "Yes" : "No") << endl;
		cout << "\t" << "request? " << (message.request ? "Yes" : "No") << endl;
		cout << "\t" << "command: " << message.command << endl;
		cout << "\t" << "value: " << message.value << endl;
		
		if (message.transferFailure == false)
		{
			bool messageHandled = false;
			if (message.request)
				messageHandled = handleRequest(message.command, message.value);
			else
				messageHandled = handleAnswer(message.command, message.value);
		}
	}
	
    return 0;
}

//extracts information from package to struct Message and tests for transfer failures
Message extractHeader(string value)
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

bool handleRequest(string command, string value)
{
	RobotHandler handler;
	if (handler.RobotFunctions.count(command) > 0)
	{
		string answerValue = handler.RobotFunctions[command](value);
		char answer[ML];
		handler.createMessage(answer, command, answerValue, false);
		SendeKommando(handler.RobotMessage, answer);
	}
	else
	{
		cout << "This command is unknown." << endl;
	}
	return true;
}

bool handleAnswer(string command, string value)
{
	cout << "handleAnswer not implemented yet" << endl;
	return true;
}

