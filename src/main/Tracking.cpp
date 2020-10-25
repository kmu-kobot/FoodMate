#include "Tracking.h"





//젓가락 좌표 반환하는 함수
Vec4i Tracking::detect_chopstic(Mat img, int thr, int minLineLength, int maxLineGap)
{
	lines.clear();
	HoughLinesP(img, lines, 1, CV_PI / 180, thr, minLineLength, maxLineGap);
	if (lines.empty())
	{
		return NULL;
	}
	return lines[0];
}


//숟가락 좌표 반환하는 함수
Vec3f Tracking::detect_spoon(Mat img, int minDistance, int thr)
{
	circles.clear();
	HoughCircles(img, circles, HOUGH_GRADIENT, 1, minDistance, thr, thr / 2, 10, 70);
	if (circles.empty())
	{
		return NULL;
	}

	return circles[0];
}


//살색 지우는 함수
Mat Tracking::make_mask_image(Mat img)
{
	cvtColor(img, img_YCrCb, COLOR_BGR2YCrCb);
	inRange(img_YCrCb, Scalar(0, 133, 77), Scalar(255, 173, 127), mask);
	bitwise_not(mask, mask);
	return mask;
}


//tracking point 젓가락의 좌표 반환
Point Tracking::tracking_point(Mat frame)
{
	if (frame.empty())
	{
		cout << "Could not open camera :(" << endl;
	}

	//frame 영상 전처리(640x480, 좌우반전, 3x3 가우시안 블러)
	resize(frame, frame, Size(640, 480));
	flip(frame, frame, 1);
	GaussianBlur(frame, blur, Size(5, 5), 0);

	//초기 차영상 마스크 Mat 생성
	if (foreground_mask.empty())
	{
		foreground_mask.create(frame.size(), frame.type());
	}

	// 1.차영상 마스크 씌우고 노이즈 제거(9x9 미디언 필터)
	bg_model->apply(blur, foreground_mask);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(9, 9), Point(-1, -1));
	morphologyEx(foreground_mask, foreground_mask, MORPH_CLOSE, kernel);
	medianBlur(foreground_mask, foreground_mask, 5);

	// 2.차영상에서 살색제거 후 skin_mask에 저장
	copyTo(frame, skin_mask, foreground_mask);
	skin_mask = make_mask_image(frame);

	// 3.식기만 남은 영상 이진화
	bitwise_and(skin_mask, foreground_mask, foreground_mask);

	// 4.이진화 영상에 Canny 적용
	Canny(foreground_mask, img_canny, 50, 150);
	//디버깅용
	imshow("canny_img", img_canny);

	// 5.숟가락을 찾으면 바로 반환하고, 숟가락이 없으면 젓가락 좌표 반환
	//숟가락 포인트
	Vec3f s = NULL;
	s = detect_spoon(img_canny, 10, 50);
	if (s[0] > 0 && s[1] > 0 && s[2] > 0)
	{
		circle(frame, Point(s[0], s[1]), s[2], Scalar(0, 255, 0), 2);
		//circle(frame, Point(s[0], s[1]), 2, Scalar(255, 0, 0), -1);
		return Point(s[0], s[1]);
	}

	//젓가락 포인트
	Vec4i c = NULL;
	c = detect_chopstic(img_canny, 70, 15, 100);
	if (c[0] > 0 && c[1] > 0 && c[2] > 0 && c[3] > 0)
	{
		line(frame, Point(c[0], c[1]), Point(c[2], c[3]), Scalar(0, 255, 0), 2, 10);
		//영상에서 더 위에있는 포인트 반환
		return Point(c[1] > c[3] ? c[2] : c[0], c[1] > c[3] ? c[3] : c[1]);
		//circle(frame, Point(x, y), 5, Scalar(255, 0, 0), -1);
	}
}




int main()
{
	Mat frame;
	Point x, y;
	VideoCapture cap;
	Tracking t = Tracking();
	cap.open(0);
	cap.set(CAP_PROP_AUTOFOCUS, 0);

	while (true)
	{
		cap.read(frame);
		flip(frame, frame, 1);
		(x, y) = t.tracking_point(frame);
		circle(frame, (x, y), 5, Scalar(255, 0, 0), -1);
		imshow("frame", frame);
		if (waitKey(5) >= 0)
			break;
	}
	return 0;
}
