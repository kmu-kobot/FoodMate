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
            if(src.empty() )
            {
                cout << "Could not open or find the image!\n" << endl;
            }

        }

        Mat img_preproces(){

            cvtColor( src, src_gray, COLOR_BGR2GRAY );
            blur( src_gray, src_gray, Size(3,3) );

            Mat canny_output;
            Canny(src_gray, canny_output,thresh, thresh*2 );    

            return canny_output;

        }
        
        Mat get_target_area(){
            
            Mat img_pre = img_preproces();
            vector<vector<Point> > contours;
            findContours(img_pre, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );

            vector<vector<Point> > contours_poly(contours.size() );
            Rect boundRect;

            Mat mask = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
            Mat dilate_edge;

            dilate(img_pre, dilate_edge, mask, Point(-1, -1), 3);

            vector<Point> biggest_area = contours[0];

            for( size_t i = 0; i < contours.size(); i++ )
            {   
                approxPolyDP( contours[i], contours_poly[i], 20, true );
                
                if(contourArea(contours_poly[i]) > contourArea(biggest_area)){
                    biggest_area  = contours_poly[i];
                }
            }

            boundRect = boundingRect(biggest_area);
            rectangle(src, boundRect.tl(), boundRect.br(), (0,0,255), 2 );
            Mat crop_img = src(boundRect);
            imshow( "Contours", crop_img);
            waitKey();

            
        }



        Mat thresh_callback()
        {

            
            Mat img_pre = img_preproces();
            vector<vector<Point> > contours;
            findContours(img_pre, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
            
            
            vector<vector<Point> > contours_poly( contours.size() );
            vector<Rect> boundRect;

            Mat mask = getStructuringElement(MORPH_RECT, Size(3, 3), Point(1, 1));
            Mat dilate_edge;

            dilate(img_pre, dilate_edge, mask, Point(-1, -1), 1);
            
            imshow( "Contours", dilate_edge);
            waitKey();



            for( size_t i = 0; i < contours.size(); i++ )
            {   
                approxPolyDP( contours[i], contours_poly[i], 20, true );
                
                if(contourArea(contours_poly[i]) < 150  ){
                    continue;
                }
                cout<<contourArea(contours_poly[i])<<endl;
                boundRect.push_back(boundingRect(contours_poly[i]));
            
            }

            Mat drawing = Mat::zeros( img_pre.size(), CV_8UC3 );
            Mat crop_img;
            int count = 0;

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


