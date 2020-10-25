#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>
#include "Filter.h"

using namespace cv;
using namespace std;


Filter::Filter(){}



int Filter::check_right(Mat src)
{
    //이미지 전처리
    Mat src_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));

    //canny함수를 이용하여 이미지의 윤곽추출
    Mat canny_output;
    Canny(src_gray, canny_output, thresh, thresh * 2);

    //이미지 안에서 contours를 찾음
    vector<vector<Point>> contours;
    findContours(canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector<vector<Point>> contours_poly(contours.size());
    int count = 0;

    //일정 크기 이하의 contour를 제거함
    for (size_t i = 0; i < contours.size(); i++)
    {
        approxPolyDP(contours[i], contours_poly[i], 20, true);

        if (contourArea(contours_poly[i]) < 600 || contourArea(contours_poly[i]) > 2000){
            ++count;
        }  
    }
    return count;
}
