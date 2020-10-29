#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;




class ThTracker {

private:
	//.......... 필요한 객체들 생성
	Tracker _Tracker = Tracker();  // 젓가락 또는 숫가락을 pointing 
	Matcher _Matcher = Matcher();  // 영역에 해당하는 음식이름 반환
	Sound _Sound = Sound(); // 음성 안내 



public:

	void* do_ThTracker(const Mat& frame, vector<pair<string, Rect>>& result);

};
