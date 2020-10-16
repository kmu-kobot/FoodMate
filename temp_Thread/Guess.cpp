#include <iostream>
#include <opencv2/opencv.hpp>
#include "Guess.h"

using namespace cv;
using namespace std;

void Guess::showimage(const Mat& img) {
	imshow("frame", img);
	cout << "hello";
}
