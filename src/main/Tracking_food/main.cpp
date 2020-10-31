#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <queue>
#include <string>
#include <semaphore.h>
#include "ThDetectRecognizer.h"
#include "ThTracker.h"
#include "unistd.h"
#include <JetsonGPIO>           //https://github.com/pjueon/JetsonGPIO
#include <ctime>

using namespace cv;
using namespace std;
//using namespace GPIO;
#define MAXFRAME 60
#define BUTTON 33

//GPIO::setmode(GPIO::BOARD);
//GPIO::setup(BOTTON, GPIO::IN);
//GPIO::cleanup();

sem_t empty;
sem_t full;
sem_t mutex1;

// 0....
VideoCapture vcap;
Mat frame;
// 1....Thread 
// pthread_mutex_t frameLocker;
pthread_t producer_thread, consumer_thread1, consumer_thread2;
ThTracker _ThTracker = ThTracker();
ThDetectRecognizer _ThDetectRecognizer = ThDetectRecognizer();

// 2....공유 데이터
queue<Mat> frameQueue;
vector<pair<string, Rect> > matched_result;
Mat currentFrame;
int push_btn_cnt = 0; // 버튼을 누른 횟수

//
void* producer_run(void* arg);
void* consumer_run1(void* arg);
void* consumer_run2(void* arg);

int main(int, char**) {
    int press_time = 0;     //버튼 누른 시간
    vcap.open(0);
    sem_init(&empty, 0, MAXFRAME);
    sem_init(&full, 0, 0);
    sem_init(&mutex1, 0, 1);

    sleep(3);
    // pthread_mutex_init(&frameLocker, NULL);
    
    cout << "Please press button for 3second";
	while (true) {
        //버튼을 누르지 않으면 press_time에 현재시간 저장
		if (GPIO::input(BUTTON) ==  GPIO::LOW) {
			press_time = time(NULL);
		}
        //버튼을 누르면 누른 시간으로부터 3초 지나고 실행
		if else (GPIO::input(BUTTON) ==  GPIO::HIGH) {
			if (time(NULL) - press_time > 2)
			{
		        pthread_create(&producer_thread, NULL, producer_run, NULL);
			pthread_create(&consumer_thread1, NULL, consumer_run1, NULL);
			pthread_create(&consumer_thread2, NULL, consumer_run2, NULL);
			break;
        	        }
        	}
	}

    
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread1, NULL);
    pthread_join(consumer_thread2, NULL);

    return 0;
}

void* producer_run(void* arg) {
    for (;;) {
        // pthread_mutex_lock(&frameLocker);
       // while (frameQueue.size() == MAXFRAME) {
	//    cout << "꽉찼습니다"<< endl;
      //      frameQueue.pop();
        //}
	    
	while (frameQueue.size() == MAXFRAME){
	    frameQueue.pop();
	}
        sem_wait(&empty); 
        sem_wait(&mutex1);
        Mat frame;
        
        vcap >> frame;
        resize(frame, frame, Size(500, 500));

        frameQueue.push(frame.clone());
        currentFrame = frameQueue.front();
        sem_post(&mutex1);
        sem_post(&full);
    }
}

void* consumer_run1(void* arg) {

    for (;;)
    {
        sem_wait(&full);
        sem_wait(&mutex1);
	while(frameQueue.empty()) {
            sem_post(&mutex1);
            sem_post(&empty);
        }

        if (push_btn_cnt % 5 == 0) { // 식판 인식 버튼 5회눌렀을 경우
            _ThDetectRecognizer.do_ThDetectRecognizer(currentFrame, matched_result, push_btn_cnt);
        }
        // 이미지 식판 - > 반찬 영역 구함 -> socket
        frameQueue.pop();
        sem_post(&mutex1);
        sem_post(&empty);

    }
}
void* consumer_run2(void* arg) {
    for (;;)
    {
//         if (frameQueue.empty()) {
//             continue;
//         }
        sem_wait(&full);
        sem_wait(&mutex1);
	while(frameQueue.empty()) {
            sem_post(&mutex1);
            sem_post(&empty);
        }
	    
      //  if (matched_result.size() != 0) {
        _ThTracker.do_ThTracker(currentFrame, matched_result, push_btn_cnt);
     //   }
        frameQueue.pop();
        sem_post(&mutex1);
        sem_post(&empty);
    }
}
