//
// Created by stephan on 17.07.17.
//

#ifndef PC_MESSAGELISTENER_H
#define PC_MESSAGELISTENER_H
#include <thread>
using std::thread;
#include <string>
using std::string;
#include <future>
using std::future;
using std::promise;
#include <list>
using std::list;

#include "ProtocolLibrary.h"

class MessageListener {
private:
   /* struct Message{
        bool transferFailure{false};
        bool request{true};
        string command;
        string value;
        bool parted;
        int parts;
        int part;
    };*/

    struct Listener{
        promise<string> prom;
        string requestedCommand;

        Listener() = default;

        Listener(const Listener&) = delete;

        Listener(Listener&& source)
                : prom(std::move(source.prom)),
                  requestedCommand(std::move(source.requestedCommand))
        {}
    };

    list<ProtocolLibrary::Message> incoming;
    list<Listener> listeners;
    list<ProtocolLibrary::Message> partedNotFinished;
    thread listeningThread;
    bool stop = false;

    void listening();
    //Message extractHeader(string value);

public:
    future<string> addListener(string command);
    void stopListening();
    void initListening();
};


#endif //PC_MESSAGELISTENER_H
