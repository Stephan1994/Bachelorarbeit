#ifndef RPI_RobotHandler_H
#define RPI_RobotHandler_H

#include <string>
#include <string.h>
using std::string;
#include <iostream>
#include <map>
#include <functional>
#include "Netzwerk/ProzessPi.h"


class RobotHandler {
public:
    enum SensorMode{ single, continous};
	std::map<string, std::function<string(string)>> RobotFunctions;
    RobotHandler();
    virtual ~RobotHandler();
 //   virtual void reset();
	virtual string connect(string value);
	virtual string closeConnection(string value);

    //motor
    virtual string forward(string speed);
//    virtual void backward(int speed);
//    virtual void setSingleMotorSpeed(int motor, int speed);
//    virtual void setCurveSpeed(int leftSpeed, int rightSpeed);
//    virtual int getSingleMotorSpeed(int motor);
//    virtual int getMotorPosition(int motor);

    //for steppermotor
//    virtual void setSingleMotorSteps(int motor, int steps);
    //for servomotor
//    virtual void setSingleMotorAngle(int motor, int angle);

    //LEDs
//    virtual void setLED(int led, bool state);

    //camera
//    virtual int getPicture(int camera = 0);
//    virtual int getVideo(int camera = 0);

    //audio
//    virtual void playSound(string filePath);
//    virtual int getMicrophone(int microphone, SensorMode mode = single);

    //sensors
//    virtual int getProximitySensor(int sensor);
//    virtual int getCompass();
//    virtual int getLightSensor(int sensor);
//    virtual int getAccelerometer();

	//communication
	bool createMessage(char* out, string command, string value, bool request = true);
	void sendSplittedMessage(char* out, string command, string value);
	char RobotMessage[6];
	void initFunctions();
};


#endif //RPI_RobotHandler_H
