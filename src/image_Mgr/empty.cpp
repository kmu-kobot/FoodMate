#include "opencv2/opencv.hpp"
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
        Mat img;
        int count = 0;
        vector<int> box_x;
        vector<int> box_y;

    public:
        Board(){
            //image load  
            img = imread("Scanned_image.jpg");

            //cant find image
            if(img.empty()){ cout<<"no image"<<endl;}

        }
        #
        values edgeOfBoard(){
            Mat img_gray;
            Mat img_edge;
            values v;

            cvtColor(img, img_gray, COLOR_BGR2GRAY);
            Canny(img_gray, img_edge, 100, 200);
            
            int rows, cols;
            rows = img_edge.rows;
            cols = img_edge.cols;
            v.x_start, v.x_end, v.y_start, v.y_end = 0;   


            for(int i = 0; i < cols; ++i){
                if(img_edge.at<int>(rows / 8, i) != 0 && v.x_start){
                    v.x_start;
                    }
                else if(img_edge.at<int>(rows / 8, cols - i) != 0 && v.y_start == 0){
                    v.x_end = cols - i;
                }
                if(v.x_start != 0 && v.x_end != 0){
                    break;
                }
            }

            for(int i = 0; i < rows; ++i){
                if(img_edge.at<int>(i, cols/2) != 0 && v.y_start == 0){
                    v.y_start = i;
                }
                else if(img_edge.at<int>(rows - i, cols / 2) != 0 && v.y_end == 0){
                    v.y_end = rows - i;
                }
                if(v.y_start != 0 && v.y_end != 0){
                    break;
                }
            }

            return v;
        }

        void frgm_board(values v){
            int count = 0;
            Mat cut_img = img(Range(v.y_start, v.y_end), Range(v.x_start, v.x_end));

            cut_img = copy(img);

            all_menu = cut_img;

                        
            int rows, cols;
            rows = img_edge.rows;
            cols = img_edge.cols;
            v.x_start, v.x_end, v.y_start, v.y_end = 0;  

            Mat imgBlur;
            Mat gray;
            Mat thr;
            vector<vector<Point>> contours;
	        vector<Vec4i> hierarchy;
            gaussianBlur(img, imgBlur , Size(5, 5), 1.0);

            cvtColor(imgBlur, img_gray, COLOR_BGR2GRAY);

            Laplacian(img_gray,thr, CV_8U, ksize = 3);

            findContours(thr, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

            int cnt = contuors.size();

            for(int i = 0; i < cnt; ++i){
                Rect rt1 =  contours[i].boundingRect();
                if((cols/2 > rt1.width && rt1.width > cols / 10) && (rows/1.5 > rt1.height && rt1.height > rows/4)){
                    rectangle(all_menu, )
                }


            }
            
        }

};


  
int main()  
{  
    values v; 
    Board b = Board();
    b.frgm_board(v);
    return 0;
}

