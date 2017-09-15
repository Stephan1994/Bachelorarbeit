#include <iostream>
#include "Robot.h"
#include <unistd.h>
#include <sys/time.h>
#include "custommainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QGridLayout>

using namespace std;
int main(int argc, char **argv) {
    QApplication app (argc, argv);

    CustomMainWindow *window = new CustomMainWindow();
    window->setWindowTitle("RobotGui");
    window->setFixedSize(480, 250);


    window->show();

    QObject::connect(&app, SIGNAL(aboutToQuit()), window, SLOT(slotDisconnect()));
/*
    Robot rob;
    bool connected = rob.connect("192.168.1.1");
    if (connected)
        cout << "connected" << endl;
    else
        cout << "not connected" << endl;

    valarray<valarray<valarray<int>>> test;
    struct timeval start, end;
    gettimeofday(&start, NULL);
     test =  rob.getPicture(0);
    gettimeofday(&end, NULL);

    double microseconds = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) /1000;
    cout << "Reaktionszeit: " << microseconds << endl;
    cout << "Daten: " << test.size() * test[0].size() * test[0][0].size() * sizeof(int)<< endl;

    rob.stop();
    rob.closeConnection();
*/
  /*  double distance = rob.getProximitySensor(0);
    cout << "distance: " << distance << endl;
	rob.forward(200);
    sleep(2);
    rob.stop();

    rob.setLeftNRightMotor(200, 0);
    sleep(2);
    rob.stop(Robot::leftMotor);

    rob.setSingleMotorSpeed(Robot::rightMotor, 200);
    sleep(2);
    rob.stop(Robot::rightMotor);
*/
	//cout << "forwarded" << endl;

    //rob.getPicture(0);
    /*rob.startVideo(320, 176);
    int i = 0;
    valarray<valarray<valarray<int>>> pic;
    while(i < 10){
        if (!rob.VideoBuffer.empty()){
            pic = *rob.VideoBuffer.begin();
            rob.VideoBuffer.pop_front();
            i++;
            cout << "Picture received" << endl;
        }
    }
    cout << "After video" << endl;
    rob.stopVideo();*/
    //valarray<valarray<valarray<int>>> pic;
    //pic = rob.getPicture();
    //cout << pic << endl;
    //rob.closeConnection();
    return app.exec();
    //return 0;
}
