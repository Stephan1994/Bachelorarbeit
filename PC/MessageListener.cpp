//
// Created by Stephan on 17.07.17.
// MessageListener:
// This class runs a thread to get new Messages from network. Listeners could be added like in Observer pattern but
// as functions.
// With this class it's possible to do parallel execution of functions in Robot class and every function still gets
// its messages.
//

#include "MessageListener.h"

//loops to get a new message every time there's one available
void MessageListener::listening() {
    fd_set receiveSet;
    struct timeval timer;
    //start loop, stop gets set by function "stopListening"
    while(!stop){
        //init the receiveSet
        FD_ZERO(&receiveSet);
        int fdEmpfangen = fileno(fd_empfangen);
        FD_SET(fdEmpfangen, &receiveSet);

        //set timer to 2 secs
        timer = {2,0};

        //waits until something is available in fd_empfangen or until timer runs out
        int ret = select(FD_SETSIZE, &receiveSet, NULL, NULL, &timer);

        //if something is available it gets read and added to the incoming queue
        if(FD_ISSET(fdEmpfangen, &receiveSet) && ret != -1 && !stop){
            char recvdValue[10000];
            //get new message
            EmpfangeRobotKommando(recvdValue);
            //convert it to string
            string recvdString(recvdValue);
            MessageListener::Message mes = extractHeader(recvdString);
            //add the new message to incoming queue
            if (!mes.transferFailure)
                incoming.push_back(mes);
        }

        //if someone is listening, the incoming queue gets searched for the command the listener waits for
        // and the promise is set if found
        if (!listeners.empty() && !incoming.empty()){
            list<MessageListener::Listener>::iterator listener_it = listeners.begin();
            //go through all listeners
            while (listener_it != listeners.end()){
                list<MessageListener::Message>::iterator message_it = incoming.begin();

                //search for command in all queued messages
                while((*message_it).command.compare((*listener_it).requestedCommand) != 0 && message_it != incoming.end()){
                    ++message_it;
                }
                //if command is found, set promise and erase message and listener from lists
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
    newListener.requestedCommand = command;

    //adds the listener to the list
    listeners.push_back(std::move(newListener));

    return listeners.back().prom.get_future();
}

//extracts information from package to struct Message and tests for transfer failures
MessageListener::Message MessageListener::extractHeader(string value)
{
    MessageListener::Message mes;
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

//starts the listening Thread, now commands can be received
void MessageListener::initListening() {
    listeningThread = thread(&MessageListener::listening, this);
}

//stops the listening Thread and waits until it's finished
void MessageListener::stopListening() {
    stop = true;
    listeningThread.join();
}