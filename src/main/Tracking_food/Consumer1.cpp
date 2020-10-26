#include "Consumer1.h"

#include <math.h>
#include <utility>
#include <opencv2/opencv.hpp>


using namespace cv;
using namespace std;
//.......... 필요한 객체들 생성
 
//SocketClient client = SocketClient();
//client.create();
//client.connect("127.0.0.1", 10000);
//Board board = Board(); 
//Sound sound = Sound();
Consumer1::Consumer1(){
    client.create();
    client.connect("127.0.0.1", 10000);
}
Consumer1::~Consumer1(){}
void* Consumer1::consumer_doing(const Mat& frame, vector<pair<string, Rect> >& matching_result) {
     cout << "!!!!!!! consumer1  !!!!!!"<<endl;
    // .......1 식판의 영역을 구한다.
    // 중심 check , 식판의 위치가 휘어지면 
    board_obj b_o= board.get_target_area(frame);  //이미지와 중심이 들어있는 구조체 반환
    board.crnt_point = b_o.board_center; // 식판의 중심좌표 update

    // 일정 수준이상 식판이 움직이면 아래의 과정을 수행한다.
    if (abs(board.pre_point.x - board.crnt_point.x) > 25 || abs(board.pre_point.y - board.crnt_point.y) > 25) {
        // 스캔 시작 안내를 띄운다 
       //-- sound.play_sound("scan_start");
        
        
        Mat board_img = b_o.board_img;
        
        // .......2 식판 내의 음식 영역
        frgm_obj f_o = board.frgm_board(board_img); // 이미지와영역벡터가 도출
        
        vector<Mat> frgm_imgs = f_o.crop_imgs;
        vector<Rect> frgm_Rects = f_o.crop_Rects;
        
        client.sendNumber(f_o.crop_imgs.size());
         for (int i = 0; i < frgm_imgs.size(); i++) { // 소켓을 통해 이미지를 전송한다.
            client.sendImage(frgm_imgs[i]);
        }

        // .......3 소켓으로부터 답변을 전송 받는다.
         vector<string> result = client.recv(); // 값이 들어오길 기다린다.
        //if (result.size() != 0) break;
        

        //matching_result를 초기화 시키고 데이터를 담는다.
        matching_result.clear();
        for (int i = 0; i < frgm_Rects.size(); i++) { // 소켓을 통해 받은 결과와 이미지의 이름을 매칭.
            matching_result.push_back({result[i],frgm_Rects[i]});

        }
        // 스캔 완료 안내를 띄운다 
      //--  sound.play_sound("scan_end");
    
    }


    // 현재의 식판의 중심좌표는 과거가 된다.
    board.pre_point = b_o.board_center; 

   

    
    


}
