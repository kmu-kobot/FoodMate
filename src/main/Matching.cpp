#include "Matching.h"

using namespace cv;
using namespace std;

void Matching::showimage(const Mat& img) {
	imshow("frame", img);
	cout << "hello";
}


// 젓가락의 좌표를 받아 어느 지정된 영역인지 matching하여 음식명을 말한다.
string Matching::matchFood(Point2i x, Point2i y) {


	string answer = "";
	return answer;
}
