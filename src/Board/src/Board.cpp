#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <string>
#include "Board.h"
#include "string"

using namespace cv;
using namespace std;


Board::Board() {}

board_obj my_board_obj;
frgm_obj my_frgm_obj;

float Board::rectArea(Rect rect){
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
    blur(src_gray, src_gray, Size(2, 2));

    //canny함수를 이용하여 이미지의 윤곽추출
    Mat canny_output;
    Canny(src_gray, canny_output, thresh, thresh * 2);
    

    return canny_output;
}

void Board::get_target_area()
{
    Mat src = imread("board.jpg");
    resize(src, src, Size(640,480));
    imshow("Contours", src);
    waitKey();

    //이미지에서 식판의 영역만을 추출
    Mat img_pre = img_preproces(src);

    //끊어진 윤곽선을 잇기 위해 canny edge에서 구한 point들을 확장
    Mat mask = getStructuringElement(MORPH_RECT, Size(5, 5), Point(1, 1));
    Mat dilate_edge;
    dilate(img_pre, dilate_edge, mask, Point(-1, -1), 4);

    imshow("Contours", dilate_edge);
    waitKey();


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
    my_board_obj.center_board = Point((boundRect.x + boundRect.width) / 2, (boundRect.y + boundRect.height) / 2);


    //사각형을 관심영역으로 하고 이미지를 crop
    my_board_obj.crop_imgs = src(boundRect);
    imshow("Contours", my_board_obj.crop_imgs);
    waitKey();
    imwrite("target_area.jpg", my_board_obj.crop_imgs);
}

void Board::frgm_board()
{   
    Mat src = imread("target_area.jpg");
    resize(src, src, Size(640,480));
    //사진의 넓이를 구함
    int target_area = src.size().width * src.size().height;

    //이미지 전처리
    Mat img_pre = img_preproces(src);

    Mat mask = getStructuringElement(MORPH_RECT, Size(2, 2), Point(1, 1));
    Mat dilate_edge;
    dilate(img_pre, dilate_edge, mask, Point(-1, -1), 2);

    //이미지 안에서 contours를 찾음
    vector<vector<Point>> contours;
    findContours(dilate_edge, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

    imshow("Contours", dilate_edge);
    waitKey();

    
    vector<vector<Point>> contours_poly(contours.size());
    vector<Rect> boundRect;
    Rect temp_rect;
    float temp_area;

    //일정 크기 이하의 contour를 제거함
    for (size_t i = 0; i < contours.size(); i++)
    {
        approxPolyDP(contours[i], contours_poly[i], 5, true);
        //필터링된 contours를 사각형으로 변환함
        temp_rect = boundingRect(contours_poly[i]);
        
        //전제 영역의 비율에서 너무 작거나 크면 제외
        if (rectArea(temp_rect)/target_area * 100  < 4 || rectArea(temp_rect)/target_area * 100  > 15)
        {
            continue;
        }

        cout<<temp_area/target_area<<endl;
        cout << contourArea(contours_poly[i]) << endl;
        
        //적절한 크기의 영역을 저장
        boundRect.push_back(boundingRect(contours_poly[i]));
    }

    Mat drawing = Mat::zeros(img_pre.size(), CV_8UC3);



    //위에서 구한 사각형 영역을 음식의 영역으로 인식하고
    //각각의 영역만큼 이미지를 crop하여 저장
    for (size_t i = 0; i < boundRect.size(); i++)
    {
        drawContours(drawing, contours_poly, (int)i, (0, 0, 255));
        rectangle(src, boundRect[i].tl(), boundRect[i].br(), (0, 0, 255), 2);
        cout << boundRect[i].x << " " << boundRect[i].y << " " << boundRect[i].width << " " << boundRect[i].height << endl;
        my_frgm_obj.crop_imgs.push_back(src(boundRect[i]));
        my_frgm_obj.crop_Rects.push_back(boundRect[i]);
        imwrite("img/my_frgm_obj" + to_string(i) +  ".jpg",src(boundRect[i]));
    }
    imshow("Contours", src);
    waitKey();

}

int Board::get_center_board_x(){
    return my_board_obj.center_board.x;
}
int Board::get_center_board_y(){
    return my_board_obj.center_board.y;
}

int Board::get_crop_Rects_x(int idex){
    return my_frgm_obj.crop_Rects[idex].x;
}
int Board::get_crop_Rects_y(int idex){
    return my_frgm_obj.crop_Rects[idex].x;

}
int Board::get_crop_Rects_w(int idex){
    return my_frgm_obj.crop_Rects[idex].x;

}
int Board::get_crop_Rects_h(int idex){
    return my_frgm_obj.crop_Rects[idex].x;
}
int Board::get_crop_Rects_size(){
    return my_frgm_obj.crop_Rects.size();
}


// PT_data Board::test(){
//     PT_data pt;
//     pt.x = 1;
//     return pt;
// }

// int main(){
//     Mat src = imread("test2.jpg");
//     resize(src,src,Size(600, 400));
//     Board b = Board();
//     frgm_obj my_frgm_obj;
//     board_obj my_board_obj;
//     my_board_obj = b.get_target_area(src);
//     my_frgm_obj =b.frgm_board(my_board_obj.crop_imgs);

// }  

extern "C" {

    Board* Board_new()
    {
        return new Board();
    }

    Mat board_img_preproces(Board * b, Mat src){ return b -> img_preproces(src);}
    void board_get_target_area(Board * b){b->get_target_area();}
    void board_frgm_board(Board * b){b->frgm_board();}
    float board_rectArea(Board * b, Rect rect){return  b->rectArea(rect);}
    //PT_data test(Board * b){return b->test();} 
    int center_board_x(Board * b){return  b->get_center_board_x();}
    int center_board_y(Board * b){return  b->get_center_board_y();}
    int crop_Rects_x(Board * b, int idex){return  b->get_crop_Rects_x(idex);}
    int crop_Rects_y(Board * b, int idex){return  b->get_crop_Rects_y(idex);}
    int crop_Rects_w(Board * b, int idex){return  b->get_crop_Rects_w(idex);}
    int crop_Rects_h(Board * b, int idex){return  b->get_crop_Rects_h(idex);}
    int crop_Rects_size(Board * b){return b->get_crop_Rects_size();}
}
