#include<iostream>
#include "opencv2/opencv.hpp"
#include"Matching.h"
#include<string>
#include<utility>


using namespace cv;
using namespace std;


Matching::Matching() {}




string Matching::match_food(Point pt, vector<pair<string, Rect>> matching_result) {
    string answer;
    int rect_x, rect_y, rect_w, rect_h;
    answer = "no match";

    for (int i = 0; i < matching_result.size(); ++i) {
        rect_x = matching_result[i].second.x;
        rect_y = matching_result[i].second.y;
        rect_w = matching_result[i].second.width;
        rect_h = matching_result[i].second.height;
        if ((rect_x < pt.x && pt.x < rect_x + rect_w) && (rect_y < pt.y && pt.y < rect_y + rect_h)) {
            answer = matching_result[i].first;
            break;
        }

    }
    cout << "x point: " << pt.x << endl << "y point: " << pt.y << endl << "answer: " << answer << endl;
    //sound
    
    return answer;
}
