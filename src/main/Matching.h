#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;

class Matching {

public:
	void showimage(const cv::Mat& img);

	string matchFood(Point2i x, Point2i y);

};