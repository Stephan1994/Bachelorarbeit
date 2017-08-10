//
// Created by Stephan on 17.07.17.
// MessageListener:
// This class runs a thread to get new Messages from network. Listeners could be added like in Observer pattern but
// as functions.
// With this class it's possible to do parallel execution of functions in Robot class and every function still gets
// its messages.
//

#include "MessageListener.h"

#include <iostream>
using std::cout;
using std::endl;

#include "Netzwerk/ProzessPiClient.h"

//loops to get a new message every time there's one available
void MessageListener::listening() {
    fd_set receiveSet{};
    struct timeval timer{};
    //start loop, stop gets set by function "stopListening"
    while(!stop){
        //init the receiveSet
        FD_ZERO(&receiveSet);
        int fdEmpfangen = fileno(fd_empfangen);
        FD_SET(fdEmpfangen, &receiveSet);

        //set timer to 2 secs
        timer = {2,0};

        //waits until something is available in fd_empfangen or until timer runs out
        int ret = select(FD_SETSIZE, &receiveSet, nullptr, nullptr, &timer);

        //if something is available it gets read and added to the incoming queue
        //if the message is splitted the function collects all parts and then add it to incoming queue
        if(FD_ISSET(fdEmpfangen, &receiveSet) && ret != -1 && !stop){
            char recvdValue[10000];
            std::fill(recvdValue, recvdValue + 10000, 0);
            //get new message
            EmpfangeRobotKommando(recvdValue);

            //convert it to string
            string recvdString(recvdValue);
            ProtocolLibrary::Message mes = ProtocolLibrary::extractHeader(recvdString);

            //test if catched message is a splitted
            if (mes.parted && !mes.transferFailure){
                //add to buffer if nothing exists there until now
                if (partedNotFinished.empty()){
                    partedNotFinished.push_back(mes);
                }
                else{
                    //go through the splitted messages that been catched
                    auto parted_it = partedNotFinished.begin();
                    bool inserted = false;
                    while (parted_it != partedNotFinished.end()){
                        if ((*parted_it).command == mes.command){
                            //add value und change actual part number to the message that fits
                            if((*parted_it).parts == mes.parts && (*parted_it).part == mes.part - 1){
                                (*parted_it).part = mes.part;
                                (*parted_it).value += mes.value;
                                //if all parts were added, the message is added to incoming queue
                                if ((*parted_it).part == (*parted_it).parts){
                                    incoming.push_back(*parted_it);
                                    partedNotFinished.erase(parted_it);
                                }
                                inserted = true;
                                break;
                            }
                        }
                        ++parted_it;
                    }
                    //if no message fits to the incoming one it gets added at the end of the queue
                    if (!inserted){
                        partedNotFinished.push_back(mes);
                    }
                }
            }
            //add the new message to incoming queue
            else if (!mes.transferFailure)
                incoming.push_back(mes);
        }

        //if someone is listening, the incoming queue gets searched for the command the listener waits for
        // and the promise is set if found
        if (!listeners.empty() && !incoming.empty()){
            auto listener_it = listeners.begin();
            //go through all listeners
            while (listener_it != listeners.end()){
                auto message_it = incoming.begin();

                //search for command in all queued messages
                while((*message_it).command != (*listener_it).requestedCommand && message_it != incoming.end()){
                    ++message_it;
                }
                //if command is found, set promise and erase message and listener from lists if message is not
                //seperated
                if(message_it != incoming.end()){
                    (*listener_it).prom.set_value((*message_it).value);
                    incoming.erase(message_it);
                    listener_it = listeners.erase(listener_it);
                }
                else{
                    ++listener_it;
                }
            }
        }
    }
}

//adds a listener to the list, returns the future<string> that should be waited for
future<string> MessageListener::addListener(string command){
    //init the new Listener with promise and given command
    MessageListener::Listener newListener;
    newListener.prom = promise<string>();
    newListener.requestedCommand = std::move(command);

    //adds the listener to the list
    listeners.push_back(std::move(newListener));

    return listeners.back().prom.get_future();
}

//starts the listening Thread, now commands can be received
void MessageListener::initListening() {
    listeningThread = thread(&MessageListener::listening, this);
}

//stops the listening Thread and waits until it's finished
void MessageListener::stopListening() {
    stop = true;
    listeningThread.join();
}