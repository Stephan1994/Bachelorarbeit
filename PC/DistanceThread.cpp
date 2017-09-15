#include "DistanceThread.h"
#include <iostream>

void DistanceThread::getDistance(Robot *rob)
{
    while(!stopDistance){
		int dist = rob->getProximitySensor(0);
		emit newDistance(dist);
	}
}

void DistanceThread::stopDistanceThread()
{
	std::cout <<"stopDistance called" << std::endl;
	stopDistance = true;
}