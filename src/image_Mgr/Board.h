#pragma
#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>


class Board {
private:
    int thresh = 10;
    Point center_board; // 식판의 중심 좌표 (center_board.x, center_board.y)

public:
    Board();

    cv::Mat img_preproces(cv::Mat src);
    cv::Mat get_target_area(cv::Mat src); // 식판 영역
    std::vector<cv::Mat> frgm_board(cv::Mat src); // 식판내의 구분된 영역들 모음
};
