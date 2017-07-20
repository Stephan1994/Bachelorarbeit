//
// Created by stephan on 17.07.17.
//

#ifndef PC_MESSAGELISTENER_H
#define PC_MESSAGELISTENER_H
#include <thread>
using std::thread;
#include <string>
using std::string;
#include "Netzwerk/ProzessPi.h"
#include <iostream>
#include <future>
using std::future;
using std::promise;
#include <list>
using std::list;

using std::cout;
using std::endl;

class MessageListener {
private:
    struct Message{
        Message() : transferFailure(false), request(true) {}
        bool transferFailure;
        bool request;
        string command;
        string value;
    };

    struct Listener{
        promise<string> prom;
        string requestedCommand;

        Listener() {}
        Listener(const Listener&) = delete;

        Listener(Listener&& source)
                : prom(std::move(source.prom)),
                  requestedCommand(std::move(source.requestedCommand))
        {}
    };

    list<Message> incoming;
    list<Listener> listeners;
    thread listeningThread;
    bool stop = false;

    void listening();
    Message extractHeader(string value);

public:
    future<string> addListener(string command);
    void stopListening();
    void initListening();
};


#endif //PC_MESSAGELISTENER_H
