#ifndef DistanceThread_H
#define DistanceThread_H

#include "Robot.h"
#include <QThread>

class DistanceThread : public QObject
{
    Q_OBJECT
    QThread distanceThread;
	bool stopDistance = false;
	
	public slots:
		void getDistance(Robot *rob);
		void stopDistanceThread();
	signals:
		void newDistance(int distance);

};

#endif //DistanceThread_H