#include <wiringPi.h>
#include <stdio.h>

using namespace std;

double distance(){
	digitalWrite(1, HIGH);
	delayMicroseconds(10);
	digitalWrite(1, LOW);
	
	double startTime = micros();
	double stopTime = micros();
	
	while (digitalRead(5) == LOW){
		startTime = micros();
	}
	
	while (digitalRead(5) == HIGH){
		stopTime = micros();
	}
		
	double timeElapsed = stopTime - startTime;
	
	return (timeElapsed * 34300) / 2;
}

int main (void){
	int echoPin = 5;
	int triggerPin = 1;
	wiringPiSetup();
	pinMode(triggerPin, OUTPUT);
	pinMode(echoPin, INPUT);
	
	for(;;){
		double dist = distance();
		printf("Distance: %.3f\n", dist);
		delay(1);
	}
	return 0;
}
	
