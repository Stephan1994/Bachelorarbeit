#include <iostream>
#include <iomanip>
#include "RobotHandler.h"
#include "Netzwerk/ProzessPi.h"
#include "ProtocolLibrary.h"

//MessageLength
//#define ML 1056

using std::cout;
using std::endl;

using namespace std;

/*struct Message{
	Message() : transferFailure(false), request(true) {}
	bool transferFailure;
	bool request;
	string command;
	string value;
};*/

bool handleRequest(string command, string value);
bool handleAnswer(string command, string value);

int main() 
{
	char recvdValue[10000];
	init((char *)"192.168.1.1", (char *) "5001");
	
	while(true)
	{
		//handleRequest("camera", "test");
		printf("Warte auf Kommando");
		EmpfangeRobotKommando(recvdValue);
		cout << "Value received: " << recvdValue << endl;
		string recvdString(recvdValue);
		ProtocolLibrary::Message message;
		message = ProtocolLibrary::extractHeader(recvdValue);

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

bool handleRequest(string command, string value)
{
	RobotHandler handler;
	if (handler.RobotFunctions.count(command) > 0)
	{
		string answerValue = handler.RobotFunctions[command](value);
		char answer[ML];
		if (ProtocolLibrary::createMessage(answer, command, answerValue, false)){
			SendeKommando(handler.RobotMessage, answer);
		}
		else{
			int parts = 0, part = 1;
			while (answerValue.length() >= ML-200){
				std::fill(answer, answer + sizeof(answer)/sizeof(answer[0]), 0);
				int writtenValueChars = ProtocolLibrary::createSplittedMessage(answer, command, answerValue, false, parts, part);
				answerValue = answerValue.substr(writtenValueChars-1);
				if (parts == 0){
					char partsNr[5];
					ProtocolLibrary::extractHeaderFieldValue(answer, partsNr, (char *)"parts");
					sscanf(partsNr, "%d", &parts);
				}
				SendeKommando(handler.RobotMessage, answer);
				part++;
				usleep(10);
			}
			if (answerValue.length() >= 0){
				std::fill(answer, answer + sizeof(answer)/sizeof(answer[0]), 0);
				ProtocolLibrary::createSplittedMessage(answer, command, answerValue, false, parts, part);
				SendeKommando(handler.RobotMessage, answer);
			}
		}		
			
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

