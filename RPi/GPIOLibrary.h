
#ifndef GPIOLIB_H
#define GPIOLIB_H

#include <opencv2/opencv.hpp>
using std::string;
using cv::Mat;
class GPIOLibrary
{
	public:
		static string takePictureUSB(int cam = 0);
	private:
		static string convertMatToString(Mat mat);
};

#endif //GPIOLIB_H