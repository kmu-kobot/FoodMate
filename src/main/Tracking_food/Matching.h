#pragma
using namespace std;
using namespace cv;


class Matching {

public:
    Matching();
    string match_food(Point pt, vector<pair<string, Rect>> matching_result);
};

