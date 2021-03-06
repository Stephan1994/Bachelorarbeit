//
// Created by Stephan on 13.06.17.
//

#include <iostream>
#include <cstring>
#include "Robot.h"

#include "ProtocolLibrary.h"

#include "Netzwerk/ProzessPiClient.h"

using std::cout;
using std::endl;
using std::to_string;

//baseclass for robots. Should be the parent of future robots

bool Robot::connect(string ip) {
    //convert ip string to char*
    char ipArray[16];
    strncpy(ipArray, ip.c_str(), sizeof(ipArray));
    ipArray[sizeof(ipArray) - 1] = 0;
    //convert port string to char*
    string portstring = to_string(5001);
    char portArray[5];
    strncpy(portArray, portstring.c_str(), sizeof(portArray));
    portArray[sizeof(portArray) - 1] = 0;
    //connect to the robot with help of extern Netzwerk-Library
    init(ipArray, portArray);


    //ToDo maybe get ipaddress
    char message[ML];
    if (!ProtocolLibrary::createMessage(message, "connect", ""))
        return false;

    SendeKommando(RobotMessage, message);

    listener.initListening();

    future<string> answerFuture = listener.addListener("connect");

    string answer = answerFuture.get();

    return answer == "connected";
}

bool Robot::closeConnection() {
    char message[ML];
    if (ProtocolLibrary::createMessage(message, "disconnect", ""))
        SendeKommando(RobotMessage, message);

    future<string> answerFuture = listener.addListener("disconnect");
    string answer = answerFuture.get();
    if (answer == "disconnected") {
        listener.stopListening();
        done();
        return true;
    }
    return false;
}

bool Robot::forward(int speed) {
    char message[ML];
    if (ProtocolLibrary::createMessage(message, "forward", to_string(speed)))
        SendeKommando(RobotMessage, message);

    future<string> answerFuture = listener.addListener("forward");
    string answer = answerFuture.get();

    return answer == "forwarded";
}

bool Robot::backward(int speed) {
    char message[ML];
    if (ProtocolLibrary::createMessage(message, "backward", to_string(speed)))
        SendeKommando(RobotMessage, message);

    future<string> answerFuture = listener.addListener("backward");
    string answer = answerFuture.get();

    return answer == "backwarded";
}

bool Robot::stop(int motor) {
    char message[ML];
    string value;
    if (motor == 0)
        value = "";
    else
        value = to_string(motor);
    if (ProtocolLibrary::createMessage(message, "stop", value))
        SendeKommando(RobotMessage, message);

    future<string> answerFuture = listener.addListener("stop");
    string answer = answerFuture.get();

    return answer == "stopped";
}

bool Robot::setSingleMotorSpeed(int motor, int speed) {
    char message[ML];
    string value = to_string(motor) + "," + to_string(speed);
    if (ProtocolLibrary::createMessage(message, "setSingleMotor", value))
        SendeKommando(RobotMessage, message);

    future<string> answerFuture = listener.addListener("setSingleMotor");
    string answer = answerFuture.get();

    return answer == "set";
}

int Robot::getSingleMotorSpeed(int motor) {
    char message[ML];
    string value = to_string(motor);
    if (ProtocolLibrary::createMessage(message, "getSingleMotor", value))
        SendeKommando(RobotMessage, message);

    future<string> answerFuture = listener.addListener("getSingleMotor");
    string answer = answerFuture.get();

    return std::stoi(answer);
}

bool Robot::setLeftNRightMotor(int leftSpeed, int rightSpeed){
    char message[ML];
    string value = to_string(leftSpeed) + "," + to_string(rightSpeed);
    if (ProtocolLibrary::createMessage(message, "setLeftNRight", value))
        SendeKommando(RobotMessage, message);

    future<string> answerFuture = listener.addListener("setLeftNRight");
    string answer = answerFuture.get();

    return answer == "set";
}

