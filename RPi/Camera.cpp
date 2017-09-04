#include "Camera.h"
#include "ProtocolLibrary.h"

#include <iostream>
using std::cout;
using std::endl;
using std::to_string;

using cv::Vec3b;

bool Camera::initCamera(MessageWriter *extWriter, int camNr)
{
	writer = extWriter;
	return cam.open(camNr);
}

Camera::~Camera()
{
	if (cam.isOpened()){
		cam.release();
	}
}

string Camera::takePicture()
{
	Mat cameraFrame = Mat(640, 480, CV_64F, 0.0);
	if (cam.isOpened()){
		cam.read(cameraFrame);
	}
	
	return convertMatToString(cameraFrame);
}

bool Camera::startStreaming(int cols, int rows)
{
	Rows = rows;
	Cols = cols;
	cam.set(CV_CAP_PROP_FRAME_WIDTH, cols);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT, rows);
	streamingThread = thread(&Camera::streaming, this);
	actStreaming = true;
	return true;
}

bool Camera::stopStreaming()
{
	cout << "stopStreming called" << endl;
	stop = true;
	streamingThread.join();
	actStreaming = false;
	return true;
}

void Camera::streaming()
{
	cout << "started streaming" << endl;
	while(!stop && cam.isOpened()){
		string picture = Camera::takePicture();
		int parts = 0, part = 1;
		char answer[ML];
		string command = "video";
		while (picture.length() >= ML-200){
			std::fill(answer, answer + sizeof(answer)/sizeof(answer[0]), 0);
			int writtenValueChars = ProtocolLibrary::createSplittedMessage(answer, command, picture, false, parts, part);
			picture = picture.substr(writtenValueChars);
			if (parts == 0){
				char partsNr[10];
				std::fill(partsNr, partsNr + sizeof(partsNr)/sizeof(partsNr[0]), 0);
				
				ProtocolLibrary::extractHeaderFieldValue(answer, partsNr, (char *)"parts");
				sscanf(partsNr, "%d", &parts);
				//cout << "PartsStr: " << partsNr << "PartsInt: " << parts << endl;
			}
			string answerString(answer);
			writer->addMessage(answerString);
			
			part++;
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}
		//cout << "After While" << endl;
		if (picture.length() >= 0){
			std::fill(answer, answer + sizeof(answer)/sizeof(answer[0]), 0);
			//answer.clear();
			parts = part;
			ProtocolLibrary::createSplittedMessage(answer, command, picture, false, parts, part);
			//cout << "After creating last message!" << endl;
			//SendeKommando(handler.RobotMessage, answer);
			string answerString(answer);
			writer->addMessage(answerString);
			//cout << "After sending last message!" << endl;
		}
	}
	cout << "stopped streaming" << endl;
}

//creates a string out of a cv::Mat matrix
//shape of this string is without '(':
//colsxrows|(bgr-values of one pixel divided by ',')(bgr-values of one pixel divided by ',')...|...|...(bgr-values of one pixel divided by ',')|end
string Camera::convertMatToString(Mat mat) {
	string output = to_string(mat.cols) + "x" + to_string(mat.rows) + "|";
	for(int row=0; row < mat.rows; row++){
		for (int col = 0; col < mat.cols; col++) {
			output += "(";
			Vec3b bgrVector = mat.at<Vec3b>(row, col);
			for (int rgb=0; rgb < 3; rgb++){
				uchar posValue = bgrVector[rgb];
				output += to_string(posValue);
				output += ",";
			}
			output += ")";
		}
		output += "|";
	}
	output += "end";
	return output;
}
	