//
// Created by stephan on 24.08.17.
//

#ifndef PC_MESSAGEWRITER_H
#define PC_MESSAGEWRITER_H
#include <thread>
using std::thread;
#include <string>
using std::string;
#include <list>
using std::list;

#include "ProtocolLibrary.h"

class MessageWriter {
private:
	char RobotMessage[6];
    list<string> incoming;
    thread writingThread;
    bool stop = false;

    void writing();

public:
	int test = 8;
	MessageWriter();
    bool addMessage(string Mes);
    void stopWriting();
    void initWriting();
};


#endif //PC_MESSAGEWRITER_H
