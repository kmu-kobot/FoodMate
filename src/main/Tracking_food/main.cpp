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



using namespace cv;
using namespace std;
#define MAXFRAME 60

sem_t empty;
sem_t full;
sem_t mutex1;


// 0....
VideoCapture vcap;
Mat frame;
// 1....Thread ����
// pthread_mutex_t frameLocker;
pthread_t producer_thread, consumer_thread1, consumer_thread2;
ThTracker _ThTracker = ThTracker();
ThDetectRecognizer _ThDetectRecognizer = ThDetectRecognizer();

// 2....공유 데이터

queue<Mat> frameQueue;
vector<pair<string, Rect> > matched_result;


void* producer_run(void* arg);
void* consumer_run1(void* arg);
void* consumer_run2(void* arg);
int main(int, char**) {
    vcap.open(0);
    sem_init(&empty, 0, MAXFRAME);
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

void* producer_run(void* arg) {

    for (;;) {
        // pthread_mutex_lock(&frameLocker);
        while (frameQueue.size() == MAXFRAME) {
            frameQueue.pop();
        }
        sem_wait(&empty);
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
        _ThDetectRecognizer.do_ThDetectRecognizer(currentFrame, matched_result);

        // 이미지 식판 - > 반찬 영역 구함 -> socket
        frameQueue.pop();
        sem_post(&mutex1);
        sem_post(&empty);
       
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

        if (matched_result.size() != 0) {
            _ThTracker.do_ThTracker(currentFrame, matched_result);
        }
       


        frameQueue.pop();
        sem_post(&mutex1);
        sem_post(&empty);
       
    }
}
