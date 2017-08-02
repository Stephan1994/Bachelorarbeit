#include "GPIOLibrary.h"

#include <iostream>
using std::cout;
using std::endl;
using std::to_string;

using cv::VideoCapture;
using cv::Vec3b;

//takes a picture from usb-webcam and returns a string containing the information
string GPIOLibrary::takePictureUSB(int cam)
{
	VideoCapture camera(cam);
	
	if (!camera.isOpened()){
		cout << "cannot open camera!" << endl;
		return nullptr;
	}
	Mat cameraFrame;
	camera.read(cameraFrame);
	
	return GPIOLibrary::convertMatToString(cameraFrame);
}

//creates a string out of a cv::Mat matrix
//shape of this string is without '(':
//colsxrows|(bgr-values of one pixel divided by ',')(bgr-values of one pixel divided by ',')...|...|...(bgr-values of one pixel divided by ',')|end
string GPIOLibrary::convertMatToString(Mat mat) {
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