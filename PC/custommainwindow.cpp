#include "custommainwindow.h"
#include <QGridLayout>
#include <QPushButton>
#include <QSlider>
#include <QKeyEvent>
#include <QTimer>
#include<QLabel>
#include <iostream>

CustomMainWindow::CustomMainWindow(QWidget *parent) : QWidget(parent)
{
    QGridLayout *baseGrid = new QGridLayout;

    left = new QPushButton("<-");
	left->setFixedSize(60,60);
    right = new QPushButton("->");
	right->setFixedSize(60,60);
    up = new QPushButton("/\\");
	up->setFixedSize(60,60);
    down = new QPushButton("\\/");
	down->setFixedSize(60,60);

    //button->setGeometry(10,10,80,30);
    baseGrid->addWidget(left,2,0,1,1);
    baseGrid->addWidget(right,2,2,1,1);
    baseGrid->addWidget(up,1,1,1,1);
    baseGrid->addWidget(down,2,1,1,1);

    connect(left, SIGNAL (pressed()), this, SLOT (slotLeftButtonPressed()));
    connect(left, SIGNAL (released()), this, SLOT (slotButtonReleased()));
    connect(right, SIGNAL (pressed()), this, SLOT (slotRightButtonPressed()));
    connect(right, SIGNAL (released()), this, SLOT (slotButtonReleased()));
    connect(up, SIGNAL (pressed()), this, SLOT (slotUpButtonPressed()));
    connect(up, SIGNAL (released()), this, SLOT (slotButtonReleased()));
    connect(down, SIGNAL (pressed()), this, SLOT (slotDownButtonPressed()));
    connect(down, SIGNAL (released()), this, SLOT (slotButtonReleased()));
	
	QGridLayout *distanceGrid = new QGridLayout;
	
	distanceSlider = new QSlider(Qt::Vertical);
	distanceSlider->setRange(0,50);
	distanceSlider->setValue(50);
	distanceSlider->setTickPosition(QSlider::TicksRight);
	distanceSlider->setTickInterval(5);
	
	distanceGrid->addWidget(distanceSlider,1,0,1,1);
	
	distanceTimer = new QTimer();
	distanceTimer->setInterval(100);
	
	connect(distanceTimer, SIGNAL(timeout()), this, SLOT(slotGetDistance()));
	connect(this, SIGNAL(newDistance(int)), distanceSlider, SLOT(setValue(int)));
	
	QLabel *distanceText = new QLabel("Distance");
	distanceGrid->addWidget(distanceText,0,0,1,1);
	
	baseGrid->addLayout(distanceGrid,1,3,3,1);
			
	this->setLayout(baseGrid);
	distanceTimer->start();

    rob.connect("192.168.1.1");
}

CustomMainWindow::~CustomMainWindow()
{
	distanceTimer->stop();
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

void CustomMainWindow::slotGetDistance()
{
	int dist = rob.getProximitySensor(0);
	emit newDistance(dist);
}

void CustomMainWindow::keyPressEvent( QKeyEvent *key)
{
	if (!(key->isAutoRepeat())){
		switch (key->key()){
			case Qt::Key_W:
				slotUpButtonPressed();
				up->setDown(true);
				break;
			case Qt::Key_S:
				slotDownButtonPressed();
				down->setDown(true);
				break;
			case Qt::Key_D:
				slotRightButtonPressed();
				right->setDown(true);
				break;
			case Qt::Key_A:
				slotLeftButtonPressed();
				left->setDown(true);
				break;
		}
	}
}

void CustomMainWindow::keyReleaseEvent( QKeyEvent *key)
{
	if (!key->isAutoRepeat()){
		slotButtonReleased();
		switch (key->key()){
			case Qt::Key_W:
				up->setDown(false);
				break;
			case Qt::Key_S:
				down->setDown(false);
				break;
			case Qt::Key_D:
				right->setDown(false);
				break;
			case Qt::Key_A:
				left->setDown(false);
				break;
		}
	}
}
