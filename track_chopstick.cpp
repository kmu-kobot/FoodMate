/**
@file videocapture_basic.cpp
@brief A very basic sample for using VideoCapture and VideoWriter
@author PkLab.net
@date Aug 24, 2016
*/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

Vec4i detect_chopstic(Mat img, int thr, int minLineLength, int maxLineGap)
{
	Mat img_canny;
	Canny(img, img_canny, 50, 150);

	vector<Vec4i> lines;
	HoughLinesP(img_canny, lines, 1, CV_PI / 180, thr, minLineLength, maxLineGap);

	Mat img_hough;
	img.copyTo(img_hough);

	if (lines.empty())
	{
		return NULL;
	}

	return lines[0];
}

int main(int, char**)
{
	Mat frame, blur, foregroundMask;
	//--- INITIALIZE VIDEOCAPTURE
	VideoCapture cap;
	// open the default camera using default API
	cap.open(0);
	// OR advance usage: select any API backend
	int deviceID = 0;             // 0 = open default camera
	int apiID = cv::CAP_ANY;      // 0 = autodetect default API
								  // open selected camera using selected API
	Vec4i l = NULL;
	cap.open(deviceID + apiID);
	cap.set(CAP_PROP_AUTOFOCUS, 0);
	// check if we succeeded
	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}
	Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2(0, 300, false);

	//--- GRAB AND WRITE LOOP
	cout << "Start grabbing" << endl;
	for (;;)
	{
		// wait for a new frame from camera and store it into 'frame'
		cap.read(frame);
		resize(frame, frame, Size(640, 480));
		flip(frame, frame, 1);
		GaussianBlur(frame, blur, Size(5, 5), 0);
		// check if we succeeded
		if (frame.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}
		// create foreground mask of proper size
		if (foregroundMask.empty()) {
			foregroundMask.create(frame.size(), frame.type());
		}
		bg_model->apply(blur, foregroundMask, false);
		///////////////////////////////////////////////////////////////
		Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
		morphologyEx(foregroundMask, foregroundMask, MORPH_CLOSE, kernel);
		l = detect_chopstic(foregroundMask, 80, 15, 100);
		// show live and wait for a key with timeout long enough to show images
		if (l[0] > 0 && l[1] > 0 && l[2] > 0 && l[3] > 0)
		{
			line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 255, 0), 2, 10);
			//draw dot at upper point
			int x,y = l[1] > l[3] ? l[2] : l[0], l[1] > l[3] ? l[3] : l[1]
			circle(frame,x,y), 5, Scalar(255, 0, 0), -1);
		}
		imshow("Live", frame);
		imshow("mask", foregroundMask);
		if (waitKey(5) >= 0)
			break;
	}
	return 0;
}