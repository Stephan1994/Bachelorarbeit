#include "custommainwindow.h"
#include <QGridLayout>
#include <QPushButton>

CustomMainWindow::CustomMainWindow(QWidget *parent) : QWidget(parent)
{
    QGridLayout *baseGrid = new QGridLayout;

    left = new QPushButton("<-");
    right = new QPushButton("->");
    up = new QPushButton("/\\");
    down = new QPushButton("\\/");

    //button->setGeometry(10,10,80,30);

    baseGrid->addWidget(left,2,0,1,1);
    baseGrid->addWidget(right,2,2,1,1);
    baseGrid->addWidget(up,1,1,1,1);
    baseGrid->addWidget(down,2,1,1,1);

    this->setLayout(baseGrid);

    rob.connect("192.168.1.1");

    connect(left, SIGNAL (pressed()), this, SLOT (slotLeftButtonPressed()));
    connect(left, SIGNAL (released()), this, SLOT (slotButtonReleased()));
    connect(right, SIGNAL (pressed()), this, SLOT (slotRightButtonPressed()));
    connect(right, SIGNAL (released()), this, SLOT (slotButtonReleased()));
    connect(up, SIGNAL (pressed()), this, SLOT (slotUpButtonPressed()));
    connect(up, SIGNAL (released()), this, SLOT (slotButtonReleased()));
    connect(down, SIGNAL (pressed()), this, SLOT (slotDownButtonPressed()));
    connect(down, SIGNAL (released()), this, SLOT (slotButtonReleased()));
}

void CustomMainWindow::slotLeftButtonPressed()
{
    rob.setSingleMotorSpeed(Robot::rightMotor, 200);
}

void CustomMainWindow::slotRightButtonPressed()
{
    rob.setSingleMotorSpeed(Robot::leftMotor, 200);
}

void CustomMainWindow::slotUpButtonPressed()
{
    rob.forward(200);
}

void CustomMainWindow::slotDownButtonPressed()
{
    rob.backward(200);
}

void CustomMainWindow::slotButtonReleased()
{
    rob.stop();
}

void CustomMainWindow::slotDisconnect()
{
    rob.closeConnection();
}
