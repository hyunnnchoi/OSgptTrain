//
// Created by HYUNMOK CHOI on 5/8/24.
//

/*
 * TODO: 1. 10개 쓰레드 순차 생성, 쓰레드 생성 시 OSgptTrain 실행
 * TODO: 2. Dataset data[] 에서 data 하나 꺼내고 학습 진행.
 */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>


#define MAX (100)
typedef struct {
    int id;
    double value;
} Dataset;

Dataset data[MAX];
int in = 0;
int out = 0;

pthread_mutex_t mutex;
sem_t empty, full;

void initializeSemaphores(){ // initialize mutex and semaphore
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, MAX); // empty buffer 크기 100으로 초기화
    sem_init(&full, 0, 0); // full buffer 크기 0으로 초기화
}

void OSgptTrain(){
    sem_wait(&full); // full semaphore 데이터 버퍼에 가용 데이터 수 추적
    pthread_mutex_lock(&mutex); // mutex lock

    int idx = out;
    Dataset item = data[idx]; // data에서 하나 읽어옴.

    printf("train id: %d, value: %f\n", item.id, item.value);

    out = (out + 1) % MAX; // index 증가하되, circular queue 처리해서 max 넘지 않도록.

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
}
void OSgptLoad(){
    sem_wait(&empty); // empty semaphore 데이터 버퍼에서 가용 데이터 추적
    pthread_mutex_lock(&mutex);

    int idx = in;
    // 데이터 랜덤 삽입
    data[idx].id = idx;
    data[idx].value = (double)rand() / RAND_MAX * 100.0;

    printf("loading data id %d, value %f\n", data[idx].id, data[idx].value);

    in = (in + 1) % MAX;

    pthread_mutex_unlock(&mutex);
    sem_post(&full);
}

void* trainThreadGenerator(void* vargp){
    OSgptTrain();
    printf("thread generated\n");
}

void* loadThreadGenerator(void* vargp){
    OSgptLoad();
    printf("thread generated\n");
}


void Train(){
    pthread_t threads[10];
    for(int i=0; i<10; i++){
        pthread_create(&threads[i], NULL, trainThreadGenerator, NULL);
    }
    for(int i=0; i<10; i++){
        pthread_join(threads[i],NULL);
    }

}

void Load() {
    pthread_t threads[10];
    for (int i = 0; i < 10; i++) {
        pthread_create(&threads[i], NULL, loadThreadGenerator, NULL);
    }
    for(int i=0; i<10; i++){
        pthread_create(&threads[i], NULL, trainThreadGenerator, NULL);
    }
}
