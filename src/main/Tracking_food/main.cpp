#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <queue>
#include <SocketClient.h>
#include <string>
#include <semaphore.h>

#include "Consumer1.h"
#include "Consumer2.h"


using namespace cv;
using namespace std;
#define MAXFRAME 60




// 0. 비디오 웹캠을 열어준다.
VideoCapture vcap;
Mat frame;


// 1. Thread 생성 및 소비자 객체 생성 
pthread_t producer_thread, consumer_thread1, consumer_thread2;

Consumer1 consumer1 = Consumer1();
Consumer2 consumer2 = Consumer2();

sem_t empty2;
sem_t full;
sem_t mutex1;




// 2. 공유 데이터 : 이미지를 담을 큐 &  영역과 이름을 매칭한 배열
queue<Mat> frameQueue;
vector<pair<string, Rect> > matching_result;



void* producer_run(void* arg);
void* consumer_run1(void* arg);
void* consumer_run2(void* arg);
int main(int, char**) {
    vcap.open(0);
    sem_init(&empty2, 0, MAXFRAME);
    sem_init(&full, 0, 0);
    sem_init(&mutex1, 0, 1);

    long int status;


    // pthread_mutex_init(&frameLocker, NULL);
    pthread_create(&producer_thread, NULL, producer_run, NULL);
    pthread_create(&consumer_thread1, NULL, consumer_run1, NULL);
    pthread_create(&consumer_thread2, NULL, consumer_run2, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread1, NULL);
    pthread_join(consumer_thread2, NULL);

    return 0;
}

// Queue에 이미지 프레임을 담아둔다.
void* producer_run(void* arg) {

    for (;;) {
        // pthread_mutex_lock(&frameLocker);
        while (frameQueue.size() == MAXFRAME) {
            frameQueue.pop();
        }
        sem_wait(&empty2);
        sem_wait(&mutex1);
        Mat frame;
        if (frameQueue.size() == MAXFRAME) frameQueue.pop();
        vcap >> frame;
        frameQueue.push(frame.clone());
        sem_post(&mutex1);
        sem_post(&full);

    }
}


void* consumer_run1(void* arg) {
    for (;;)
    {

        if (frameQueue.empty()) {
            continue;
        }
        sem_wait(&full);
        sem_wait(&mutex1);

        Mat currentFrame = frameQueue.front();
        //imshow("consumer image", currentFrame);
        //if ((cv::waitKey(10) & 255) == 27) break;
        // consumer1이 궁극적으로 하는 일 : 이미지의 라벨링 정보 update
        consumer1.consumer_doing(currentFrame, matching_result);

        frameQueue.pop();
        sem_post(&mutex1);
        sem_post(&empty2);

    }
}
void* consumer_run2(void* arg) {
    for (;;)
    {

        if (frameQueue.empty()) {
            continue;
        }
        sem_wait(&full);
        sem_wait(&mutex1);

        Mat currentFrame = frameQueue.front();
        //imshow("consumer image2", currentFrame);
        //if (cv::waitKey(20) == 27) break;
        // consumer2가 궁극적으로 하는 일 : 젓가락 추적 및 음식판단 정보 알림 
        consumer2.consumer_doing(currentFrame, matching_result);

        frameQueue.pop();
        sem_post(&mutex1);
        sem_post(&empty2);

    }
}