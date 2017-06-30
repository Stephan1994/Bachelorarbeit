#include <iostream>
#include <iomanip>
#include "Netzwerk/ProzessPi.h"

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

Message analyzeHeader(string value);

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
		message = analyzeHeader(recvdValue);
		cout << "Actual Message:" << endl;
		cout << "\t" << "failure? " << (message.transferFailure ? "Yes" : "No") << endl;
		cout << "\t" << "request? " << (message.request ? "Yes" : "No") << endl;
		cout << "\t" << "command: " << message.command << endl;
		cout << "\t" << "value: " << message.value << endl;
	}
	
    return 0;
}

//extracts information from package to struct Message and tests for transfer failures
Message analyzeHeader(string value)
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
