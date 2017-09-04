#include "MessageWriter.h"
#include "Netzwerk/ProzessPi.h"
#include <cstring>

#include <iostream>
using std::cout;
using std::endl;

MessageWriter::MessageWriter()
	:incoming()
{
	strncpy(RobotMessage, "Robot", sizeof(RobotMessage));
}

//loops to get a new message every time there's one available
void MessageWriter::writing() {
	
    //start loop, stop gets set by function "stopWriting"
    while(!stop){
       if (!incoming.empty())
	   {
		   //printf("have messages to send\n");
		   auto message_it = incoming.begin();
		   while (message_it != incoming.end()){
			   char out[ML];
				strncpy(out, (*message_it).c_str(), (*message_it).length());
				out[(*message_it).length()] = 0;
				SendeKommando(RobotMessage,out);
				//cout << out << endl;
				message_it = incoming.erase(message_it);
				//++message_it;
		   }
	   }
    }
}

//adds a listener to the list, returns the future<string> that should be waited for
bool MessageWriter::addMessage(string mes){
	incoming.push_back(mes);

    return true;
}

//starts the writing Thread, now commands can be received
void MessageWriter::initWriting() {
    writingThread = thread(&MessageWriter::writing, this);
}

//stops the listening Thread and waits until it's finished
void MessageWriter::stopWriting() {
    stop = true;
    writingThread.join();
}