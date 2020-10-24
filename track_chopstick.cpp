
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

	if (lines.empty())
	{
		return NULL;
	}

	return lines[0];
}

Vec3f detect_spoon(Mat img, int minDistance, int thr)
{
	Mat img_canny;
	Canny(img, img_canny, 50, 150);
	imshow("canny", img_canny);
	vector<Vec3f> circles;
	HoughCircles(img_canny, circles, HOUGH_GRADIENT, 1, minDistance, thr, thr / 2, 10, 50);

	if (circles.empty())
	{
		return NULL;
	}

	return circles[0];
}

Mat make_mask_image(Mat frame)
{
	Mat img_YCrCb;
	Mat mask;
	cvtColor(frame, img_YCrCb, COLOR_BGR2YCrCb);
	inRange(img_YCrCb, Scalar(0, 133, 77), Scalar(255, 173, 127), mask);
	//vector<Mat> planes;
	//split(img_YCrCb, planes);
	//Mat mask = (128 < planes[1]) & (planes[1] < 170) & (73 < planes[2]) & (planes[2] < 158);

	bitwise_not(mask, mask);
	return mask;
}

int main(int, char**)
{
	Mat frame, blur, mask_img, foregroundMask;
	//--- INITIALIZE VIDEOCAPTURE
	VideoCapture cap;
	// open the default camera using default API
	cap.open(0);
	// OR advance usage: select any API backend
	int deviceID = 0;             // 0 = open default camera
	int apiID = cv::CAP_ANY;      // 0 = autodetect default API
								  // open selected camera using selected API

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


		mask_img = make_mask_image(frame);
		imshow("mask_img", mask_img);
		bg_model->apply(blur, foregroundMask, false);
		///////////////////////////////////////////////////////////////
		Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5), Point(-1, -1));
		morphologyEx(foregroundMask, foregroundMask, MORPH_CLOSE, kernel);


		bitwise_and(mask_img, foregroundMask, foregroundMask);
		medianBlur(foregroundMask, foregroundMask, 9);
		imshow("mask", foregroundMask);

		//detect chopstick
		Vec4i l = NULL;
		l = detect_chopstic(foregroundMask, 70, 15, 100);
		// show live and wait for a key with timeout long enough to show images
		if (l[0] > 0 && l[1] > 0 && l[2] > 0 && l[3] > 0)
		{
			line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 255, 0), 2, 10);
			//draw dot at upper point
			int x = l[1] > l[3] ? l[2] : l[0];
			int y = l[1] > l[3] ? l[3] : l[1];
			circle(frame, Point(x, y), 5, Scalar(255, 0, 0), -1);
		}
		//detect spoon

		Vec3f c = NULL;
		c = detect_spoon(foregroundMask, 10, 30);
		if (c[0] > 0 && c[1] > 0 && c[2] > 0)
		{
			circle(frame, Point(c[0], c[1]), c[2], Scalar(0, 255, 0), 2);
			circle(frame, Point(c[0], c[1]), 2, Scalar(255, 0, 0), -1);
		}

		imshow("Live", frame);
		imshow("mask", foregroundMask);
		if (waitKey(5) >= 0)
			break;
	}
	return 0;
}
