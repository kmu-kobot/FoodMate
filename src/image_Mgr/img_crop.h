#ifndef IMG_CROP_H
#define IMG_CROP_H

class Board{
    private:

        int thresh;



    public:
        Board(){}

        Mat img_preproces(Mat src){return canny_output;}
        
        Mat get_target_area(Mat src){return crop_img;}

        vector<Mat> thresh_callback(Mat src){return crop_img;}
};

#endif
