#include <iostream>
#include <iomanip>
#include <thread>
#include "RobotHandler.h"
#include "Netzwerk/ProzessPi.h"
#include "ProtocolLibrary.h"

using std::cout;
using std::endl;

using namespace std;

bool handleRequest(string command, string value);
bool handleAnswer(string command, string value);

int main() 
{
	char recvdValue[10000];
	init((char *)"192.168.1.1", (char *) "5001");
	
	while(true)
	{
		printf("Warte auf Kommando\n");
		EmpfangeRobotKommando(recvdValue);
		
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
			
			if (messageHandled == false){}
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
			cout << "Splitted Message is needed!" << endl;
			int parts = 0, part = 1;
			while (answerValue.length() >= ML-200){

				std::fill(answer, answer + sizeof(answer)/sizeof(answer[0]), 0);
				int writtenValueChars = ProtocolLibrary::createSplittedMessage(answer, command, answerValue, false, parts, part);
				answerValue = answerValue.substr(writtenValueChars);
				if (parts == 0){
					char partsNr[10];
					std::fill(partsNr, partsNr + sizeof(partsNr)/sizeof(partsNr[0]), 0);
					ProtocolLibrary::extractHeaderFieldValue(answer, partsNr, (char *)"parts");
					sscanf(partsNr, "%d", &parts);
					cout << "PartsStr: " << partsNr << "PartsInt: " << parts << endl;
				}

				SendeKommando(handler.RobotMessage, answer);
				part++;
				std::this_thread::sleep_for(std::chrono::microseconds(10));
			}
			cout << "After While" << endl;
			if (answerValue.length() >= 0){
				std::fill(answer, answer + sizeof(answer)/sizeof(answer[0]), 0);
				ProtocolLibrary::createSplittedMessage(answer, command, answerValue, false, parts, part);
				cout << "After creating last message!" << endl;
				SendeKommando(handler.RobotMessage, answer);
				cout << "After sending last message!" << endl;
			}
		}		
			
	}
	else
	{
		cout << "This command is unknown." << endl;
		return false;
	}
	return true;
}

bool handleAnswer(string command, string value)
{
	cout << "handleAnswer not implemented yet" << endl;
	return true;
}

