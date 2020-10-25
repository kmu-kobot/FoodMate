#pragma


struct board_obj{
    cv::Point center_board;
    cv::Mat crop_imgs;
};

struct frgm_obj{
    std::vector<cv::Mat> crop_imgs; // 잘린 이미지를 담는곳
    std::vector<cv::Rect> crop_Rects; // 잘린 이미지의 위치를 담는곳 
};


class Board{
    private:
        int thresh = 10;


    public:
        
        Board();

        cv::Mat img_preproces(cv::Mat src);
        board_obj get_target_area(cv::Mat src);
        frgm_obj frgm_board(cv::Mat src);
};

