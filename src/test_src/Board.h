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
        int thresh = 25;
        board_obj my_board_obj;
        frgm_obj my_frgm_obj;


    public:
        
        Board();

        cv::Mat img_preproces(cv::Mat src);
        void get_target_area();
        void frgm_board();
        float rectArea(cv::Rect rect);
        cv::Rect rect_return(); 
        cv::Point point_return();
};
