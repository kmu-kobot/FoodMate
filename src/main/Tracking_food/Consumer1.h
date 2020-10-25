#pragma once
using namespace cv;
using namespace std;
#include "opencv2/opencv.hpp"
// image¸¦ ¹Þ¾Æ¼­ TrackingÇÏ´Â -> matching Thread



class Consumer1 {

public:
	
	
	void* consumer_doing(const Mat& frame, vector<pair<string, Rect>>& v);
};

