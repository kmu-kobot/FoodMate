#include "Tracker.h"  
#include "Matcher.h"
#include "ThTracker.h"
#include "Sound.h"
#include <iostream>
using namespace cv;
using namespace std;




void* ThTracker::do_ThTracker(const Mat& frame, vector<pair<string, Rect> >& result) {

    //############# 받은 이미지를 이용하여 ###################
    // .......1. 젓가락 포인터를 좌표를 구한다.
    Point center = _Tracker.track_point(frame);

    // .......2. 구한 좌표가 특정 음식 영역 어디에 있나 확인한다.
    string  answer = _Matcher.match_food(center, result);

    // .......3. 나온 결과를 음성으로 안내한다.
    _Sound.play_sound(answer);
    cout << "결과: " << answer << endl;

}
