#pragma once
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;


// image를 받아서 Tracking하는 -> matching Thread


class Consumer2 {

public:

	void* consumer_doing(const Mat& frame, vector<pair<string, Rect>>& result);

};
