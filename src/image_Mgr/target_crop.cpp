#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>  
  
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
        int thresh = 70;
        Mat src_gray;

    public:
        Board(){
            //image load  
            src = imread("Scanned_image.jpg");
            if(src.empty() )
            {
                cout << "Could not open or find the image!\n" << endl;
            }

        }
        
        Mat thresh_callback()
        {
            
        
            cvtColor( src, src_gray, COLOR_BGR2GRAY );
            blur( src_gray, src_gray, Size(3,3) );

            Mat canny_output;
            Canny( src_gray, canny_output,thresh, thresh*2 );
            
            vector<vector<Point> > contours;
            findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
            
            
            vector<vector<Point> > contours_poly( contours.size() );
            vector<Rect> boundRect;
            vector<Point2f>centers( contours.size() );
            vector<float>radius( contours.size() );

            for( size_t i = 0; i < contours.size(); i++ )
            {   
                approxPolyDP( contours[i], contours_poly[i], 20, true );
                
                if(contourArea(contours_poly[i]) < 150 || contourArea(contours_poly[i]) > 1000 ){
                    continue;
                }
                cout<<contourArea(contours_poly[i])<<endl;
                boundRect.push_back(boundingRect(contours_poly[i]));
            
            }

            Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
            
            for( size_t i = 0; i< boundRect.size(); i++ )
            {
                drawContours( drawing, contours_poly, (int)i, (0,0,255) );
                rectangle( src, boundRect[i].tl(), boundRect[i].br(), (0,0,255), 2 );
                cout<<boundRect[i].x<<" "<< boundRect[i].y<<" "<<boundRect[i].width<<" "<<boundRect[i].height<<endl;
                
            }
            imshow( "Contours", src );
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


