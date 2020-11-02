#pragma


struct board_obj{
    cv::Point center_board;
    cv::Mat crop_imgs;
};

struct frgm_obj{
    std::vector<cv::Mat> crop_imgs; // 잘린 이미지를 담는곳
    std::vector<cv::Rect> crop_Rects; // 잘린 이미지의 위치를 담는곳 
};

typedef struct{
    int x = 5;
    int y = 1;
    int w = 2;
    int h = 3;
}PT_data;



class Board{
    private:
        int thresh = 25;

    public:

        Board();
        cv::Mat img_preproces(cv::Mat src);
        void get_target_area();
        void frgm_board();
        float rectArea(cv::Rect rect);
        int get_crop_Rects_size();
        int get_center_board_x();
        int get_center_board_y();
        int get_crop_Rects_x(int idex);
        int get_crop_Rects_y(int idex);
        int get_crop_Rects_w(int idex);
        int get_crop_Rects_h(int idex);

};
