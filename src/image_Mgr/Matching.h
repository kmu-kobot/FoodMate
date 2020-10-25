#pragma

class Matching{

    public:
        Matching();
        std::string match_food(cv::Point pt, std::vector<std::pair<std::string, cv::Rect>> matching_result);
};
