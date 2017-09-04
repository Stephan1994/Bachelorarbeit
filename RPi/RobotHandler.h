#ifndef RPI_RobotHandler_H
#define RPI_RobotHandler_H

#include <string>
#include <cstring>
using std::string;
#include <iostream>
#include <map>
#include <functional>
#include "Netzwerk/ProzessPi.h"
#include "Camera.h"


class RobotHandler {
public:
	std::map<string, std::function<string(string)>> RobotFunctions;
   // RobotHandler(MessageWriter *extWriter);
   // virtual ~RobotHandler();
	virtual string connect(string value);
	virtual string closeConnection(string value);

    //motor
    virtual string forward(string speed) = 0;
	virtual string backward(string speed) = 0;
	virtual string stop(string parameter) = 0;
    virtual string setSingleMotorSpeed(string parameter) = 0;
    virtual string setLeftNRightMotor(string parameter) = 0;
    virtual string getSingleMotorSpeed(string motor) = 0;
//    virtual int getMotorPosition(int motor);

    //for steppermotor
//    virtual void setSingleMotorSteps(int motor, int steps);
    //for servomotor
//    virtual void setSingleMotorAngle(int motor, int angle);

    //LEDs
//    virtual void setLED(int led, bool state);

    //camera
    virtual string getPicture(string camera = "0") = 0;
    virtual string startVideo(string camera = "0") = 0;
	virtual string stopVideo(string camera = "0") = 0;
	
    //audio
//    virtual void playSound(string filePath);
//    virtual int getMicrophone(int microphone, SensorMode mode = single);

    //sensors
    virtual string getProximitySensor(string sensor = "0") = 0;
//    virtual int getCompass();
//    virtual int getLightSensor(int sensor);
//    virtual int getAccelerometer();

	//communication
	virtual void initFunctions();
	Camera camera;
};


#endif //RPI_RobotHandler_H
