#pragma
class Filter{
    private:
        int thresh = 10;

    public:
        Filter();
        int check_right(cv::Mat src);

};

