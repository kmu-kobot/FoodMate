#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <pthread.h>
#include <queue>

#include  "Consumer.h"




using namespace cv;
using namespace std;
#define MAX 60







// 0....웹캠 열어주기
VideoCapture vcap;
Mat frame;
// 1....Thread 생성
pthread_mutex_t frameLocker;
pthread_t producer_thread, consumer_thread;
pthread_cond_t empty_queue;
pthread_cond_t fill_queue; // conditional value

// 3....이미지를 담을 큐생성
queue<Mat> frameQueue;

void* producer_run(void* arg);
void* consumer_run(void* arg);

int main(int, char**) {
    vcap.open(0);

    // 클라이언트로써 이미지를 넘겨받아서 
    // 결과를 받는다.






    // 쓰레드를 생산하여 Tracking을 시작한다.
    pthread_mutex_init(&frameLocker, NULL);
    pthread_create(&producer_thread, NULL, producer_run, NULL);
    pthread_create(&consumer_thread, NULL, consumer_run, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}

// 이미지를 담아서 queue에 넣는다.
void* producer_run(void* arg) {

    for (;;) {

        //pthread_mutex_lock(&frameLocker);

        while (frameQueue.size() == MAX) {
            // pthread_cond_wait(&empty_queue, &frameLocker); // �� Ǯ�� ���� �ִ�..
            // ť�� ��ġ�� �׳� ©���! wait x ����� ���ϰ�  pop
            // 
            frameQueue.pop();
        }
        Mat frame;
        vcap >> frame;
        frameQueue.push(frame.clone());
        pthread_cond_broadcast(&fill_queue);
        //pthread_mutex_unlock(&frameLocker);
    }
}

void* consumer_run(void* arg) {
    for (;;) {
        pthread_mutex_lock(&frameLocker);

        while (frameQueue.empty()) {
            pthread_cond_wait(&fill_queue, &frameLocker);
        }
        Mat currentFrame = frameQueue.front();

        // tracking으로 보내준다.

        imshow("consumer image", currentFrame); cv::waitKey(5);
        frameQueue.pop();

        // pthread_cond_signal(&empty_queue);
        pthread_cond_broadcast(&empty_queue);

        pthread_mutex_unlock(&frameLocker);
    }
}