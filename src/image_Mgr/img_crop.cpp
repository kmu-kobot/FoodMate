#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>  
#include <string>
  
using namespace cv;  
using namespace std;  




typedef struct value{
    int x_start = 0;
    int x_end = 0;
    int y_start = 0; 
    int y_end = 0;
} values;



class Board{
    private:
        Mat src;
        int count = 0;
        vector<int> box_x;
        vector<int> box_y;
        int thresh = 10;
        Mat src_gray;

    public:
        Board(){
            //image load  
            src = imread("Scanned_image3.jpg");
            //load fail
            if(src.empty() )
            {
                cout << "Could not open or find the image!\n" << endl;
            }

        }

        Mat img_preproces(){
            //이미지 전처리 과정
            //이미지 흑백으로 전환후 노이즈 감소를 위한 블러처리
            cvtColor( src, src_gray, COLOR_BGR2GRAY );
            blur( src_gray, src_gray, Size(3,3) );
            
            //canny함수를 이용하여 이미지의 윤곽추출
            Mat canny_output;
            Canny(src_gray, canny_output,thresh, thresh*2 );    
      
            return canny_output;

        }
        
        Mat get_target_area(){
            //이미지에서 식판의 영역만을 추출
            Mat img_pre = img_preproces();
            
            //끊어진 윤곽선을 잇기 위해 canny edge에서 구한 point들을 확장
            Mat mask = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
            Mat dilate_edge;
            dilate(img_pre, dilate_edge, mask, Point(-1, -1), 3);
          
            //edge를 확장한 이미지에서 contour를 찾음 
            vector<vector<Point> > contours;
            findContours(dilate_edge, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );

            vector<vector<Point> > contours_poly(contours.size() );
            Rect boundRect;
            
            vector<Point> biggest_area = contours[0];
             
            //이미지에서 가장 큰 윤곽선을 찾음 
            for( size_t i = 0; i < contours.size(); i++ )
            {   
                approxPolyDP( contours[i], contours_poly[i], 20, true );
                
                if(contourArea(contours_poly[i]) > contourArea(biggest_area)){
                    biggest_area  = contours_poly[i];
                }
            }
            
            //위에서 찾은 가장큰 윤곽선을 사각형으로 전환
            boundRect = boundingRect(biggest_area);
            rectangle(src, boundRect.tl(), boundRect.br(), (0,0,255), 2 );
            
            //사각형을 관심영역으로 하고 이미지를 crop 
            Mat crop_img = src(boundRect);
            imshow( "Contours", crop_img);
            waitKey();

            
        }



        Mat thresh_callback()
        {

            //이미지 전처리
            Mat img_pre = img_preproces();
            vector<vector<Point> > contours;
           
            //이미지 안에서 contours를 찾음
            findContours(img_pre, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
          
            vector<vector<Point> > contours_poly( contours.size() );
            vector<Rect> boundRect;

            //일정 크기 이하의 contour를 제거함
            for( size_t i = 0; i < contours.size(); i++ )
            {   
                approxPolyDP( contours[i], contours_poly[i], 20, true );
                
                if(contourArea(contours_poly[i]) < 150  ){
                    continue;
                }
                cout<<contourArea(contours_poly[i])<<endl;
                //필터링된 contours를 사각형으로 변환함
                boundRect.push_back(boundingRect(contours_poly[i]));
            
            }
    
            Mat drawing = Mat::zeros( img_pre.size(), CV_8UC3 );
            Mat crop_img;
            int count = 0;
            
            //위에서 구한 사각형 영역을 음식의 영역으로 인식하고 
            //각각의 영역만큼 이미지를 crop하여 저장
            for( size_t i = 0; i< boundRect.size(); i++ )
            {
                drawContours( drawing, contours_poly, (int)i, (0,0,255) );
                rectangle(src, boundRect[i].tl(), boundRect[i].br(), (0,0,255), 2 );
                cout<<boundRect[i].x<<" "<< boundRect[i].y<<" "<<boundRect[i].width<<" "<<boundRect[i].height<<endl;
                crop_img = src(boundRect[i]);
                //imwrite(to_string(count) + ".jpg", crop_img);
                ++count;
            
            }
            imshow( "Contours", src);
            waitKey();

            return src;
        }

};

extern "C" {
 
    Board* Board_new()
    {
        return new Board();
    }
 
    void thresh_callback( Board * b)
    {
        b-> thresh_callback();
    }

}

int main(){
    Board b = Board();
    //b.get_target_area();
    b.thresh_callback();


    
}


