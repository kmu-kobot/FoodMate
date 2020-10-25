#pragma
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>


struct board_obj;  // board와 관련된 구조체 
struct frgm_obj;  // fragment와 관련된 구조체


class Board {
private:
    int thresh = 10;
    Point pre_point = Point(0, 0);  // 식판의 이전 위치 
    Point crnt_point; // 식판의 현재 위치 

public:
    Board();

    cv::Mat img_preproces(cv::Mat src);
    cv::Mat get_target_area(cv::Mat src); // 식판 영역
    std::vector<cv::Mat> frgm_board(cv::Mat src); // 식판내의 구분된 영역들 모음
};