#pragma once
using namespace std;
using namespace cv;


class Matcher{

public:
    Matcher();
    string match_food(Point pt, vector<pair<string, Rect>> matched_result);
};

