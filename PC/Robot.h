//
// Created by stephan on 13.06.17.
//

#ifndef PC_ROBOT_H

#include <string>
using std::string;

#include <valarray>
using std::valarray;

#include "MessageListener.h"
#define PC_ROBOT_H

#include <thread>
using std::thread;


class Robot {
public:
	enum {leftMotor = 1, rightMotor = 2};

    Robot() = default;
	std::list<valarray<valarray<valarray<int>>>> VideoBuffer;
    virtual ~Robot() = default;

    //connect this PC with the Robot who has ip
	virtual bool connect(string ip);
	virtual bool closeConnection();

    //motor
    virtual bool forward(int speed);
    virtual bool backward(int speed);
	virtual bool stop(int motor = 0);
    virtual bool setSingleMotorSpeed(int motor, int speed);
    virtual bool setLeftNRightMotor(int leftSpeed, int rightSpeed);
    virtual int getSingleMotorSpeed(int motor);
//    virtual int getMotorPosition(int motor);

    //for steppermotor
//    virtual void setSingleMotorSteps(int motor, int steps);
    //for servomotor
//    virtual void setSingleMotorAngle(int motor, int angle);

    //LEDs
//    virtual void setLED(int led, bool state);

    //camera
    virtual valarray<valarray<valarray<int>>> getPicture(int camera = 0);
    virtual std::list<valarray<valarray<valarray<int>>>>* startVideo(int rows, int cols);
	virtual bool stopVideo();

    //audio
//    virtual void playSound(string filePath);
//    virtual int getMicrophone(int microphone, SensorMode mode = single);

    //sensors
    virtual double getProximitySensor(int sensor);
//    virtual int getCompass();
//    virtual int getLightSensor(int sensor);
//    virtual int getAccelerometer();

	//communication
private:
    char RobotMessage[6] = "Robot";
    MessageListener listener;
	thread videoThread;
	bool stopVideoThread = false;
	void receiveVideo();
    static valarray<valarray<valarray<int>>> convertStringToMat(string s);
};


#endif //PC_ROBOT_H
