#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <queue>
#include "SocketClient.h"
#include <string>
#include <semaphore.h>


using namespace cv;
using namespace std;
#define MAXFRAME 60

sem_t empty;
sem_t full;
sem_t mutex;


// 0....
VideoCapture vcap;
Mat frame;
// 1....Thread ����
// pthread_mutex_t frameLocker;
pthread_t producer_thread, consumer_thread1, consumer_thread2;
// pthread_cond_t empty_queue;
// pthread_cond_t fill_queue; // conditional value

// 2....공유 데이터
queue<Mat> frameQueue;

void* producer_run(void* arg);
void* consumer_run1(void* arg);
void *consumer_run2(void* arg);
int main(int, char**) {
    vcap.open(0);
    sem_init(&empty, 0, MAXFRAME);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    // pthread_mutex_init(&frameLocker, NULL);
    pthread_create(&producer_thread, NULL, producer_run, NULL);
    pthread_create(&consumer_thread1, NULL, consumer_run1, NULL);
    pthread_create(&consumer_thread2, NULL, consumer_run, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread1, NULL);
    pthread_join(consumer_thread2, NULL);

    return 0;
}

// �̹����� �о Queue�� ��� ������
void* producer_run(void* arg) {

    for(;;) {

        //pthread_mutex_lock(&frameLocker);
        
        // while (frameQueue.size() == MAXFRAME) {
        //     // pthread_cond_wait(&empty_queue, &frameLocker); // �� Ǯ�� ���� �ִ�..
        //     // ť�� ��ġ�� �׳� ©���! wait x ����� ���ϰ�  pop
        //     // 
        //     frameQueue.pop();
        // }
        sem_wait(&empty);
        sem_wait(&mutex);
        Mat frame;
        if (frameQueue.size()==MAXFRAME) frameQueue.pop();
        vcap >> frame;
        frameQueue.push(frame.clone());
        sem_post(&mutex);
        sem_post(&full);
        pthread_cond_broadcast(&fill_queue);
        //pthread_mutex_unlock(&frameLocker);
    }
}
// void put(Mat mat){

// }

void *consumer_run1(void* arg) {
    for(;;) {
        pthread_mutex_lock(&frameLocker);

        // while(frameQueue.empty()) {
        //     pthread_cond_wait(&fill_queue, &frameLocker);
        // }
        sem_wait(&empty);
        sem_wait(&mutex);
        if(frameQueue.empty()) {
            sem_post(&mutex);
            sem_post(&full);
        }
        Mat currentFrame = frameQueue.front();
        // 이미지 식판 - > 반찬 영역 구함 -> socket
        frameQueue.pop();
        sem_post(&mutex);
        sem_post(&full);
        // pthread_cond_signal(&empty_queue);
        // pthread_cond_broadcast(&empty_queue);
        // pthread_mutex_unlock(&frameLocker);
    }
}
void *consumer_run2(void* arg) {
    for(;;) {
        pthread_mutex_lock(&frameLocker);

        // while(frameQueue.empty()) {
        //     pthread_cond_wait(&fill_queue, &frameLocker);
        // }
        sem_wait(&empty);
        sem_wait(&mutex);
        if(frameQueue.empty()) {
            sem_post(&mutex);
            sem_post(&full);
        }
        Mat currentFrame = frameQueue.front();
        // 추적
        // imshow("consumer image", currentFrame); cv::waitKey(5);
        frameQueue.pop();
        sem_post(&mutex);
        sem_post(&full);
        // pthread_cond_signal(&empty_queue);
        // pthread_cond_broadcast(&empty_queue);
        // pthread_mutex_unlock(&frameLocker);
    }
}


