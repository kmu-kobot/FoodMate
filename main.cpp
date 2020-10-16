#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <queue>
#include "Guess.h"

using namespace cv;
using namespace std;

// 0....웹캠 열어주기
VideoCapture vcap;
Mat frame;
// 1....Thread 생성
pthread_mutex_t frameLocker;
pthread_t UpdThread;


// 2....클래스 객체 생성하기
Guess guess;
// 3....이미지를 담을 큐생성
deque<Mat> frameSeq;

// 이미지를 읽어서 QUEUE에 담는 쓰레드 
void* UpdateFrame(void* arg)
{
    for (;;)
    {
        Mat tempFrame;
        vcap >> tempFrame;

        frame = tempFrame;
        // queue에 이미지 담기
        frameSeq.push_back(frame.clone());
        //if framneSeq    

    }

    pthread_exit((void*)0);
}


int main(int, char**)
{
    vcap.open(0);

    pthread_mutex_init(&frameLocker, NULL);
    pthread_create(&UpdThread, NULL, UpdateFrame, NULL);


    for (;;)
    {
        Mat currentFrame, grayImage;

        pthread_mutex_lock(&frameLocker);
        currentFrame = frame;
        pthread_mutex_unlock(&frameLocker);


        if (currentFrame.empty())
            continue;


        cvtColor(currentFrame, grayImage, COLOR_BGR2GRAY);
        imshow("Input Image", frameSeq.front());
        guess.showimage(currentFrame);

        //ESC키를 누르면 쓰레드를 강제 종료후.. 프로그램 종료한다. 
        if (waitKey(20) == 27)
        {

            int ret = pthread_cancel(UpdThread);
            int status;

            if (ret == 0)
            {
                //자동 종료    
                ret = pthread_join(UpdThread, (void**)&status);
                if (ret == 0)
                    //강제 종료시키면 status로 -1이 리턴된다. 
                    cout << "스레드 종료 성공" << status << endl;
                else
                    cout << "스레드 종료 에러" << ret << endl;

                break;
            }
        }

    }

    return 0;
}