#pragma once
#include "SocketClient.h"
#include "Board.h"
#include "Sound.h"
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

// image¸¦ ¹Þ¾Æ¼­ TrackingÇÏ´Â -> matching Thread



class Consumer1 {
private:
SocketClient client = SocketClient();
Board board = Board(); 
Sound sound = Sound();

public:
	Consumer1();
	~Consumer1();	
	
	void* consumer_doing(const Mat& frame, vector<pair<string, Rect>>& v);
};

