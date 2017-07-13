#ifndef RPI_RobotHandler_H

#include <string>
using std::string;
#include <iostream>

#define RPI_RobotHandler_H


class RobotHandler {
public:
    enum SensorMode{ single, continous};
    RobotHandler();
    virtual ~RobotHandler();
 //   virtual void reset();
    //connect this PC with the Robot who has ip
	virtual void connect(string ip, int port = 5001);
	virtual void closeConnection();

    //motor
    virtual void forward(int speed);
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
private:
	bool createMessage(char* out, string command, string value);
	void sendSplittedMessage(char* out, string command, string value);
	char RobotMessage[6];
};


#endif //RPI_RobotHandler_H
