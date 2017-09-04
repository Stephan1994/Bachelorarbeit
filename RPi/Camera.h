//
// Created by stephan on 24.08.17.
//

#ifndef PC_CAMERA_H
#define PC_CAMERA_H
#include <thread>
using std::thread;
#include <string>
using std::string;
#include <list>
using std::list;

#include <opencv2/opencv.hpp>
using cv::Mat;
using cv::VideoCapture;

#include "ProtocolLibrary.h"
#include "MessageWriter.h"

class Camera {
private:
    thread streamingThread;
    bool stop = false;
	int Rows;
	int Cols;
	VideoCapture cam;
	MessageWriter *writer;
    string convertMatToString(Mat mat);
	void streaming();

public:
	bool actStreaming = false;

	~Camera();
	bool initCamera(MessageWriter *extWriter, int camNr);
    bool startStreaming(int cols, int rows);
	bool stopStreaming();
	string takePicture();
};


#endif //PC_CAMERA_H
