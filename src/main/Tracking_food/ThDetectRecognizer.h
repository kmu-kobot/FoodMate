#pragma once
#include "SocketClient.h"
#include "Board.h"
#include "Sound.h"
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;


class ThDetectRecognizer {
private:
SocketClient _client = SocketClient();
Board _board = Board(); 
Sound _sound = Sound();

public:
	ThDetectRecognizer();
	~ThDetectRecognizer();	
	
	void* do_ThDetectRecognizer(const Mat& frame, vector<pair<string, Rect>>& matched_result);
};

