#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>
#include "Board.h"

using namespace cv;
using namespace std;


Board::Board() {}



float Board::rectArea(Rect rect) {
    float area;
    //사각형 영역의 넓이를 구함
    area = rect.width * rect.height;
    return area;
}

Mat Board::img_preproces(Mat src)
{
    //이미지 전처리 과정
    //이미지 흑백으로 전환후 노이즈 감소를 위한 블러처리
    Mat src_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, src_gray, Size(3, 3));

    //canny함수를 이용하여 이미지의 윤곽추출
    Mat canny_output;
    Canny(src_gray, canny_output, thresh, thresh * 2);

    return canny_output;
}

board_obj Board::get_target_area(Mat src)
{

    board_obj my_board_obj;
    //이미지에서 식판의 영역만을 추출
    Mat img_pre = img_preproces(src);

    //끊어진 윤곽선을 잇기 위해 canny edge에서 구한 point들을 확장
    Mat mask = getStructuringElement(MORPH_RECT, Size(2, 2), Point(1, 1));
    Mat dilate_edge;
    dilate(img_pre, dilate_edge, mask, Point(-1, -1), 1);

    //edge를 확장한 이미지에서 contour를 찾음
    vector<vector<Point>> contours;
    findContours(dilate_edge, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

    vector<vector<Point>> contours_poly(contours.size());
    Rect boundRect;

    vector<Point> biggest_area = contours[0];

    //이미지에서 가장 큰 윤곽선을 찾음
    for (size_t i = 0; i < contours.size(); i++)
    {
        approxPolyDP(contours[i], contours_poly[i], 20, true);

        if (contourArea(contours_poly[i]) > contourArea(biggest_area))
        {
            biggest_area = contours_poly[i];
        }
    }

    //위에서 찾은 가장큰 윤곽선을 사각형으로 전환
    boundRect = boundingRect(biggest_area);
    rectangle(src, boundRect.tl(), boundRect.br(), (0, 0, 255), 2);

    // 식판의 중심을 구한다.
    my_board_obj.board_center = Point((boundRect.x + boundRect.width) / 2, (boundRect.y + boundRect.height) / 2);


    //사각형을 관심영역으로 하고 이미지를 crop
    my_board_obj.board_img = src(boundRect);
   // imshow("Contours", my_board_obj.board_img);
   // waitKey();

    return my_board_obj;
}

frgm_obj Board::frgm_board(Mat src)
{

    
    // 0.......이미지 전처리
    Mat img_pre = img_preproces(src);

    Mat mask = getStructuringElement(MORPH_RECT, Size(2, 2), Point(1, 1));
    Mat dilate_edge;
    dilate(img_pre, dilate_edge, mask, Point(-1, -1), 2);
    vector<vector<Point>> contours;  //이미지 안에서 contours를 찾음
    findContours(dilate_edge, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

   // imshow("Contours", dilate_edge);
   // waitKey();


    vector<vector<Point>> contours_poly(contours.size());
    vector<Rect> boundRect;
    Rect temp_rect;

    //일정 크기 이하의 contour를 제거함
    int target_area = src.size().width * src.size().height; // 식판 전체 크기
    for (size_t i = 0; i < contours.size(); i++)
    {
        approxPolyDP(contours[i], contours_poly[i], 5, true);
        temp_rect = boundingRect(contours_poly[i]); // contours -> 사각형 

      if (rectArea(temp_rect) / target_area * 100 < 4 || rectArea(temp_rect) / target_area * 100 > 15){
            continue;
        }
        cout << rectArea(temp_rect) / target_area << endl;
        cout << contourArea(contours_poly[i]) << endl;
        //필터링된 contours를 사각형으로 변환함
        boundRect.push_back(boundingRect(contours_poly[i]));
    }

    Mat drawing = Mat::zeros(img_pre.size(), CV_8UC3);



    //위에서 구한 사각형 영역을 음식의 영역으로 인식하고
    //각각의 영역만큼 이미지를 crop하여 저장
    frgm_obj my_frgm_obj;
    for (size_t i = 0; i < boundRect.size(); i++)
    {
        drawContours(drawing, contours_poly, (int)i, (0, 0, 255));
        rectangle(src, boundRect[i].tl(), boundRect[i].br(), (0, 0, 255), 2);
        cout << boundRect[i].x << " " << boundRect[i].y << " " << boundRect[i].width << " " << boundRect[i].height << endl;
        my_frgm_obj.crop_imgs.push_back(src(boundRect[i]));
        my_frgm_obj.crop_Rects.push_back(boundRect[i]);
    }
   // imshow("Contours", src);
   // waitKey();

    return my_frgm_obj;
}
