#include "Tracking.h"  
#include "Matching.h"
#include "Consumer2.h"

using namespace cv;

//.......... 필요한 객체들 생성
Tracking tracker = Tracking();  // 젓가락 또는 숫가락을 pointing 
Matching matcher = Matching();  // 영역에 해당하는 음식이름 반환





void* Consumer2::consumer_doing(const Mat& frame, vector<pair<string, Rect> >& result) {


    //############# 받은 이미지를 이용하여 ###################

      // .......1. 젓가락 포인터를 좌표를 구한다.
    Point center = tracker.tracking_point(frame);

    // .......2. 구한 좌표가 특정 음식 영역 어디에 있나 확인한다.
    string  answer = matcher.match_food(center, result);


    // .......3. 나온 결과를 음성으로 안내한다.



}
