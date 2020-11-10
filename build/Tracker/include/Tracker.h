///////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2020 KOBOT kobot9@naver.com.
// All rights reserved.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE
//
//////////////////////////////////////////////////////////////////////////////////


#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;


class Tracker {
private:
Mat blur, skin_mask, foreground_mask, img_canny;
	//차영상 background 설정(history = -1, threshold = 400, shadow = false) 
	Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2(-1, 400, false);
	vector<Vec4i> lines;
	vector<Vec3f> circles;
	Vec4i last_line;
	Mat img_YCrCb;
	Mat mask;
	int button = 13;

public:
	Tracker();
	//젓가락 좌표 반환하는 함수
	Vec4i detect_chopstic(Mat img, int thr, int minLineLength, int maxLineGap);
	//숟가락 좌표 반환하는 함수
	Vec3f detect_spoon(Mat img, int minDistance, int thr);
	//살색 지우는 함수
	Mat make_mask_image(Mat img);
	//tracking point 젓가락의 좌표 반환 _______ 이것만 사용!!
	void track_point();

	int get_track_point_x();
	int get_track_point_y();
};
