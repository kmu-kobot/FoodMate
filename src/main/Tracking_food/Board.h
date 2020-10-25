#pragma
#include "opencv2/opencv.hpp"


struct board_obj {
    cv::Point board_center;
    cv::Mat board_img;
};

struct frgm_obj {
    std::vector<cv::Mat> crop_imgs; // 잘린 이미지를 담는곳
    std::vector<cv::Rect> crop_Rects; // 잘린 이미지의 위치를 담는곳 
};


class Board {
private:
    int thresh = 10;
   

public:

    Board();


    cv::Point pre_point = cv::Point(0, 0);  // 식판의 이전 위치 
    cv::Point crnt_point; // 식판의 현재 위치 

    cv::Mat img_preproces(cv::Mat src);
    board_obj get_target_area(cv::Mat src);
    frgm_obj frgm_board(cv::Mat src);
};

