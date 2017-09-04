#ifndef RPI_ExampleHandler_H
#define RPI_ExampleHandler_H

#include "RobotHandler.h"


class ExampleHandler : public RobotHandler {
	int SerialFd;
public:	
	ExampleHandler(MessageWriter *extWriter);
    //motor
    string forward(string speed);
    string backward(string speed);
	string stop(string parameter);
	string setSingleMotorSpeed(string parameter);
    string setLeftNRightMotor(string parameter);
    string getSingleMotorSpeed(string motor);
//    virtual int getMotorPosition(int motor);

    //for steppermotor
//    virtual void setSingleMotorSteps(int motor, int steps);
    //for servomotor
//    virtual void setSingleMotorAngle(int motor, int angle);

    //LEDs
//    virtual void setLED(int led, bool state);

    //camera
    string getPicture(string camera = "0");
    string startVideo(string camera = "0");
	string stopVideo(string camera = "0");
	
    //audio
//    virtual void playSound(string filePath);
//    virtual int getMicrophone(int microphone, SensorMode mode = single);

    //sensors
	string getProximitySensor(string sensor = "0");
//    virtual int getCompass();
//    virtual int getLightSensor(int sensor);
//    virtual int getAccelerometer();

	//communication
	void initFunctions();
};


#endif //RPI_ExampleHandler_H
