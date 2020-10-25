#pragma



class Board{
    private:
        int thresh;

    public:
        Board();

        cv::Mat img_preproces(cv::Mat src);
        cv::Mat get_target_area(cv::Mat src);
        std::vector<cv::Mat> thresh_callback(cv::Mat src);
};

