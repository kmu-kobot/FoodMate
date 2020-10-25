#pragma once
using namespace cv;


// image를 받아서 Tracking하는 -> matching Thread



class Consumer1 {

public:
	
	
	void* consumer_doing(const Mat& frame, vector<pair<string, Rect>>& v);
};
