#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;




class Tracking {
private:
	Mat blur, skin_mask, foreground_mask, img_canny;
	//차영상 background 설정(history = -1, threshold = 400, shadow = false) 
	Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2(-1, 400, false);
	vector<Vec4i> lines;
	vector<Vec3f> circles;
	Mat img_YCrCb;
	Mat mask;
public:


	//젓가락 좌표 반환하는 함수
	Vec4i detect_chopstic(Mat img, int thr, int minLineLength, int maxLineGap);

	//숟가락 좌표 반환하는 함수
	Vec3f detect_spoon(Mat img, int minDistance, int thr);

	//살색 지우는 함수
	Mat make_mask_image(Mat img);

	//tracking point 젓가락의 좌표 반환 _______ 이것만 사용!!
	Point2i tracking_point(Mat frame);
};