valarray<valarray<valarray<int>>> Robot::getPicture(int camera) {
    char message[ML];
    if (ProtocolLibrary::createMessage(message, "picture", to_string(camera)))
        SendeKommando(RobotMessage, message);

    future<string> answerFuture = listener.addListener("picture");
    string answer = answerFuture.get();

    return Robot::convertStringToMat(answer);
}

double Robot::getProximitySensor(int sensor){
    char message[ML];
    if (ProtocolLibrary::createMessage(message, "distance", to_string(sensor)))
        SendeKommando(RobotMessage, message);

    future<string> answerFuture = listener.addListener("distance");
    string answer = answerFuture.get();

    return std::stod(answer);
}

std::list<valarray<valarray<valarray<int>>>>* Robot::startVideo(int cols, int rows) {
    char message[ML];
    if (ProtocolLibrary::createMessage(message, "startVideo", to_string(cols) + "x" + to_string(rows)))
        SendeKommando(RobotMessage, message);

    future<string> answerFuture = listener.addListener("startVideo");
    string answer = answerFuture.get();


    videoThread = thread(&Robot::receiveVideo, this);

    return &VideoBuffer;
}

bool Robot::stopVideo() {
    char message[ML];
    if (ProtocolLibrary::createMessage(message, "stopVideo", ""))
        SendeKommando(RobotMessage, message);

    future<string> answerFuture = listener.addListener("stopVideo");
    string answer = answerFuture.get();

    cout << "after getting answer for stopping video." << endl;
    stopVideoThread = true;
    videoThread.join();
    VideoBuffer.clear();
}

void Robot::receiveVideo(){

    while(!stopVideoThread){
        future<string> answerFuture = listener.addListener("video");
        string answer = answerFuture.get();

        int i = 0;
        string colStr;
        while (answer[i] != 'x') {
            colStr += answer[i];
            i++;
        }
        int cols = stoi(colStr);
        string rowStr;
        i++;
        while (answer[i] != '|') {
            rowStr += answer[i];
            i++;
        }
        int rows = stoi(rowStr);

        valarray<valarray<valarray<int>>> array((size_t) cols);
        try{
            array = convertStringToMat(answer);
        }catch (...){
            cout << "received wrong formatted picture." << endl;
            for (int j = 0; j < cols; j++) {
                array[j].resize((size_t) rows);
                for (int k = 0; k < rows; k++) {
                    array[j][k].resize((size_t) 3, 0);
                }
            }
        }

        if (VideoBuffer.size() < 10){
            VideoBuffer.push_back(array);
        }
        else{
            VideoBuffer.pop_front();
            VideoBuffer.push_back(array);
        }
    }
}

valarray<valarray<valarray<int>>> Robot::convertStringToMat(string s) {
    int i = 0;
    string colStr;
    while (s[i] != 'x') {
        colStr += s[i];
        i++;
    }
    int cols = stoi(colStr);
    string rowStr;
    i++;
    while (s[i] != '|') {
        rowStr += s[i];
        i++;
    }
    int rows = stoi(rowStr);

    int row = 0, col = 0;
    i++;
    valarray<valarray<valarray<int>>> array((size_t) cols);
    for (int j = 0; j < cols; j++) {
        array[j].resize((size_t) rows);
        for (int k = 0; k < rows; k++) {
            array[j][k].resize((size_t) 3);
        }
    }

    int rgb = 0;
    while (s[i] != 'e' && s[i + 1] != 'n' && s[i + 2] != 'd' && i < ((int)(s.length()) - 3)) {
        while (s[i] != '|') {
            if (s[i] == '(') {
                rgb = 0;
            } else if (s[i] == ')') {
                col++;
            } else {
                string number;
                while (s[i] != ',') {
                    number += s[i];
                    i++;
                }
                array[col][row][rgb] = stoi(number);
                rgb++;
            }
            i++;
        }
        row++;
        col = 0;
        i++;
    }
    return array;
}
